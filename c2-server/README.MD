## Features

- `/checkin` — receives beacon from implant
- `/command` — returns a predefined safe command (`whoami`)
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

From the project directory:

```bash
docker build -t c2-server-draft .
```

## Run Instructions

```bash
docker run -d -p 5000:5000 --name capstone-c2-server-draft c2-server-draft
```

The server will be available at `http://localhost:5000`.

---

## Example Usage

### 1. Check-in

```bash
curl -X POST http://localhost:5000/checkin \
  -H "Content-Type: application/json" \
  -d '{"host":"test-machine"}'
```

### 2. Get Command

```bash
curl http://localhost:5000/command
```

**Response:**

```json
{ "command": "whoami" }
```

### 3. Send Result

```bash
curl -X POST http://localhost:5000/result \
  -H "Content-Type: application/json" \
  -d '{"output":"user"}'
```
