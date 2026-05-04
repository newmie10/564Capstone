// generated with Claude Sonnet 4.6 Adaptive
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdint>
#include <ctime>

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// ── Key recovery ─────────────────────────────────────────────────────────────

std::string recover_key() {
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

// ── Base64url helpers ────────────────────────────────────────────────────────

static std::string base64url_encode(const std::vector<uint8_t>& data) {
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

static std::vector<uint8_t> base64url_decode(const std::string& s) {
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

// ── Key splitting ────────────────────────────────────────────────────────────

static bool split_fernet_key(const std::string& key_b64,
                              std::vector<uint8_t>& signing_key,
                              std::vector<uint8_t>& enc_key) {
    auto raw = base64url_decode(key_b64);
    if (raw.size() != 32) return false;
    signing_key = {raw.begin(),      raw.begin() + 16};
    enc_key     = {raw.begin() + 16, raw.end()};
    return true;
}

// ── Fernet encrypt / decrypt ─────────────────────────────────────────────────

std::string fernet_encrypt(const std::string& plaintext, const std::string& key_b64) {
    std::vector<uint8_t> signing_key, enc_key;
    if (!split_fernet_key(key_b64, signing_key, enc_key)) return "";

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

    return base64url_encode(token);
}

std::string fernet_decrypt(const std::string& token_b64, const std::string& key_b64) {
    std::vector<uint8_t> signing_key, enc_key;
    if (!split_fernet_key(key_b64, signing_key, enc_key)) return "";

    auto token = base64url_decode(token_b64);
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

// ── Minimal HTTP helpers ─────────────────────────────────────────────────────

static std::string recv_http_body(int fd) {
    std::string raw;
    raw.reserve(2048);
    char buf[2048];

    while (raw.find("\r\n\r\n") == std::string::npos) {
        int n = recv(fd, buf, sizeof(buf), 0);
        if (n <= 0) break;
        raw.append(buf, n);
    }

    size_t cl_pos = raw.find("Content-Length: ");
    if (cl_pos == std::string::npos) return "";
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

static void send_http_200(int fd) {
    const char* resp = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\nConnection: close\r\n\r\nOK";
    send(fd, resp, strlen(resp), 0);
}

static void http_post(const char* host, int port,
                      const char* path, const std::string& body) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    inet_pton(AF_INET, host, &addr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(sock); return;
    }

    std::string req =
        std::string("POST ") + path + " HTTP/1.1\r\n"
        "Host: " + host + "\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n\r\n" + body;

    send(sock, req.c_str(), req.size(), 0);

    char tmp[256];
    while (recv(sock, tmp, sizeof(tmp), 0) > 0) {}
    close(sock);
}

// ── Main ─────────────────────────────────────────────────────────────────────

int main() {
    const std::string key = recover_key();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return 1;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(8010);

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) return 1;
    listen(server_fd, 5);
    std::cout << "[C++] Server listening on port 8010 …\n" << std::flush;

    int client_fd = accept(server_fd, nullptr, nullptr);
    std::string token = recv_http_body(client_fd);
    std::cout << "\n[C++] ← received encrypted token : " << token << "\n";

    std::string plaintext = fernet_decrypt(token, key);
    std::cout << "[C++] ← decrypted message         : " << plaintext << "\n" << std::flush;

    send_http_200(client_fd);
    close(client_fd);

    const std::string msg       = "Hello from C++!";
    const std::string our_token = fernet_encrypt(msg, key);
    std::cout << "\n[C++] → original message          : " << msg << "\n";
    std::cout << "[C++] → sending encrypted token   : " << our_token << "\n" << std::flush;

    http_post("127.0.0.1", 8000, "/message", our_token);

    close(server_fd);
    return 0;
}
