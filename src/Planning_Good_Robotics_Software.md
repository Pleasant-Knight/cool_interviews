# Planning A Good Robotics Software - By Michel Hanna - Software Engineer

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

** Two Ways to Fight Complexity:**
* Eliminate complexity: make code simpler and more obvious (e.g., avoid special cases, use consistent naming).
* Encapsulate complexity: isolate it within well-defined modules (modular design), allowing developers to work without knowing the entire system’s details.
* Complexity should guide every design decision. Simplify and encapsulate relentlessly.

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

* Complexity = Anything related to the structure of a software system that makes it hard to understand and modify the system.
* Complexity is experienced in what a developer feels when trying to understand, debug, or extend a codebase.
* Symptoms: change amplification, cognitive load, unknown unknowns.
* In simple systems, changes are obvious.
* In complex systems, every change is risky and unpredictable.
* Root causes: dependencies and obscurity.
* Complexity Is Incremental, once it has accumulated, removing it is very hard.


**Key Takeaway**

> Complexity accumulates quietly; the only cure is to simplify relentlessly.
> Complexity is the silent killer of progress. 
> Every design choice either fights complexity or feeds it.
> Great software design is the art of making change easy

**🤖 Robotics Lesson**
Robotics complexity often manifests in inter-module dependencies between sensors, planners, & actuators.
Reduce hidden dependencies between sensors and controllers.
Suppose each robot subsystem (e.g., camera, motor, navigation) directly depends on others’ internal parameters.
Changing the camera resolution might break path planning because image scaling logic is buried in multiple modules → change amplification.
The solution is encapsulation and clarity:
* Standardize units and coordinate frames across modules.
* Document assumptions between hardware and algorithms.
* Avoid coupling calibration data with control logic.
  This ensures safe evolution when adding new sensors or improving algorithms.

---

## Chapter 3 – Working Code Isn’t Enough (Strategic vs. Tactical Programming)

**Core Ideas**

* Code that works today may become unmaintainable tomorrow.
* Design quality matters more in the long run than quick fixes.
* Short-term optimizations create long-term debt.
* The goal should be to create clean, maintainable, and extensible designs.
* Tactical programming focuses on short-term results — getting a feature implemented or a bug fixed as fast as possible.
* Strategic programming, in contrast, is about investing in the future structure of the system.
* Most technical debt is never fully repaid; you keep paying forever.
* Avoid the Startup Trap — “Move Fast and Break Things”

**Reocommendation**: spending 10–20% of your time on design improvement and cleanup, enough to prevent complexity from growing faster than you can control. This includes:
* Simplifying confusing interfaces.
* Refactoring redundant or inconsistent code.
* Improving documentation and comments.
* Reducing dependencies between modules.

**Strategic Programming in Practice**
Principles to adopt:
 - Pause when you see a red flag. Don’t patch over bad design.
- Refactor continuously. Small cleanups are better than massive rewrites.
- Think long-term. Ask, “Will this make future changes easier or harder?”
- Balance practicality with quality. 
- Don’t over-engineer, but never settle for fragile hacks.
- “Your primary goal is to produce a great design that also happens to work.”

**Key Takeaway**

> The goal is not just working code, but code that will continue to work easily.
> “Good design doesn’t come for free, it’s an investment. But it pays for itself, and sooner than you might think.”
>  The real goal of software development is to build systems that stay simple, clear, and easy to change as they grow.

**🤖 Robotics Lesson**
In robotics, a system that “just works” in simulation might fail in the field.
Design for maintainability:

* Avoid hardcoded gains or parameters in controllers.
* Use configuration files for tuning.
* Implement logging and replay tools for debugging sensor issues later.
* A strategically designed robot control system lets engineers innovate safely, adding new sensors or motion behaviors without rewriting the core control logic.

---

## Chapter 4 – Modules Should Be Deep

### 🧩 Modular Design, The Foundation of Simplicity:
Good software is composed of deep modules; modules with simple interfaces that conceal rich, complex implementations.
Modular design is the process of dividing a system into smaller, relatively independent parts (modules), each responsible for a specific piece of functionality. Each module has two parts:
* Interface: What the module promises to do (the part other modules see).
* Implementation: How it actually does it (the hidden complexity).

In an ideal world, each module would be completely independent, then the complexity of the system would be that of its most complex module.
While complete independence is impossible, we can minimize dependencies through good interface design.

### The Principle of Depth:
The best modules are deep; they provide a lot of functionality through a simple interface.
Module depth as the ratio between:
 - Benefit (Functionality): How much useful work the module performs.
 - Cost (Interface Complexity): How much information others must know to use it.

**Deep module = high functionality / low interface complexity.**
A deep module hides complexity. Developers can rely on it without worrying about its internal machinery.
By contrast, shallow modules expose too much of their inner workings; their interfaces are as complex as their implementations.
“A shallow module is one whose interface is complicated relative to the functionality it provides.”

