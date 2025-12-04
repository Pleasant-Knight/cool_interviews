# Ad Hoc Networks, Mesh Networks, MANETs and Swarms Introduction

This document provides a clear, modern, and interview‑ready explanation of **Ad Hoc Networks**, **Wireless Mesh Networks**, and **Mobile Ad Hoc Networks (MANETs)**. It includes concise definitions, architectures, use cases, metrics, challenges, and simple diagrams to visualize their operation.

---

# 🛰️ 1. Ad Hoc Networks

## **Definition**

An **ad hoc network** is a decentralized wireless network formed without fixed infrastructure. Nodes communicate directly and can forward packets for others.

Key properties:

* Infrastructure‑less
* Nodes self-organize
* Multi-hop routing possible
* Dynamic topology

## **Architecture Diagram (ASCII)**

```
   [Node A] ---- [Node B] ---- [Node C]
       \                            /
        -------- [Node D] ---------
```

Characteristics:

* Nodes join/leave anytime
* No central access point
* Routing decided collaboratively

## **Strengths**

* Rapid deployment
* No dependency on central infrastructure
* Resilient to single point failures

## **Weaknesses**

* Harder to manage
* Limited scalability
* Less efficient than infrastructure networks

---

# 🕸️ 2. Wireless Mesh Networks (WMN)

## **Definition**

A **mesh network** is a type of ad hoc network where nodes (routers or devices) connect in a mesh topology to expand coverage and improve reliability.

Mesh networks can be:

* **Full Mesh** — every node connects to every other node
* **Partial Mesh** — nodes connect selectively based on range or cost

## **Architecture Diagram: Full Mesh**

```
       [A]------[B]
        | \    / |
        |  \  /  |
       [C]--\/---[D]
           /\
          /  \
        [E]--[F]
```

## **Characteristics**

* Redundant paths → highly fault-tolerant
* Stable compared to MANET
* Often used for city-wide WiFi, battlefield networks, disaster recovery

## **Strengths**

* Scalable
* Self-healing routes
* Works with fixed or semi-mobile nodes

## **Weaknesses**

* More routing overhead
* Requires smarter routing protocols (e.g., OLSR, BATMAN)
* Still sensitive to interference

---

# 🚁 3. MANET (Mobile Ad Hoc Network)

## **Definition**

A **Mobile Ad Hoc Network (MANET)** is an ad hoc network where **all nodes are mobile**. Topology changes frequently and unpredictably.

Used in:

* UAV swarms
* Naval unmanned systems
* Soldier radios
* Disaster response

## **Architecture Diagram: MANET in Motion**

```
Initial:
    [UAV1] ---- [UAV2] ---- [UAV3]
        \                     |
         \                   [UAV4]

After Movement:
    [UAV1]        [UAV2] ---- [UAV4]
        \             \
         \             [UAV3]
```

Mobility forces:

* Frequent route recalculation
* Rapidly changing link quality
* Need for adaptive routing

## **Routing Protocols**

* **AODV** – on-demand, reactive
* **OLSR** – proactive, table‑based
* **DSR** – source routing
* **BATMAN** – each node chooses next hop based on best originator messages

## **Strengths**

* Operates without infrastructure
* Highly flexible
* Good for tactical, remote, or denied environments

## **Weaknesses**

* High routing overhead
* Links break often (mobility) → packet loss
* Limited throughput due to multi-hop wireless

---

# 📡 4. Key Differences Summary

| Feature           | Ad Hoc           | Mesh                   | MANET                |
| ----------------- | ---------------- | ---------------------- | -------------------- |
| Infrastructure    | None             | None or minimal        | None                 |
| Mobility          | Optional         | Low/Medium             | High                 |
| Routing Stability | Moderate         | High                   | Low                  |
| Use Case          | Quick networking | City WiFi, battlefield | UAV swarms, soldiers |
| Fault Tolerance   | Medium           | High                   | Variable             |

