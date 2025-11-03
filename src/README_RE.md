# Robot Embeded mock up interview: Problem descriptions + questions to ask:

## 1- HAL + Logic Separation (LedDriver)

### Problem (what it shows): 
Clean interface boundaries between hardware access and business logic so the same logic runs on MCU and in host tests. Reduces bench time and makes failures reproducible in CI.

### Questions to ask:
- Which HAL style do you prefer (pure interfaces vs C-style function tables)?
- What’s the unit-test strategy for driver logic—host mocks or HIL benches?
- Any constraints on dynamic memory/exceptions/RTTI in firmware?

## 2- ISR→Task SPSC Ring (Deterministic Concurrency):

### Problem: 
Move data safely from an interrupt producer to a task consumer without locks, with bounded memory and correct memory ordering.

### Questions:
- What are your worst-case interrupt latencies and throughput targets?
- Do you require multi-producer or is single ISR source guaranteed?
- Preferred tracing to prove no overruns (high-water marks, drops)?

## 3- UART DMA Double-Buffered TX:

### Problem: 
Sustain throughput without per-byte IRQ load; minimize jitter by DMA chunking and buffer flip in ISR.

### Questions:
- What’s the common TX packet size and target baud rates?
- Is TX completion IRQ reliable across all boards? Any silicon errata?
- Do you prefer zero-copy from an upstream ring to DMA (scatter-gather)?

## 4- CRC-16/CCITT Utility:

### Problem: 
Lightweight, portable integrity check for frames/flash without a table, suitable for bootloaders and OTA.

### Questions:
- Which CRC/Hash do your current protocols standardize on (CRC-16/32, SHA)?
- Is a LUT-accelerated version acceptable for speed vs flash size?

## 5- Fixed-Point PID (Q15) with Anti-Windup:

### Problem:
Real-time control on MCUs without FPU; bounded integral and outputs to avoid actuator saturation.

### Questions:
- Typical control loop rate and timing budget?
- How are gains provisioned (calibration file, cloud, compile-time)?
- Any safety limits or rate limiters required around actuator commands?

### 6- OTA State Machine (CRC-gated, Resumable)

### Problem: 
Safe firmware delivery in chunks, with robust finalize step and a bootloader-read “ready” tag, avoiding partial boots.

### Questions:
- Single-bank vs dual-bank flash? Required atomicity guarantees?
- Rollback policy if post-update health checks fail?
- What telemetry do you collect during OTA (power voltage, RSSI, errors)?

## 7- Safety Interlock (Latched Fault):

### Problem:
Software mirrors hardware e-stop; once tripped, remains latched until a physical reset condition is met.

### Questions:
- What are the hardware safety lines and their priorities?
- Do you require SIL/ISO 13849-style patterns (e.g., diverse redundancy)?
- What events should latch vs auto-recover?

## 8- Boot Breadcrumbs (Reset Reason Telemetry):

### Problem:
Persist minimal counters for boots/watchdogs/brownouts to accelerate field debug and SRE incident analysis.

Questions:
- What reset taxonomy does the current platform expose?
- Wear-leveling expectations and retention lifetime for the counters page?
- Do you ingest these counters into fleet dashboards automatically?


## Integration Story (tying it together):

### Problem: 
Show how the pieces compose: ISR fills SPSC → UART DMA drains; OTA uses CRC; Safety guards actuators; Breadcrumbs inform ops.

### Questions:
- What’s your current bring-up flow—how soon can we exercise this pipeline on new boards?
- Which part causes the most on-call pain today (telemetry gaps, flaky comms, unsafe updates)?
- Any platform-wide coding standards you want these modules to adhere to (MISRA/CERT, no-new/delete, static analyzers)?