### Abstraction and Its Pitfalls
Each module provides an abstraction, a simplified view that omits unnecessary details. 
But abstractions can fail in two ways:
 1. Too detailed: The interface includes things that users don’t need to know (increasing cognitive load).
 → Example: An API that exposes every low-level control flag.
 1. Too vague: It hides important information that users do need to know, creating confusion and bugs.

The art of design is deciding what really matters and exposing only that.
“The key to designing abstractions is to understand what is important, and to minimize what’s important.”

### Classitis — When “Small” Goes Too Far
Many programmers, especially influenced by object-oriented dogma, believe that: 
*“Classes should be small.”*
Ousterhout calls this classitis →the mistaken belief that more classes automatically means better design.
In reality:
 - Too many small, shallow classes increase the number of interfaces.
 - Every interface adds complexity to the overall system.
 - Systems like the Java class library suffer from this — a single task (like file reading) might require instantiating three or more wrapper classes (FileInputStream, BufferedInputStream, ObjectInputStream).

This makes simple operations verbose and error-prone → “Small classes don’t contribute much functionality, so there have to be a lot of them… each with its own interface. That’s complexity.”

### How to Design Deep Modules
1. Expose less information.
    - The fewer assumptions other modules need to know, the better.
1. Hide as much as possible.
    - Implementation details, data structures, and internal policies should stay private.
1. Combine functionality.
    - If multiple related features share information, bring them into the same module.
1. Favor powerful, general-purpose interfaces.
    - Avoid special-case methods; one method that handles multiple cases is deeper.
1. Don’t be afraid of big modules — only shallow ones.
    - A large module with a simple interface is better than many small, trivial ones.

**Core Ideas**

* A good module hides complex functionality behind a simple interface.
* Interface: small and stable; Implementation: large and evolving.
* Deep modules provide leverage — small surface, big impact.
* Deep modules let programmers reason about a complex system one simple part at a time.

**Key Takeaway**

> Shallow modules clutter the system; deep modules create clarity and reuse.
> The most important issue in designing classes and other modules is to make them deep.
> It’s more important for a module to have a simple interface than a simple implementation.

**🤖 Robotics Lesson**
In robotics, deep modules can dramatically reduce cognitive load: A motion control module might provide only a few high-level functions:
 - move_to(target_position)
 - set_speed(speed)
 - stop()

Internally, it could manage:
 - PID control loops,
 - Sensor feedback,
 - Obstacle avoidance,
 - Safety limits,
 - Hardware synchronization.

By hiding all of this behind a simple interface, roboticists can focus on behavior and planning instead of motor dynamics. In contrast, a shallow design (where each subsystem exposes too many knobs and parameters) would make every integration brittle and time-consuming.

Make each ROS2 node deep:

* `LocalizationNode` should expose only `get_pose()` even if it fuses multiple sensors internally.
* Keep motion planning nodes independent of robot geometry details.
* Deep modules simplify integration across heterogeneous robots.

---

## Chapter 5 – Information Hiding and Leakage

### 🧩 1. Information Hiding — The Core of Deep Design
Information hiding means encapsulating the details of a design decision within a single module so that no other module depends on it.
The goal is to make it possible to change those details later without affecting the rest of the system. 

 - **Examples of Information to Hide:**
    - How data is stored (e.g., internal data structures).
    - How a protocol works internally.
    - The format of a file or message.
    - Configuration policies or performance optimizations.
 - **When this information is hidden:**
    - The module’s interface becomes simpler, reducing cognitive load.
    - Fewer parts of the code need to be modified if something changes.
    - Complexity becomes localized, not contagious.

### Information Leakage — The Enemy of Modularity
Information leakage occurs when a design decision is reflected in multiple modules, creating unnecessary dependencies. 
If that decision changes, all of those modules must change too.
Examples of Leakage:
 - Two classes that both know about a shared file format.
 - APIs that expose internal data structures.
 - Shared assumptions or duplicated constants.

Leakage is dangerous because:
 - It ties modules together conceptually.
 - It increases the surface area of complexity.
 - It hides dependencies, making bugs harder to track.
 - “Information leakage is one of the most important red flags in software design. Learn to develop a high sensitivity to it.”

### Common Source of Leakage — Temporal Decomposition
A classic mistake that causes information leakage is temporal decomposition, structuring the code based on the order of operations, rather than on what knowledge each part should encapsulate.
Example of a system reads, modifies, and then writes a file:
 - Bad design: Separate classes for reading and writing→ Both classes must understand the file format (leakage).
 - Better design: One class handles both reading and writing → The file format knowledge is contained in one place.
 - “It’s easy to fall into the trap of temporal decomposition, but it almost always leads to information leakage.”


**Core Ideas**