---

# ⚙️ 5. Core Challenges Across These Networks

### **1. Routing in dynamic environments**

* Topology changes → routing table churn
* High mobility → stale routes quickly

### **2. Interference & RF propagation**

* Multi-hop amplifies latency
* High packet loss in contested environments

### **3. Limited bandwidth & spectrum**

* Wireless medium shared by all nodes

### **4. Energy constraints (for mobile platforms)**

* Routing consumes CPU and radio power

### **5. Security vulnerabilities**

* No central authority → authentication is hard
* Susceptible to jamming, spoofing

---

# 🧠 6. Relevance to Multi-Domain AI Autonomy Context

For UAVs, USVs, and UUVs operating together:

* MANET supports **vehicle-to-vehicle coordination**
* Mesh networking ensures **resilience** if one node drops
* Ad hoc design allows **operation without infrastructure**
* Routing protocols must adapt to mobility, water reflection, Doppler shifts, etc.

Networking directly affects:

* Task allocation
* Swarm consensus
* Distributed autonomy
* Real-time telemetry

---

# 📚 7. Recommended Further Reading (Non-Obsolete)

* *RFC 3626 – OLSR Routing Protocol*
* *BATMAN-adv Official Documentation*
* *A Survey of MANET Protocols (ACM Computing Surveys)*
* *ns-3 MANET Simulation Examples*

---

# 🔄 8. Routing Protocols for Swarms (Introduction)

Routing protocols are the **core of distributed autonomy**. For UAVs, USVs, and UUVs operating in fast-changing, contested, or GPS‑denied environments, routing must:

* Adapt instantly to topology changes
* Minimize latency for command & control
* Ensure delivery even with intermittent links
* Recover from node failures automatically
* Work without centralized infrastructure

Swarm routing must consider:

* **Mobility models** (e.g., UAV flight paths, maritime drift)
* **Dynamic link quality** (RF obstruction, multipath, Doppler)
* **Energy constraints** (routing vs mission execution)
* **Bandwidth prioritization** (telemetry, video, autonomy signals)

Three of the most common routing options used in ad hoc robotics systems are:

* **AODV** (Reactive / On-demand)
* **OLSR** (Proactive / Table-based)
* **BATMAN** (Hybrid originator-based)

Below is a detailed comparison.

---

# ⚔️ 9. AODV vs OLSR vs BATMAN (Routing Protocol Comparison)

## **High-Level Summary Diagram**

```
 Reactive (On-demand)      Proactive (Always routing)     Hybrid (Next-hop focus)
      AODV                        OLSR                         BATMAN
```

## **Conceptual Behavior**

```
AODV:
  Route only when needed
  ↓
  Lower overhead, but slower for first packet

OLSR:
  Maintain full routing tables at all times
  ↓
  Higher overhead, but instant forwarding

BATMAN:
  Nodes know ONLY their best next-hop
  ↓
  Extremely robust to mobility & failures
```

---

## **Detailed Comparison Table**

| Feature               | **AODV**                          | **OLSR**                         | **BATMAN / BATMAN-adv**                    |
| --------------------- | --------------------------------- | -------------------------------- | ------------------------------------------ |
| Routing Type          | Reactive                          | Proactive                        | Hybrid / Originator-based                  |
| Best For              | Energy saving, light traffic      | Dense networks, high traffic     | Highly dynamic topologies                  |
| Control Overhead      | Low                               | High                             | Medium                                     |
| Initial Latency       | High (route discovery)            | Very low                         | Very low                                   |
| Mobility Handling     | Moderate                          | Weak under high mobility         | Strong — mobility-tolerant                 |
| Complexity            | Medium                            | High                             | Low/Medium                                 |
| Link Awareness        | Route breaks frequent             | Tries to maintain all links      | Continuously measures link quality         |
| Multi-hop Performance | Good                              | Degrades with mobility           | Excellent                                  |
| Kernel Integration    | No                                | No                               | Yes (BATMAN‑adv in Linux Kernel)           |
| Best Use Cases        | UAVs/UUVs with intermittent comms | Mesh with fixed/semi-fixed nodes | UAV swarms, field robotics, tactical comms |

