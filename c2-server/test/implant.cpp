/**
 * GENERATED USING CLAUDE SONNET 4.6 ADAPTIVE
 * fernet_decrypt.cpp
 *
 * Decrypts a Fernet-encrypted token (Python cryptography library compatible).
 *
 * Fernet token layout (after base64url decode):
 *   [0]      : version byte (0x80)
 *   [1..8]   : 64-bit big-endian timestamp (seconds since Unix epoch)
 *   [9..24]  : 128-bit IV
 *   [25..N-32]: ciphertext (AES-128-CBC, PKCS7-padded)
 *   [N-32..N]: HMAC-SHA256 over bytes [0..N-32]
 *
 * Fernet key layout (32 bytes after base64url decode):
 *   [0..15]  : signing key  (used for HMAC-SHA256)
 *   [16..31] : encryption key (used for AES-128-CBC)
 *
 * Build:
 *   g++ -std=c++17 -o fernet_decrypt fernet_decrypt.cpp -lssl -lcrypto
 *
 * Usage:
 *   ./fernet_decrypt <base64url-key> <fernet-token>
 *
 * Example (Python to generate):
 *   from cryptography.fernet import Fernet
 *   key = Fernet.generate_key()
 *   f = Fernet(key)
 *   token = f.encrypt(b"Hello, World!")
 *   print(key.decode(), token.decode())
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdint>

#include <openssl/evp.h>
#include <openssl/hmac.h>

// ─────────────────────────────────────────────────────────────────────────────
// Base64url helpers
// ─────────────────────────────────────────────────────────────────────────────

static const std::string B64_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/** Convert a standard base64 index table to URL-safe variant. */
static std::vector<uint8_t> base64url_decode(const std::string &input)
{
    // Normalise: replace URL-safe chars with standard base64 chars
    std::string s = input;
    for (char &c : s) {
        if (c == '-') c = '+';
        else if (c == '_') c = '/';
    }
    // Re-pad to multiple of 4
    while (s.size() % 4 != 0) s += '=';

    // Decode with OpenSSL BIO or manual lookup
    static const int decode_table[256] = {
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
        int v0 = decode_table[(unsigned char)s[i]];
        int v1 = decode_table[(unsigned char)s[i+1]];
        int v2 = decode_table[(unsigned char)s[i+2]];
        int v3 = decode_table[(unsigned char)s[i+3]];

        if (v0 < 0 || v1 < 0) throw std::runtime_error("Invalid base64 character");

        out.push_back(static_cast<uint8_t>((v0 << 2) | (v1 >> 4)));
        if (s[i+2] != '=') out.push_back(static_cast<uint8_t>(((v1 & 0xF) << 4) | (v2 >> 2)));
        if (s[i+3] != '=') out.push_back(static_cast<uint8_t>(((v2 & 0x3) << 6) | v3));
    }
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
// HMAC-SHA256
// ─────────────────────────────────────────────────────────────────────────────

static std::vector<uint8_t> hmac_sha256(
    const uint8_t *key,   size_t key_len,
    const uint8_t *data,  size_t data_len)
{
    unsigned int out_len = 32;
    std::vector<uint8_t> digest(out_len);
    if (!HMAC(EVP_sha256(), key, static_cast<int>(key_len),
              data, data_len, digest.data(), &out_len))
        throw std::runtime_error("HMAC computation failed");
    return digest;
}

// ─────────────────────────────────────────────────────────────────────────────
// Constant-time comparison (guard against timing attacks)
// ─────────────────────────────────────────────────────────────────────────────

static bool ct_eq(const uint8_t *a, const uint8_t *b, size_t len)
{
    volatile uint8_t diff = 0;
    for (size_t i = 0; i < len; i++) diff |= (a[i] ^ b[i]);
    return diff == 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// AES-128-CBC decrypt with PKCS7 unpadding
// ─────────────────────────────────────────────────────────────────────────────

static std::vector<uint8_t> aes128_cbc_decrypt(
    const uint8_t *enc_key,   // 16 bytes
    const uint8_t *iv,        // 16 bytes
    const uint8_t *ciphertext,
    size_t         ct_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_CIPHER_CTX_new failed");

    std::vector<uint8_t> plaintext(ct_len + EVP_MAX_BLOCK_LENGTH);
    int out_len1 = 0, out_len2 = 0;

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, enc_key, iv) != 1 ||
        EVP_DecryptUpdate(ctx, plaintext.data(), &out_len1, ciphertext, static_cast<int>(ct_len)) != 1 ||
        EVP_DecryptFinal_ex(ctx, plaintext.data() + out_len1, &out_len2) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("AES-128-CBC decryption failed (bad key, IV, or padding)");
    }
    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(static_cast<size_t>(out_len1 + out_len2));
    return plaintext;
}

