## Features

- `/checkin` — receives beacon from implant
- `/add_command` — adds a command to the command queue. Commands must be added to the queue first before the user wishes to use "/command".
- `/command` — returns the specified command
- `/result` — receives execution output from implant
- Allowlisted commands only (no arbitrary execution)
- Runs in Docker for isolation and reproducibility

---

## Project Structure

```
c2-server/
├── app.py
├── requirements.txt
├── Dockerfile
└── README.md
```

---

## Build Instructions

From the c2-server/ directory (or wherever the Dockerfile lives):

```bash
docker build -t c2-server-draft .
```

## Run Instructions

```bash
docker run -d -p 5000:5000 --name capstone-c2-server-draft c2-server-draft
```

````

The server will be available at `http://localhost:5000`.

---

#Using the http://localhost:5000 UI (Note, refreshing the web page frequently is needed to see changes to Queued commands and Task History.)

### 1. Queuing a Command

At the top of the screen, there should be a text bar where a command can be typed/copied into and submitted to the queue. Upon submitting, the "Queued Commands" list will get updated.

### 2. Issuing the Command to the Implant

Below that, there should be a button called "Fetch and Issue Command". Clicking that button will take the first command in the queue and issue it to the implant. Removes that command from the queue, and adds it to Task History on the website.

## Example Usage with curl

### 1. Check-in

```bash
curl -X POST http://localhost:5000/checkin \
  -H "Content-Type: application/json" \
  -d '{"host":"test-machine"}'
````

### 2. Add Command

```bash
curl.exe -X POST http://localhost:5000/add_command -H "Content-Type: application/json" -d "{\"command\":\"whoami\"}"
```

**Note that the "whoami" in -d "{\"command\":\"whoami\"} can be changed to any allowlisted command within app.py**

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