---

# 📡 10. Additional Diagrams

## **AODV Route Discovery (Simplified)**

```
[Source] -- RREQ --> (unknown path) -- ... --> [Destination]
                      ^ receives RREP back
[Source] <-- RREP <-------------------------- [Destination]
```

Characteristics:

* Route Request (RREQ): flood
* Route Reply (RREP): unicast
* Route maintained until broken

## **OLSR Multipoint Relay (MPR) Concept**

```
   [Node A]
   /  |   \
[B] [C]  [D]
  \   |   /
   [E]-[F]
```

Only selected **MPR nodes** rebroadcast announcements → reduces overhead.

## **BATMAN Next-Hop Decision**

```
[Node X]
  | Best link
  v
[Node Y] --> Rest of network
```

Node X doesn’t know the full path — only which neighbor leads to the best connectivity.

---

# 🚀 11. Why Routing Protocols Matter for Swarms

Routing controls how fast, reliably, and effectively autonomous systems can:

* Share state
* Publish telemetry
* Coordinate actions
* Execute multi-agent missions

In swarm robotics, routing performance directly affects:

* Formation control
* Consensus algorithms
* Collaborative mapping
* Distributed task allocation
* Collision avoidance
* Command & control reliability

Bad routing ⇢ failed autonomy.
Excellent routing ⇢ mission success.

---

---

# 🛠️ 12. Installing / Using Each Network Type on Linux & Other OS

This section provides practical guidance on how to deploy ad hoc, mesh, and BATMAN/MANET configurations on Linux (primary platform) and notes for other OS.

## **12.1 Ad Hoc Mode (802.11 IBSS)**

Ad hoc networks use **IBSS mode**.

### **Linux Installation & Setup (Linux Mint/Ubuntu/Debian/Kali)**

```
sudo apt update
sudo apt install wireless-tools net-tools

# Create ad hoc network
sudo iwconfig wlan0 mode ad-hoc
sudo iwconfig wlan0 essid myAdhocNet
sudo iwconfig wlan0 channel 6
sudo ifconfig wlan0 10.0.0.1 netmask 255.255.255.0 up
```

### **Check**

```
iwconfig
ip addr
```

### **References**

