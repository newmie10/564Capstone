# generated with Claude Sonnet 4.6 Adaptive
import threading
import time
import urllib.request
from cryptography.fernet import Fernet
from http.server import BaseHTTPRequestHandler, HTTPServer

KEY = "V9TcZqQXfTm53ri9vPoybLjQN0m0jtrFh09QXRZCi_s="
f   = Fernet(KEY)

def encrypt(message: str) -> bytes:
    return f.encrypt(message.encode())

def decrypt(token: bytes) -> str:
    return f.decrypt(token).decode()

# ── Handler ──────────────────────────────────────────────────────────────────

class Handler(BaseHTTPRequestHandler):
    def do_POST(self):
        length  = int(self.headers.get("Content-Length", 0))
        token   = self.rfile.read(length)

        print(f"\n[Python] ← received encrypted token : {token.decode()}")
        plaintext = decrypt(token)
        print(f"[Python] ← decrypted message         : {plaintext}")

        self.send_response(200)
        self.send_header("Content-Length", "2")
        self.end_headers()
        self.wfile.write(b"OK")

    # silence default request logs so our prints are readable
    def log_message(self, fmt, *args):
        pass

# ── Sender ───────────────────────────────────────────────────────────────────

def send_to_cpp():
    """Wait for the C++ server to be ready, then push one encrypted message."""
    time.sleep(0.8)
    msg   = "Hello from Python!"
    token = encrypt(msg)
    print(f"[Python] → original message          : {msg}")
    print(f"[Python] → sending encrypted token   : {token.decode()}")
    req = urllib.request.Request(
        "http://127.0.0.1:8010/message",
        data    = token,
        headers = {"Content-Type": "application/octet-stream"},
    )
    urllib.request.urlopen(req, timeout=5)

# ── Main ─────────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    server = HTTPServer(("0.0.0.0", 8000), Handler)
    print("[Python] Server listening on port 8000 …")
    threading.Thread(target=send_to_cpp, daemon=True).start()
    server.serve_forever()
