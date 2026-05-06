## Final Deliverable Grading Rubric (60 pts)
The final deliverable is graded using the rubric below.

## Category/Requirements (what you must demonstrate in your presentation)

### 1. Zero-Click Exploitation (6 pts)
Reliable remote exploit gains root without user action or notification
(No metasploit/msfconsole. Utilize proof-of-concept projects on GitHub.) **4 pts**

Two-stage delivery (loader/beachhead/dropper → full payload) **2 pts**

### 2. Implant Capability (15 pts)
(Off-the-shelf implants may be used as reference only. An off-the-shelf implant earns 0 points.)

≥ 4 built-in multi-step tasks that the implant can run on a single trigger* **6 pts**

On-demand arbitrary command execution (as root) with output returned to
operator **3 pts**

Persists after reboot / power loss (auto-starts itself)* **2 pts**

Self-destruct functionality that wipes binaries, logs, persistence mechanism, etc. **2 pts**

Meaningful file exfiltration (not just command output) * **2 pts**

### 3. Stealth (15 pts)
Implant is hidden, RE-resistant, and prevents/cleans its own logs on the target
system **5 pts**

The payload that reaches the target should be a compiled, stripped binary—no plaintext comments,
debug symbols, or hard-coded secrets. **2 pts**

C2 traffic is encrypted, non-predictable, obfuscated (different than exfil), and with
at least one ”hop” between operator and implant **5 pts**

Exfil traffic is encrypted, non-predictable, obfuscated (different than C2), and
with at least one ”hop” between operator and implant **5 pts**

### 4. Concurrent Command and Control (2 pts)
Demonstrate ability to concurrently manage and task multiple implants where
implants on different targets operate independently* **2 pts**

### 5. Code Quality & Reproducibility (5 pts)
Concise README in GitHub with project purpose, build/run steps, threat model
diagram (hosts, hops, trust boundaries) **3 pts**

Single operator action triggers a full chain (not manual step-by-step) from exploit
→ implant → task → exfil **2 pts**

### 6. Performance & Footprint (4 pts)
CPU/RAM profile on target (top, perfmon) < 5% sustained **2 pts**
Network throughput graph (Wireshark, pcap stats) shows low, burst-only traffic **2 pts**

### 7. Presentation (13 pts)
Final video is a sub 10min, with professional slides and a single cut end-to-end
demonstration, not just a combination of previous videos. **5 pts**
Explains all concepts, including CVE and exploit logic, at a high level **4 pts**
Clear narration, pacing, clean audio, and readable text/graphics **2 pts**
Lessons learned / future work **2 pts**

Subtotal 60 pts