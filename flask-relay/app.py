import os
import logging
from flask import Flask, request, jsonify, render_template, abort, Response
import requests
from functools import wraps

app = Flask(__name__)

# Config from env
PEER_A_IP = os.environ.get("PEER_A_IP", "127.0.0.1")  # allowed source for /api/v1/sync/upstream
PEER_B_IP = os.environ.get("PEER_B_IP", "127.0.0.1")  # allowed source for /api/v1/sync/downstream
PEER_A_FORWARD = os.environ.get("PEER_A_FORWARD", "http://127.0.0.1:8081/receive")  # where upstream body gets sent
PEER_B_FORWARD = os.environ.get("PEER_B_FORWARD", "http://127.0.0.1:8082/receive")  # where downstream body gets sent
FORWARD_TIMEOUT = int(os.environ.get("FORWARD_TIMEOUT", "30"))
TRUST_PROXY = os.environ.get("TRUST_PROXY", "false").lower() == "true"

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")
log = logging.getLogger(__name__)


def get_client_ip():
    """Return client IP. If TRUST_PROXY is set, honor X-Forwarded-For first hop."""
    if TRUST_PROXY:
        xff = request.headers.get("X-Forwarded-For", "")
        if xff:
            return xff.split(",")[0].strip()
    return request.remote_addr


def require_ip(allowed_ip_var):
    """Decorator: only allow requests from the configured peer IP."""
    def decorator(f):
        @wraps(f)
        def wrapped(*args, **kwargs):
            allowed = os.environ.get(allowed_ip_var, "")
            client = get_client_ip()
            if not allowed or client != allowed:
                log.warning(f"rejected {client} on {request.path} (allowed={allowed})")
                abort(404)  # 404 not 403, less informative to scanners
            return f(*args, **kwargs)
        return wrapped
    return decorator


# ---------- Decoy / cover routes ----------

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/about")
def about():
    return render_template("about.html")

@app.route("/contact")
def contact():
    return render_template("contact.html")

@app.route("/health")
def health():
    return jsonify({"status": "ok"}), 200

@app.route("/robots.txt")
def robots():
    return Response("User-agent: *\nDisallow:\n", mimetype="text/plain")

@app.route("/favicon.ico")
def favicon():
    return Response(status=204)


# ---------- Relay endpoints ----------

@app.route("/api/v1/sync/upstream", methods=["POST"])
@require_ip("PEER_A_IP")
def relay_upstream():
    """Peer A POSTs here -> body forwarded to PEER_B_FORWARD."""
    body = request.get_data()
    headers = {"Content-Type": request.headers.get("Content-Type", "application/octet-stream")}
    try:
        r = requests.post(PEER_B_FORWARD, data=body, headers=headers, timeout=FORWARD_TIMEOUT)
        log.info(f"upstream relay {len(body)}B -> {PEER_B_FORWARD} ({r.status_code})")
        return Response(r.content, status=r.status_code, content_type=r.headers.get("Content-Type", "application/octet-stream"))
    except requests.RequestException as e:
        log.error(f"upstream relay failed: {e}")
        return jsonify({"error": "upstream unavailable"}), 502


@app.route("/api/v1/sync/downstream", methods=["POST"])
@require_ip("PEER_B_IP")
def relay_downstream():
    """Peer B POSTs here -> body forwarded to PEER_A_FORWARD."""
    body = request.get_data()
    headers = {"Content-Type": request.headers.get("Content-Type", "application/octet-stream")}
    try:
        r = requests.post(PEER_A_FORWARD, data=body, headers=headers, timeout=FORWARD_TIMEOUT)
        log.info(f"downstream relay {len(body)}B -> {PEER_A_FORWARD} ({r.status_code})")
        return Response(r.content, status=r.status_code, content_type=r.headers.get("Content-Type", "application/octet-stream"))
    except requests.RequestException as e:
        log.error(f"downstream relay failed: {e}")
        return jsonify({"error": "downstream unavailable"}), 502


@app.errorhandler(404)
def not_found(e):
    return render_template("404.html"), 404


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
