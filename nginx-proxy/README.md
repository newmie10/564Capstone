# Nginx Reverse Proxy (cover site + transparent passthrough)

Nginx serves a normal-looking marketing landing page at `/` and transparently reverse-proxies everything under `/api/v1/*` to your existing Flask container, with the `/api/v1/` prefix stripped. Method, headers, query string, and body all pass through unmodified.

## Layout

```
nginx-proxy/
├── docker-compose.yml
└── nginx/
    ├── conf.d/default.conf      # proxy config + path rewrite
    └── html/index.html          # cover landing page
```

## What gets forwarded

The device hits:
```
GET  http://<host>/api/v1/whatever
POST http://<host>/api/v1/anything
```

Flask receives:
```
GET  /whatever          (with all original headers + query string)
POST /anything          (with all original headers + body)
```

Nginx adds three headers so your Flask app can see the real client:
- `X-Real-IP`
- `X-Forwarded-For`
- `X-Forwarded-Proto`

Everything else (Content-Type, Authorization, custom headers, body, etc.) is passed through untouched.

## Setup

You need nginx and your Flask container on the **same docker network**, and nginx needs to resolve your Flask container by name. Pick the option that matches your setup.

### Option A — Your Flask container is already on a docker network

1. Find the network name:
   ```bash
   docker inspect <your-flask-container-name> --format '{{range $k,$v := .NetworkSettings.Networks}}{{$k}} {{end}}'
   ```
2. Edit `docker-compose.yml`, replace the bottom `networks:` block with:
   ```yaml
   networks:
     proxy_net:
       external: true
       name: <that-network-name>
   ```
3. Make sure the Flask service is reachable as `flask` on that network. If it has a different name, edit `nginx/conf.d/default.conf` and change:
   ```
   proxy_pass http://flask:5000;
   ```
   to use the actual container/service name and port.

### Option B — Let this compose file create the network

1. Keep `docker-compose.yml` as-is, then:
   ```bash
   docker compose up -d
   docker network connect nginx-proxy_proxy_net <your-flask-container-name>
   ```
2. If the Flask container isn't named `flask`, edit `proxy_pass` in `nginx/conf.d/default.conf` to point at the right name (or rename the container).

## Run

```bash
cd nginx-proxy
docker compose up -d
```

Stop:
```bash
docker compose down
```

Logs:
```bash
docker compose logs -f nginx
```

## Test

Cover site:
```bash
curl http://localhost/
```

Through the proxy to Flask (assuming Flask has e.g. `@app.route('/health')`):
```bash
curl http://localhost/api/v1/health
```

POST with body and custom headers — both pass through:
```bash
curl -X POST http://localhost/api/v1/telemetry \
  -H "Content-Type: application/json" \
  -H "X-Device-ID: abc123" \
  -d '{"temp":42.7}'
```

## Customization

- **Different prefix** (e.g. `/cdn/` or `/static/v2/`): change both occurrences in `nginx/conf.d/default.conf` — the `location /api/v1/ {` line and the `rewrite ^/api/v1/(.*)$` line.
- **Different cover page**: drop your own files into `nginx/html/`.
- **Different upstream port**: edit `proxy_pass http://flask:5000;`.
- **Run on a different host port**: change `"80:80"` in `docker-compose.yml`.
- **Bigger uploads**: raise `client_max_body_size`.

After any nginx config change:
```bash
docker compose restart nginx
```

## Caveats

- HTTP only, as you specified — traffic is plaintext on the wire.
- No auth, no rate limiting. Anyone who guesses the `/api/v1/` prefix reaches Flask. The cover site is obscurity, not security.
- `proxy_buffering off` is set so streaming/long responses work cleanly. If your endpoints return large payloads and you'd rather nginx absorb them before sending to the client, flip that to `on`.
