/**
 * fernet_encrypt.cpp
 *
 * Encrypts a message using Fernet (Python cryptography library compatible).
 *
 * Fernet token layout (before base64url encoding):
 *   [0]       : version byte (0x80)
 *   [1..8]    : 64-bit big-endian timestamp (seconds since Unix epoch)
 *   [9..24]   : 128-bit IV (random)
 *   [25..N-32]: ciphertext (AES-128-CBC, PKCS7-padded)
 *   [N-32..N] : HMAC-SHA256 over bytes [0..N-32]
 *
 * Build:
 *   g++ -std=c++17 -o fernet_encrypt fernet_encrypt.cpp -lssl -lcrypto
 *
 * Usage:
 *   ./fernet_encrypt "message to encrypt"
 */

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

// ─────────────────────────────────────────────────────────────────────────────
// Key recovery  — paste your H1 / H2 from the offline generator
// ─────────────────────────────────────────────────────────────────────────────

static constexpr uint8_t M1[] = {
    0x4F, 0xA3, 0x7C, 0x91, 0xE2, 0x5B, 0x38, 0xD6,
    0x0E, 0xF7, 0x29, 0xB4, 0x6A, 0xC5, 0x83, 0x1D,
    0x70, 0x4E, 0x9F, 0x22, 0xAB, 0x57
};
static constexpr uint8_t M2[] = {
    0x3B, 0x88, 0xF1, 0x64, 0x0D, 0xC9, 0x52, 0xA7,
    0x1E, 0x75, 0xEC, 0x30, 0xB6, 0x4D, 0x97, 0xF3,
    0x2A, 0x61, 0xDE, 0x08, 0x93, 0x5C
};
static constexpr uint8_t H1[] = {
    0x19,0x9A,0x28,0xF2,0xB8,0x2A,0x69,0x8E,0x68,0xA3,0x44,0x81,0x59,0xB7,0xEA,0x24,0x06,0x1E,0xF0,0x5B,0xC9,0x1B
};

static constexpr uint8_t H2[] = {
    0x51,0xD9,0xBF,0x54,0x60,0xF9,0x38,0xD3,0x6C,0x33,0x84,0x00,0x8F,0x1C,0xCF,0xA1,0x70,0x22,0xB7,0x57,0xE0,0x61
};

static std::string recover_key()
{
    static_assert(sizeof(H1) == 22, "H1 must be 22 bytes — run the offline generator first");
    static_assert(sizeof(H2) == 22, "H2 must be 22 bytes — run the offline generator first");
    static_assert(sizeof(M1) == 22, "M1 must be 22 bytes");
    static_assert(sizeof(M2) == 22, "M2 must be 22 bytes");

    std::string key(44, '\0');
    for (int i = 0; i < 22; i++) key[i]    = H1[i] ^ M1[i];
    for (int i = 0; i < 22; i++) key[22+i] = H2[i] ^ M2[i];
    return key;
}

// ─────────────────────────────────────────────────────────────────────────────
// Base64url encode
// ─────────────────────────────────────────────────────────────────────────────