// ─────────────────────────────────────────────────────────────────────────────
// Main Fernet decryption routine
// ─────────────────────────────────────────────────────────────────────────────

std::string fernet_decrypt(const std::string &key_b64, const std::string &token_b64)
{
    // 1. Decode key (must be exactly 32 bytes)
    std::vector<uint8_t> key = base64url_decode(key_b64);
    if (key.size() != 32)
        throw std::runtime_error("Invalid Fernet key: expected 32 bytes after decoding, got "
                                 + std::to_string(key.size()));

    const uint8_t *signing_key = key.data();       // bytes  0–15
    const uint8_t *enc_key     = key.data() + 16;  // bytes 16–31

    // 2. Decode token
    std::vector<uint8_t> token = base64url_decode(token_b64);

    // Minimum: 1 (version) + 8 (ts) + 16 (IV) + 16 (≥1 AES block) + 32 (HMAC) = 73
    if (token.size() < 73)
        throw std::runtime_error("Token too short to be a valid Fernet token");

    // 3. Check version byte
    if (token[0] != 0x80)
        throw std::runtime_error("Unsupported Fernet version: "
                                 + std::to_string(static_cast<int>(token[0])));

    // 4. Extract fields
    // Timestamp: bytes 1–8
    uint64_t timestamp = 0;
    for (int i = 1; i <= 8; i++)
        timestamp = (timestamp << 8) | token[i];

    // IV: bytes 9–24
    const uint8_t *iv = token.data() + 9;

    // Ciphertext: bytes 25 .. (N-33)
    size_t ct_start = 25;
    size_t ct_end   = token.size() - 32;  // exclusive
    if (ct_end <= ct_start || (ct_end - ct_start) % 16 != 0)
        throw std::runtime_error("Invalid ciphertext length (must be a non-zero multiple of 16)");

    const uint8_t *ciphertext = token.data() + ct_start;
    size_t         ct_len     = ct_end - ct_start;

    // Stored HMAC: last 32 bytes
    const uint8_t *stored_hmac = token.data() + ct_end;

    // 5. Verify HMAC over everything except the last 32 bytes
    std::vector<uint8_t> computed_hmac =
        hmac_sha256(signing_key, 16, token.data(), ct_end);

    if (!ct_eq(computed_hmac.data(), stored_hmac, 32))
        throw std::runtime_error("HMAC verification failed: token is invalid or tampered");

    // 6. Decrypt
    std::vector<uint8_t> plaintext = aes128_cbc_decrypt(enc_key, iv, ciphertext, ct_len);

    return std::string(plaintext.begin(), plaintext.end());
}

std::string recover_key() {
    // XOR-obfuscated halves stored separately
    static constexpr uint8_t H1[] = {
        0x19,0x9A,0x28,0xF2,0xB8,0x2A,0x69,0x8E,0x68,0xA3,0x44,0x81,0x59,0xB7,0xEA,0x24,0x06,0x1E,0xF0,0x5B,0xC9,0x1B
    };

    static constexpr uint8_t H2[] = {
        0x51,0xD9,0xBF,0x54,0x60,0xF9,0x38,0xD3,0x6C,0x33,0x84,0x00,0x8F,0x1C,0xCF,0xA1,0x70,0x22,0xB7,0x57,0xE0,0x61
    };
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

    std::string key(44, '\0');
    for (int i = 0; i < 22; i++) key[i]    = H1[i] ^ M1[i];
    for (int i = 0; i < 22; i++) key[22+i] = H2[i] ^ M2[i];
    std::cout << key << "\n";
    return key;
}

// ─────────────────────────────────────────────────────────────────────────────
// Entry point
// ─────────────────────────────────────────────────────────────────────────────

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <fernet-key> <fernet-token>\n\n"
                  << "Both arguments should be base64url-encoded strings as produced by\n"
                  << "Python's cryptography.fernet.Fernet.\n";
        return 1;
    }

        std::string key = recover_key();
        std::string plaintext = fernet_decrypt(key, argv[1]);
        std::cout << plaintext << "\n";

    return 0;
}
