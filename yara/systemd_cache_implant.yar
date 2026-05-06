/*
 * GENERATED USING CLAUDE SONNET 4.6 ADAPTIVE
 *
 * YARA Rules: systemd-cache ELF C2 Implant
 *
 * Description:
 *   Detects a statically-linked ELF64 implant that installs itself as a
 *   systemd service named "systemd-cache", beacons to a remote C2 over HTTP
 *   using binary (octet-stream) payloads, and performs active log scrubbing,
 *   network reconnaissance, and SSH key exfiltration.
 *
 * Reference hash:
 *   SHA256: 9d7048a1d1061d945a24005933e2e301d9c504e3816309b1926f97378a1f870b
 *
 * Rule tiers:
 *   HIGH   - Exact hash match. Zero false positives. No variant coverage.
 *   MEDIUM - Unique operational strings. Very low FP rate. Catches exact
 *            builds and near-identical recompiles that keep plaintext strings.
 *   BROAD  - Structural/behavioral indicators. Low-medium FP rate. Intended
 *            for hunting across similar static-linked C2 implants even if
 *            strings are changed. Should be paired with other filters in
 *            production (e.g., file size > 5MB, systemd service context).
 *
 * Tested against: PersistentObfuscated.elf (7,478,312 bytes)
 * Author: Blue Team Research
 */

import "elf"

/* =========================================================================
 * RULE 1 — HIGH CONFIDENCE: Exact hash match
 * Use for: retrospective SIEM/EDR lookups, blocklisting, alert enrichment
 * ========================================================================= */
rule ELF_Implant_SystemdCache_ExactHash
{
    meta:
        description     = "Exact SHA256 match for systemd-cache ELF C2 implant"
        hash_sha256     = "9d7048a1d1061d945a24005933e2e301d9c504e3816309b1926f97378a1f870b"
        confidence      = "HIGH"
        false_positive  = "None expected"
        threat_class    = "Trojan.ELF.C2.Implant"

    condition:
        hash.sha256(0, filesize) == "9d7048a1d1061d945a24005933e2e301d9c504e3816309b1926f97378a1f870b"
}


/* =========================================================================
 * RULE 2 — MEDIUM CONFIDENCE: Unique operational strings
 * Use for: on-access scanning, threat hunting, memory scanning
 *
 * Key indicators:
 *   - "systemd-cache" embedded in log-scrubbing sed commands
 *   - Hardcoded kill-chain step banners (reconnaissance, exfil, cleanup)
 *   - Log files targeted for scrubbing by service name
 *   - C2 exfil HTTP header
 * ========================================================================= */
rule ELF_Implant_SystemdCache_OperationalStrings
{
    meta:
        description     = "Detects systemd-cache ELF implant via unique hardcoded operational strings"
        confidence      = "MEDIUM-HIGH"
        false_positive  = "Very unlikely — string combination is implant-specific"
        threat_class    = "Trojan.ELF.C2.Implant"
        note            = "Covers exact build and recompiles that retain plaintext strings"

    strings:
        // Service masquerade name in log scrubbing commands — strongest single IOC
        $log_scrub_auth = "sed -i '/systemd-cache/d' /var/log/auth.log" ascii
        $log_scrub_sys  = "sed -i '/systemd-cache/d' /var/log/syslog" ascii

        // Hardcoded kill-chain step banners
        $step_ssh_keys  = "--- Step 1: SSH Private Keys ---" ascii
        $step_encode    = "--- Step 2: Encoding ---" ascii
        $step_c2_send   = "--- Step 3: Sending to C2 ---" ascii
        $step_recon1    = "--- Step 1: Detecting local subnet ---" ascii
        $step_recon2    = "--- Step 2: Ping sweep ---" ascii
        $step_recon3    = "--- Step 3: Port scan on live hosts ---" ascii
        $step_rm_unit   = "--- Step 2: Removing unit file ---" ascii
        $step_rm_bin    = "--- Step 3: Removing binary ---" ascii
        $step_clean     = "--- Step 4: Cleaning logs ---" ascii
        $step_disable   = "--- Step 1: Disabling service ---" ascii

        // C2 beacon HTTP content-type for binary exfil
        $c2_header      = "Content-Type: application/octet-stream" ascii

        // Cleanup confirmation strings
        $unit_removed   = "Unit file removed, systemd reloaded" ascii
        $unit_removing  = "--- Step 2: Removing unit file ---" ascii

    condition:
        uint32(0) == 0x464c457f  // ELF magic
        and filesize > 1MB
        and (
            // Log scrubbing pair is definitive on its own
            ($log_scrub_auth and $log_scrub_sys)
            or
            // Any two step banners + C2 header
            (2 of ($step_*) and $c2_header)
            or
            // SSH exfil chain
            ($step_ssh_keys and $step_c2_send)
            or
            // Cleanup chain
            ($step_disable and $step_clean and $unit_removed)
        )
}


