## Features

- `/checkin` — receives beacon from implant
- `/add_command` — adds a command to the command queue (must be queued before issuing via `/command`)
- `/command` — issues the next queued command to the implant
- `/result` — receives execution output from implant
- Allowlisted commands only (no arbitrary execution)
- Runs in Docker for isolation and reproducibility

---

## Build Instructions

From the `c2-server/` directory:

```bash
docker build -t c2-server-draft .
```

## Run Instructions

```bash
docker run -d -p 5000:5000 --name capstone-c2-server-draft c2-server-draft
```

The server will be available at `http://localhost:5000`.

---

## Using the Web UI

> **Note:** Refresh the page frequently to see updated Queued Commands and Task History.

### 1. Queuing a Command

Type a command into the text bar at the top of the page and click **Send Command**. The command will appear in the Queued Commands list.

### 2. Issuing a Command to the Implant

Click **Fetch & Issue Command** to dequeue the first command and issue it to the implant. It will be removed from the queue and added to Task History.

---

## Example Usage (curl)

### 1. Check-in

```bash
curl -X POST http://localhost:5000/checkin \
  -H "Content-Type: application/json" \
  -d '{"host":"test-machine"}'
```

### 2. Add Command

```bash
curl -X POST http://localhost:5000/add_command \
  -H "Content-Type: application/json" \
  -d '{"command":"whoami"}'
```

> Replace `whoami` with any allowlisted command defined in `app.py`.

### 3. Get Command

```bash
curl http://localhost:5000/command
```

**Response:**

```json
{ "command": "whoami" }
```

### 4. Send Result

```bash
curl -X POST http://localhost:5000/result \
  -H "Content-Type: application/json" \
  -d '{"output":"user"}'
```