static std::string base64url_encode(const uint8_t *data, size_t len)
{
    static const char *b64 =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string out;
    out.reserve(((len + 2) / 3) * 4);

    for (size_t i = 0; i < len; i += 3) {
        uint32_t v  = (uint32_t)data[i] << 16;
        if (i+1 < len) v |= (uint32_t)data[i+1] << 8;
        if (i+2 < len) v |= (uint32_t)data[i+2];

        out += b64[(v >> 18) & 0x3F];
        out += b64[(v >> 12) & 0x3F];
        out += (i+1 < len) ? b64[(v >>  6) & 0x3F] : '=';
        out += (i+2 < len) ? b64[(v >>  0) & 0x3F] : '=';
    }

    // Convert to URL-safe base64
    for (char &c : out) {
        if (c == '+') c = '-';
        else if (c == '/') c = '_';
    }
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// Base64url decode  (needed to decode the key)
// ─────────────────────────────────────────────────────────────────────────────

static std::vector<uint8_t> base64url_decode(const std::string &input)
{
    std::string s = input;
    for (char &c : s) {
        if (c == '-') c = '+';
        else if (c == '_') c = '/';
    }
    while (s.size() % 4) s += '=';

    static const int T[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1, 0,-1,-1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
    };

    std::vector<uint8_t> out;
    out.reserve(s.size() * 3 / 4);
    for (size_t i = 0; i < s.size(); i += 4) {
        int v0 = T[(uint8_t)s[i]],   v1 = T[(uint8_t)s[i+1]];
        int v2 = T[(uint8_t)s[i+2]], v3 = T[(uint8_t)s[i+3]];
        if (v0 < 0 || v1 < 0) throw std::runtime_error("Invalid base64");
        out.push_back((uint8_t)((v0 << 2) | (v1 >> 4)));
        if (s[i+2] != '=') out.push_back((uint8_t)(((v1 & 0xF) << 4) | (v2 >> 2)));
        if (s[i+3] != '=') out.push_back((uint8_t)(((v2 & 0x3) << 6) | v3));
    }
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// HMAC-SHA256
// ─────────────────────────────────────────────────────────────────────────────

static std::vector<uint8_t> hmac_sha256(
    const uint8_t *key, size_t key_len,
    const uint8_t *data, size_t data_len)
{
    unsigned int out_len = 32;
    std::vector<uint8_t> digest(out_len);
    if (!HMAC(EVP_sha256(), key, (int)key_len, data, data_len, digest.data(), &out_len))
        throw std::runtime_error("HMAC computation failed");
    return digest;
}

// ─────────────────────────────────────────────────────────────────────────────
// AES-128-CBC encrypt (PKCS7 padding applied automatically by OpenSSL)
// ─────────────────────────────────────────────────────────────────────────────

static std::vector<uint8_t> aes128_cbc_encrypt(
    const uint8_t *enc_key,    // 16 bytes
    const uint8_t *iv,         // 16 bytes
    const uint8_t *plaintext,
    size_t         pt_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_CIPHER_CTX_new failed");

    std::vector<uint8_t> ciphertext(pt_len + EVP_MAX_BLOCK_LENGTH);
    int out_len1 = 0, out_len2 = 0;

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, enc_key, iv) != 1 ||
        EVP_EncryptUpdate(ctx, ciphertext.data(), &out_len1, plaintext, (int)pt_len) != 1 ||
        EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len1, &out_len2) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("AES-128-CBC encryption failed");
    }
    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize((size_t)(out_len1 + out_len2));
    return ciphertext;
}

// ─────────────────────────────────────────────────────────────────────────────
// Fernet encrypt
// ─────────────────────────────────────────────────────────────────────────────

std::string fernet_encrypt(const std::string &key_b64, const std::string &message)
{
    // 1. Decode the 32-byte Fernet key
    std::vector<uint8_t> key = base64url_decode(key_b64);
    if (key.size() != 32)
        throw std::runtime_error("Invalid Fernet key: expected 32 bytes");

    const uint8_t *signing_key = key.data();       // bytes  0–15
    const uint8_t *enc_key     = key.data() + 16;  // bytes 16–31

    // 2. Generate a random 16-byte IV
    uint8_t iv[16];
    if (RAND_bytes(iv, sizeof(iv)) != 1)
        throw std::runtime_error("RAND_bytes failed");

    // 3. Get current timestamp (big-endian 64-bit)
    uint64_t ts = (uint64_t)std::time(nullptr);

    // 4. Encrypt the plaintext
    std::vector<uint8_t> ciphertext = aes128_cbc_encrypt(
        enc_key, iv,
        reinterpret_cast<const uint8_t *>(message.data()), message.size());

    // 5. Assemble the token payload (everything except the HMAC)
    //    version(1) + timestamp(8) + iv(16) + ciphertext
    size_t payload_len = 1 + 8 + 16 + ciphertext.size();
    std::vector<uint8_t> payload;
    payload.reserve(payload_len + 32);  // +32 for HMAC

    payload.push_back(0x80);            // version

    for (int i = 7; i >= 0; i--)       // big-endian timestamp
        payload.push_back((ts >> (i * 8)) & 0xFF);

    payload.insert(payload.end(), iv, iv + 16);
    payload.insert(payload.end(), ciphertext.begin(), ciphertext.end());

    // 6. Compute HMAC-SHA256 over the payload and append it
    std::vector<uint8_t> mac = hmac_sha256(
        signing_key, 16, payload.data(), payload.size());

    payload.insert(payload.end(), mac.begin(), mac.end());

    // 7. Base64url-encode the full token
    return base64url_encode(payload.data(), payload.size());
}

// ─────────────────────────────────────────────────────────────────────────────
// Entry point
// ─────────────────────────────────────────────────────────────────────────────

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " \"message to encrypt\"\n";
        return 1;
    }

    try {
        std::string key = recover_key();
        std::string token = fernet_encrypt(key, argv[1]);

        // Wipe key from memory immediately after use
        OPENSSL_cleanse(key.data(), key.size());

        std::cout << token << "\n";
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
