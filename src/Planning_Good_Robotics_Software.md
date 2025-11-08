# Planning Good Software for Robotics

Inspired by *A Philosophy of Software Design (2nd Edition)* by Dr. John Ousterhout
Integrated with applied lessons for robotics software architecture and intelligent control systems.

---

## Table of Contents

1. [Chapter 1 – It’s All About Complexity](#chapter-1--its-all-about-complexity)
2. [Chapter 2 – The Nature of Complexity](#chapter-2--the-nature-of-complexity)
3. [Chapter 3 – Working Code Isn’t Enough](#chapter-3--working-code-isnt-enough)
4. [Chapter 4 – Modules Should Be Deep](#chapter-4--modules-should-be-deep)
5. [Chapter 5 – Information Hiding and Leakage](#chapter-5--information-hiding-and-leakage)
6. [Chapter 6 – General-Purpose Modules are Deeper](#chapter-6--general-purpose-modules-are-deeper)
7. [Chapter 7 – Different Layer, Different Abstraction](#chapter-7--different-layer-different-abstraction)
8. [Chapter 8 – Pull Complexity Downward](#chapter-8--pull-complexity-downward)
9. [Chapter 9 – Define Errors Out of Existence](#chapter-9--define-errors-out-of-existence)
10. [Chapter 10 – Design It Twice](#chapter-10--design-it-twice)
11. [Chapter 11 – Why Write Comments?](#chapter-11--why-write-comments)
12. [Chapter 12 – Comments Should Describe Design](#chapter-12--comments-should-describe-design)
13. [Chapter 13 – Choosing Names](#chapter-13--choosing-names)
14. [Chapter 14 – Write Small Functions](#chapter-14--write-small-functions)
15. [Chapter 15 – Designing for Performance](#chapter-15--designing-for-performance)
16. [Chapter 16 – Designing for Testability](#chapter-16--designing-for-testability)
17. [Chapter 17 – Managing State](#chapter-17--managing-state)
18. [Chapter 18 – Define Interfaces Precisely](#chapter-18--define-interfaces-precisely)
19. [Chapter 19 – Handle Errors Thoughtfully](#chapter-19--handle-errors-thoughtfully)
20. [Chapter 20 – Code Should Be Obvious](#chapter-20--code-should-be-obvious)
21. [Chapter 21 – Conclusion: Evolving a Design Philosophy](#chapter-21--conclusion-evolving-a-design-philosophy)

---

## Chapter 1 – It’s All About Complexity

**Core Ideas**

* Complexity is the central challenge of software design.
* Two strategies: eliminate complexity and encapsulate what remains.
* Software design is iterative; reducing complexity is a continuous process.

**Key Takeaway**

> Complexity slows progress more than any technical limitation. Every design decision should aim to reduce it.

**🤖 Robotics Lesson**
In robotics, complexity emerges from the interplay of sensing, planning, and actuation.
Design modules that encapsulate these layers:

* Create motion control APIs like `move_to(x, y)` to hide PID and motor details.
* Modularize perception pipelines to isolate camera, LiDAR, and IMU logic.
  This allows hardware or algorithm swaps (e.g., from wheels to legs) without rewriting high-level logic.

---

## Chapter 2 – The Nature of Complexity

**Core Ideas**

* Complexity = what makes code hard to understand or modify.
* Symptoms: change amplification, cognitive load, unknown unknowns.
* Root causes: dependencies and obscurity.

**Key Takeaway**

> Complexity accumulates quietly; the only cure is to simplify relentlessly.

**🤖 Robotics Lesson**
Reduce hidden dependencies between sensors and controllers:

* Standardize units and coordinate frames across modules.
* Document assumptions between hardware and algorithms.
* Avoid coupling calibration data with control logic.
  This ensures safe evolution when adding new sensors or improving algorithms.

---

## Chapter 3 – Working Code Isn’t Enough

**Core Ideas**

* Code that works today may become unmaintainable tomorrow.
* Design quality matters more in the long run than quick fixes.
* Short-term optimizations create long-term debt.

**Key Takeaway**

> The goal is not just working code, but code that will continue to work easily.

**🤖 Robotics Lesson**
In robotics, a system that “just works” in simulation might fail in the field.
Design for maintainability:

* Avoid hardcoded gains or parameters in controllers.
* Use configuration files for tuning.
* Implement logging and replay tools for debugging sensor issues later.

---

## Chapter 4 – Modules Should Be Deep

**Core Ideas**

* A good module hides complex functionality behind a simple interface.
* Interface: small and stable; Implementation: large and evolving.
* Deep modules provide leverage — small surface, big impact.

**Key Takeaway**

> Shallow modules clutter the system; deep modules create clarity and reuse.

**🤖 Robotics Lesson**
Make each ROS2 node deep:

* `LocalizationNode` should expose only `get_pose()` even if it fuses multiple sensors internally.
* Keep motion planning nodes independent of robot geometry details.
  Deep modules simplify integration across heterogeneous robots.

---

## Chapter 5 – Information Hiding and Leakage

**Core Ideas**

* Hide implementation details to allow safe modification.
* Leakage (when internals affect other modules) causes fragile systems.
* Interfaces should reflect *what* not *how*.

**Key Takeaway**

> Design modules so they can change internally without breaking anything else.

**🤖 Robotics Lesson**
Sensor drivers should hide hardware specifics:

* Don’t expose serial protocols or bitmasks.
* Abstract them as standard message types.
  Avoid leaking timing or bandwidth assumptions into control layers.

---

## Chapter 6 – General-Purpose Modules are Deeper

**Core Ideas**

* Reusable, general-purpose modules tend to be deeper.
* Don’t overfit modules to current use cases.
* Flexibility enhances longevity.

**Key Takeaway**

> Design today for tomorrow’s unknown use cases.

**🤖 Robotics Lesson**
Create general-purpose modules:

* A `MapServer` that can handle 2D occupancy or 3D voxel grids.
* A generic task executor that supports both cleaning and delivery robots.
  Avoid assumptions tied to one hardware model.

---

## Chapter 7 – Different Layer, Different Abstraction

**Core Ideas**

* Each software layer should present a new abstraction.
* Avoid redundant or mismatched layers.
* Lower layers handle details; upper layers express goals.

**Key Takeaway**

> Each layer should simplify what’s below, not merely repeat it.

**🤖 Robotics Lesson**
Layer your robot stack:

* Hardware drivers → Middleware (ROS2 topics/services) → Behavior trees → Task planners.
  Keep clear abstraction boundaries so developers can modify one layer safely without side effects.

---

## Chapter 8 – Pull Complexity Downward

**Core Ideas**

* Push complexity to lower layers to simplify higher ones.
* Lower-level modules can encapsulate details better.
* Avoid burdening high-level logic with mechanical details.

**Key Takeaway**

> Simplify the most visible interfaces by absorbing complexity underneath.

**🤖 Robotics Lesson**
Push calibration, unit conversion, and filtering to drivers and middleware.
Allow higher-level nodes to think in clean abstractions like poses and velocities, not raw voltages or encoder ticks.

---

## Chapter 9 – Define Errors Out of Existence

**Core Ideas**

* Simplify design by preventing invalid states.
* Anticipate and design around errors.
* Good abstractions make failure impossible or obvious.

**Key Takeaway**

> The best error handling is designing systems that cannot produce certain errors.

**🤖 Robotics Lesson**
Use strong type systems and assertions:

* Replace raw floats with structured `Pose` or `Velocity` types.
* Prevent invalid coordinate frames.
* In distributed robots, validate message timestamps and frame IDs automatically.

---

## Chapter 10 – Design It Twice

**Core Ideas**

* The first idea is rarely the best.
* Sketch multiple alternatives before committing.
* Compare trade-offs and simplicity.

**Key Takeaway**

> Redesigning early prevents years of technical debt later.

**🤖 Robotics Lesson**
Prototype multiple control architectures:

* Try both centralized and distributed planners.
* Compare RAFT-based consensus vs ROS2 lifecycle coordination.
  Early iteration clarifies which structure scales better to multi-robot environments.

---

## Chapter 11 – Why Write Comments?

**Core Ideas**

* Comments explain *why* code exists, not *what* it does.
* Well-written comments communicate design intent.
* Code alone cannot convey motivation or assumptions.

**Key Takeaway**

> Comments preserve design rationale — your future self will thank you.

**🤖 Robotics Lesson**
Document robot behavior assumptions:

* Why a LiDAR filter uses a 0.2s timeout.
* Why obstacle inflation radius is set to 0.5m.
  This context prevents regression when parameters change.

---

## Chapter 12 – Comments Should Describe Design

**Core Ideas**

* Comments belong at the design level, not line-by-line.
* Explain structure, purpose, and trade-offs.
* Avoid duplicating what’s evident from code.

**Key Takeaway**

> Comment the *thought process*, not the syntax.

**🤖 Robotics Lesson**
Document each node’s purpose and interface in README.md files.
Describe message formats, timing guarantees, and assumptions about real-world performance.

---

## Chapter 13 – Choosing Names

**Core Ideas**

* Names shape understanding.
* Good names communicate role and intent.
* Consistency across modules reduces confusion.

**Key Takeaway**

> A clear name is documentation that never goes out of sync.

**🤖 Robotics Lesson**
Use intuitive naming conventions:

* `odom_frame`, `base_link`, `map_frame` (ROS standard).
* Distinguish between `Planner`, `Controller`, and `Executor` classes.
  Name clarity accelerates collaboration across robot teams.

---

## Chapter 14 – Write Small Functions

**Core Ideas**

* Short functions improve readability and reuse.
* Each function should perform one logical task.
* Easier to test and modify.

**Key Takeaway**

> Simplicity grows from well-factored small pieces.

**🤖 Robotics Lesson**
Split monolithic control loops:

* One function for sensor reading, one for filtering, one for control computation.
  This modularity enables real-time debugging and adaptive upgrades.

---

## Chapter 15 – Designing for Performance

**Core Ideas**

* Don’t prematurely optimize.
* Optimize after identifying real bottlenecks.
* Good design and clarity often yield performance benefits naturally.

**Key Takeaway**

> Start simple; measure, then optimize.

**🤖 Robotics Lesson**
Use profiling before optimizing motion planners.
For distributed robots, measure latency via telemetry before altering algorithms.
Clarity enables predictable timing in real-time robotics.

---

## Chapter 16 – Designing for Testability

**Core Ideas**

* Testability is a hallmark of good design.
* Isolate side effects and external dependencies.
* Use mocks, fakes, and simulation for deterministic testing.

**Key Takeaway**

> If code is hard to test, it’s probably poorly designed.

**🤖 Robotics Lesson**
Integrate Gazebo or Isaac Sim test environments early.
Ensure modules can run standalone with fake sensor data.
This promotes continuous integration of robotics codebases.

---

## Chapter 17 – Managing State

**Core Ideas**

* Minimize mutable state.
* Keep state localized; avoid global variables.
* Make state transitions explicit.

**Key Takeaway**

> Hidden state breeds hidden bugs.

**🤖 Robotics Lesson**
State machines should explicitly represent mission phases.
Use ROS2 lifecycle states (`unconfigured`, `active`, `paused`).
Keep state logs to replay and diagnose mission failures.

---

## Chapter 18 – Define Interfaces Precisely

**Core Ideas**

* Clear contracts prevent misuse.
* Define what inputs mean and what outputs guarantee.
* Validate at boundaries.

**Key Takeaway**

> Strong interfaces create strong systems.

**🤖 Robotics Lesson**
Use IDL (Interface Definition Language) in ROS2 to define messages and services precisely.
Document expected timing and QoS behavior for each topic.
This reduces miscommunication between distributed nodes.

---

## Chapter 19 – Handle Errors Thoughtfully

**Core Ideas**

* Not all errors deserve equal treatment.
* Distinguish between recoverable and fatal errors.
* Fail fast and fail loud when necessary.

**Key Takeaway**

> Good error handling turns chaos into resilience.

**🤖 Robotics Lesson**
Design robots to recover gracefully:

* Retry sensor connections.
* Switch to backup localization when GPS drops.
* Log all exceptions for post-mortem analysis.

---

## Chapter 20 – Code Should Be Obvious

**Core Ideas**

* Readability outweighs cleverness.
* Avoid surprises in control flow or side effects.
* Code should communicate intent at first glance.

**Key Takeaway**

> Write code that your teammate can understand instantly.

**🤖 Robotics Lesson**
Make control logic readable:

* Favor descriptive function names over compact expressions.
* Use visualization tools to show real-time decisions.
  Readable robotics code ensures safer autonomous behavior.

---

## Chapter 21 – Conclusion: Evolving a Design Philosophy

**Core Ideas**

* Software design is an art of continuous refinement.
* Complexity never vanishes; it must be managed.
* The best designs evolve from clear thought and deliberate iteration.

**Key Takeaway**

> Good design is not accidental — it’s cultivated.

**🤖 Robotics Lesson**
Treat your robot’s architecture as a living organism.
Evolve interfaces, documentation, and modularity continuously.
Just as robots learn from experience, robotic software should learn from iteration and feedback.

---

*End of Document*