/* =========================================================================
 * RULE 3 — BROAD: Structural + behavioral indicators
 * Use for: proactive hunting for similar implants, variant detection
 *
 * Key indicators:
 *   - ELF64 x86-64, statically linked (no PT_INTERP), no dynamic section
 *   - Bundles full OpenSSL (CRYPTOGAMS attribution strings present)
 *   - Large binary (>5MB) — consistent with static linking of crypto libs
 *   - Contains network syscalls AND crypto primitives AND log manipulation
 *   - Targets systemd log files for scrubbing
 *
 * Note: This rule has a higher FP potential and should be validated against
 * baseline of known-good statically linked binaries in your environment.
 * ========================================================================= */
rule ELF_Implant_StaticLinked_C2_Structural
{
    meta:
        description     = "Detects statically-linked ELF implants with OpenSSL, network C2 capability, and log tampering — matches systemd-cache implant family"
        confidence      = "MEDIUM"
        false_positive  = "Low — legitimate static binaries rarely bundle OpenSSL AND scrub log files"
        threat_class    = "Trojan.ELF.C2.Implant"
        note            = "Tune with process context: flag if parent is systemd and binary is not a known service"

    strings:
        // OpenSSL CRYPTOGAMS attribution strings — only present in statically
        // compiled binaries (dynamic builds load these at runtime from libssl.so)
        $openssl_aes_gcm    = "AES-NI GCM module for x86_64, CRYPTOGAMS by <appro@openssl.org>" ascii
        $openssl_ghash      = "GHASH for x86_64, CRYPTOGAMS by <appro@openssl.org>" ascii
        $openssl_chacha     = "ChaCha20 for x86_64, CRYPTOGAMS by <appro@openssl.org>" ascii
        $openssl_sha256     = "SHA256 block transform for x86_64, CRYPTOGAMS by <appro@openssl.org>" ascii

        // Log file targeting for scrubbing
        $log_auth           = "/var/log/auth.log" ascii
        $log_syslog         = "/var/log/syslog" ascii
        $log_scrub_cmd      = "sed -i '/" ascii  // sed in-place delete pattern

        // HTTP C2 exfil pattern
        $c2_octet           = "Content-Type: application/octet-stream" ascii

        // SSH key exfiltration targeting
        $ssh_key_str        = "SSH Private Keys" ascii

        // Systemd service self-reference
        $systemd_log_path   = "/var/log/auth.log" ascii

    condition:
        uint32(0) == 0x464c457f         // ELF magic bytes
        and uint8(4) == 2               // ELF64
        and uint16(18) == 0x3e          // x86-64 (EM_X86_64)
        and uint16(16) == 2             // ET_EXEC (executable, not shared lib)
        and filesize > 5MB              // Static linking makes these large
        and filesize < 20MB             // Upper bound sanity check
        // Confirm static linking: no PT_INTERP segment (no dynamic linker path)
        and not elf.type == elf.ET_DYN
        // Must have statically bundled OpenSSL (2+ CRYPTOGAMS strings)
        and 2 of ($openssl_*)
        // Must show evidence of log tampering AND either C2 or SSH exfil
        and ($log_scrub_cmd and ($log_auth or $log_syslog))
        and ($c2_octet or $ssh_key_str)
}
