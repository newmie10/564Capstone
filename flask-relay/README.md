# Flask Relay

Dockerized Flask app with two purposes:
1. Look like a generic SaaS landing page on the surface (`/`, `/about`, `/contact`).
2. Relay POST bodies between two known peers via IP-restricted endpoints.

## How the relay works

- `POST /api/v1/sync/upstream` — only `PEER_A_IP` may hit this. Body is forwarded to `PEER_B_FORWARD`, response returned.
- `POST /api/v1/sync/downstream` — only `PEER_B_IP` may hit this. Body is forwarded to `PEER_A_FORWARD`, response returned.

Any other source IP gets a 404 (deliberately uninformative — looks like the route doesn't exist).

## Setup

```bash
cp .env.example .env
# edit .env with your real peer IPs and forward URLs
docker compose up --build -d
```

The app listens on host port `8080` by default.

## Config

| Var | Purpose |
|---|---|
| `PEER_A_IP` | Source IP allowed to POST to `/api/v1/sync/upstream` |
| `PEER_B_IP` | Source IP allowed to POST to `/api/v1/sync/downstream` |
| `PEER_A_FORWARD` | URL the **downstream** endpoint forwards to (i.e. delivers to peer A) |
| `PEER_B_FORWARD` | URL the **upstream** endpoint forwards to (i.e. delivers to peer B) |
| `FORWARD_TIMEOUT` | Seconds before forward request times out |
| `TRUST_PROXY` | If `true`, honor `X-Forwarded-For` first hop (only set true behind a trusted reverse proxy) |

## Important caveats

- IP-based allowlisting is fragile. If the app is behind a load balancer or reverse proxy and `TRUST_PROXY=false`, every request will appear to come from the proxy and the allowlist will reject everything. Set `TRUST_PROXY=true` only when you actually have a trusted proxy in front, otherwise you've created a header-spoofable bypass.
- There's no auth beyond IP. If either peer's IP is spoofable on your network path, anyone can hit the relay. For anything serious, add a shared secret header or mTLS.
- Bodies are forwarded as-is. No size limit is enforced; add one (`MAX_CONTENT_LENGTH`) if peers are untrusted.
- The relay is a single hop. Logs include sizes and status codes — strip them in `app.py` if you don't want them.

## Test locally

```bash
# in another terminal, start a sink
python3 -m http.server 8082

# set PEER_A_IP=127.0.0.1 and PEER_B_FORWARD=http://host.docker.internal:8082/
curl -X POST http://localhost:8080/api/v1/sync/upstream -d 'hello'
```
