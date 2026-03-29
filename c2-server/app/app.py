from flask import Flask, request, jsonify
import os

app = Flask(__name__)

# Allowlisted safe commands
ALLOWED_COMMANDS = {
    "whoami": "whoami",
    "hostname": "hostname",
    "pwd": "pwd"
}

@app.route("/")
def home():
    return "<h1>C2 Server Running</h1>"

@app.route("/checkin", methods=["POST"])
def checkin():
    data = request.json
    print("[+] Check-in received:", data)
    return jsonify({"status": "ok"})

@app.route("/command", methods=["GET"])
def command():
    # Always return a safe demo command
    return jsonify({"command": "whoami"})

@app.route("/result", methods=["POST"])
def result():
    data = request.json
    print("[+] Result received:", data)
    return jsonify({"status": "received"})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)