* Hide implementation details to allow safe modification.
    * “Each module should encapsulate a few pieces of knowledge that represent design decisions."
    * "That knowledge is embedded in the implementation but invisible in the interface.”
* Leakage (when internals affect other modules) causes fragile systems.
* Interfaces should reflect *what* not *how*.
* Balance is key:
    - Expose what’s necessary, hide everything else. 
    - “If the information is needed outside the module, you must not hide it. The goal is to minimize what’s needed outside, not to pretend it doesn’t exist.”


**Key Takeaway**

> Design modules so they can change internally without breaking anything else.
> Complexity spreads when knowledge spreads.
    > Keep each piece of knowledge in one place, hide it there, and your system will stay simple, deep, and adaptable.
> “If you can hide more information, you can simplify the interface and that makes the module deeper.”

**🤖 Robotics Lesson**
In robotics, information hiding can dramatically reduce system fragility.
Example, A motor control subsystem might handle:
 - PID tuning, torque limits, voltage compensation, and encoder calibration internally.
 - If other modules (like path planners) know these details, any change in hardware requires rewriting the planner, that’s information leakage.

Better design --> **Expose only high-level commands**:
 - motor.move_to(angle)
 - motor.set_speed(velocity)
 - The internal control logic, feedback loops, and safety features remain hidden.
 - If the robot switches to a different motor type, the interface stays the same, only the internal module changes.

Sensor drivers should hide hardware specifics:

* Don’t expose serial protocols or bitmasks.
* Abstract them as standard message types.
  Avoid leaking timing or bandwidth assumptions into control layers.

---

## Chapter 6 – General-Purpose Modules are Deeper

**Core Ideas**

* Generality reduces complexity.
    * Code that is somewhat general-purpose tends to be simpler, cleaner, and more reusable than code that’s narrowly specialized.
* Reusable, general-purpose modules tend to be deeper.
* The more general-purpose interface reduced the number of methods, simplified the code, and improved information hiding.
* Don’t overfit modules to current use cases.
* Flexibility enhances longevity.

### Why Generality Leads to Better Design
Ousterhout explains that generality increases depth because:
 - It hides more knowledge — specialized details are kept within the module.
 - It reduces duplication — multiple special cases become one unified mechanism.
 - It lowers cognitive load — fewer methods to learn and maintain.
 - It supports new features naturally — without refactoring.

### How to Balance Generality
You shouldn’t make everything universal. Over-generalization can make APIs abstract, bloated, or unintuitive. 
So Ousterhout gives three guiding questions:
1. What’s the simplest interface that covers all current needs?
    - If you can combine several special methods into one simple one, do it.
1. In how many situations will this method be used?
    - If a method serves only one caller, that’s a red flag — it may be too special-purpose.
1. Is this API easy to use for today’s needs?
    - If it’s so general that current tasks require lots of extra code, you’ve gone too far.
1. “General-purpose interfaces are deeper and simpler — as long as they’re still convenient for current needs.”

### Pushing Specialization Up or Down
Not all specialization is bad; but it should be placed in the right layer.
1. Push Specialization Upward
    - High-level modules (like user interfaces or applications) can be special-purpose since they define unique behavior.
    - Example: The text editor UI knows about cursors, selections, and keystrokes.
    - Those specifics belong at the top of the stack, not inside the text storage class.
1. Push Specialization Downward
    - Sometimes specialization is required at the hardware or driver level.
    - Example: Different disk drivers know the quirks of their specific devices.
    - The operating system kernel defines a general driver interface like read_block() and write_block(), and the specialized behavior lives beneath it.
1. “Specialized code should be separated cleanly from general-purpose code — either above or below.”

###  Eliminating Special Cases
Special cases in code are small but deadly. They add:
 - Extra branches (if/else clutter),
 - Hidden complexity,
 - And subtle bugs.
 - The *antidote* is to design normal-case logic that automatically handles edge conditions.


**Key Takeaway**

> Design today for tomorrow’s unknown use cases.
> “Unnecessary specialization is a major source of complexity. Generality makes code simpler, not harder.”


**🤖 Robotics Lesson**
In robotics, general-purpose modules lead to cleaner, more flexible architectures:
❌ Specialized Design:
A robot’s motion controller has methods like:
 - turn_left()
 - turn_right()
 - move_forward()
 - Each method hardcodes parameters — specialized and brittle.

✅ General-Purpose Design:
Instead, a motion control API might offer:
 - move(direction_vector, speed) or 
 - set_target_pose(x, y, orientation)
This single interface can support walking, turning, or reversing and allows higher-level behaviors (like obstacle avoidance) to reuse the same API.


Create general-purpose modules:

* A `MapServer` that can handle 2D occupancy or 3D voxel grids.
* A generic task executor that supports both cleaning and delivery robots.
* Avoid assumptions tied to one hardware model.

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