* Arch Wiki: Wireless Ad Hoc – [https://wiki.archlinux.org/title/Wireless_ad-hoc_network](https://wiki.archlinux.org/title/Wireless_ad-hoc_network)
* Linux Wireless Docs – [https://wireless.wiki.kernel.org/](https://wireless.wiki.kernel.org/)

### **Windows / macOS**

* Native ad hoc support is mostly deprecated.
* Must use **hosted network** (Windows) or a WiFi adapter that supports IBSS.
* Best option: external USB Wi-Fi NIC with Linux.

---

## **12.2 Mesh Networking (802.11s)**

Linux supports 802.11s natively via **mac80211**.

### **Linux Installation & Setup**

```
sudo apt install iw

# Set to mesh mode
sudo iw dev wlan0 set type mp
sudo ip link set wlan0 down
sudo iw dev wlan0 mesh join myMesh freq 2412
sudo ip link set wlan0 up

# Assign IP
sudo ip addr add 192.168.50.1/24 dev wlan0
```

### **Check mesh peers**

```
iw dev wlan0 mpath dump
iw dev wlan0 station dump
```

### **References**

* Linux 802.11s Mesh: [https://wireless.wiki.kernel.org/en/developers/documentation/802.11s](https://wireless.wiki.kernel.org/en/developers/documentation/802.11s)
* YouTube 802.11s demo: "Linux Mesh 802.11s"

### **Windows / macOS**

* No native 802.11s support.
* Requires specialized hardware (OpenWRT routers, Silvus radios, etc.)

---

## **12.3 BATMAN / BATMAN-adv (Layer 2 Mesh)**

BATMAN-adv is built into the Linux kernel and is the **best mesh option for robotics**.

### **Linux Installation**

```
sudo apt update
sudo apt install batctl

# Load kernel module
sudo modprobe batman-adv

# Add interface
sudo ip link set wlan0 down
sudo iwconfig wlan0 mode ad-hoc essid batman channel 1
sudo ip link set wlan0 up

# Attach to BATMAN
sudo batctl if add wlan0
sudo ip link set up dev bat0
sudo ip addr add 10.0.1.1/24 dev bat0
```

### **Check neighbors**

```
batctl n
batctl o
batctl p <IP>
```

### **References**

* BATMAN-adv Wiki: [https://www.open-mesh.org/projects/batman-adv/wiki](https://www.open-mesh.org/projects/batman-adv/wiki)
* GitHub Tools: [https://github.com/open-mesh-mirror/batctl](https://github.com/open-mesh-mirror/batctl)

### **Windows / macOS**

* Not supported.
* Must run Linux (native or VM with USB Wi-Fi passthrough).

---

# 🧭 13. Swarm Networking Architectures (Centralized, Decentralized, Hybrid)

This section explains how multi-robot swarms organize communication and decision-making.

## **13.1 Centralized Swarm Architecture**

```
                +------------------------+
                |  Central Mission Brain |
                | (Ground Station / CNS) |
                +-----------+------------+
                            |
       -------------------------------------------------
       |             |              |                |
    [Robot 1]     [Robot 2]     [Robot 3] ...     [Robot N]
```

### **Characteristics**

* One central controller assigns tasks.
* Robots send telemetry upward.
* Easy to manage, but fragile.
* If central node fails → swarm collapses.

### **Used In**

* Early UAV systems
* Simple surveillance swarms
* Centralized fleet management systems

---

## **13.2 Decentralized (Fully Distributed) Swarm Architecture**

```
 [Robot A] <----> [Robot B] <----> [Robot C]
     ^                |                 |
     |                v                 v
 [Robot D] <--------> [Robot E] <----> [Robot F]
```

### **Characteristics**

* No central controller.
* Each robot shares state with neighbors.
* Consensus, gossip, or peer-to-peer routing.
* Extremely robust → no single point of failure.
* Harder to coordinate global tasks.

### **Used In**

* Battlefield MANETs
* Underwater robotics
* High-resilience UAV/UUV swarms

---

## **13.3 Hybrid Swarm Architecture (Most Realistic)**

```
                   +-------------------------+
                   |  Orchestration Layer    |
                   | (Optional / Intermitt.) |
                   +-----------+-------------+
                               |
           ------------------------------------------------
           |                 |                 |           
      [Local Subswarm]  [Local Subswarm]  [Local Subswarm]
           |   |   |         |   |   |         |   |   |
        Robots share with neighbors; global planning optional
```

### **Characteristics**

* Mix of centralized planning + distributed execution.
* Each subswarm operates independently.
* Coordinator is *optional* → resilient.
* Best performance for large, multi-domain operations.

### **Used In**

* NODA AI “URZA-like” systems
* UAV–USV multi-domain operations
* Search & rescue
* Heterogeneous mission packages

---

If you want, I can add:

* A **simulation section** (ns-3 setup for AODV, OLSR, and BATMAN)
* A **Swarm Task Allocation + Networking Interaction** diagram
* A **Troubleshooting guide** for mesh/MANET deployments
  by adding:
* A *Routing Protocols for Swarms — Deep Dive* section
* A separate **Routing Protocols Simulation Examples (ns-3 scripts)** section
* Or a diagram showing **centralized vs decentralized swarm networking architectures**
