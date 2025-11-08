# C++ Classic & Modern Features — Quick Reference (C++03 → C++23)

A compact, practical reminder of core C++ concepts and the most useful features across language versions, with minimal, runnable snippets that emphasize performance, clarity, and maintainability.

---

## 0) Foundational Ideas to Keep in Mind

### Move semantics — why care (and when not to)

**What:** Move semantics lets you transfer ownership of resources instead of copying them.

* **Care when:** objects manage heavy resources (heap buffers, files, sockets), you pass/return large containers, or you build value types that are frequently shuffled in containers.
* **Care less when:** trivial/cheap-to-copy types (small PODs), or hot paths where you’ve already avoided allocations.
* **Gotchas:** enable fast moves by making your type *movable* and marking heavy operations `noexcept` so containers prefer moving.

```cpp
// C++11+ minimal movable type
struct Buffer {
    size_t n{}; char* p{};
    Buffer(size_t n): n(n), p(new char[n]) {}
    ~Buffer(){ delete[] p; }
    // non-copyable
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    // movable
    Buffer(Buffer&& other) noexcept : n(other.n), p(other.p) { other.n=0; other.p=nullptr; }
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) { delete[] p; n=other.n; p=other.p; other.n=0; other.p=nullptr; }
        return *this;
    }
};
```

### RAII (Resource Acquisition Is Initialization)

**What:** Bind a resource’s lifetime to an object so that construction acquires and destruction releases — *no leaks, even on exceptions*.

```cpp
#include <mutex>

std::mutex m;
void f() {
    std::lock_guard<std::mutex> guard(m); // acquires in ctor, releases in dtor
    // ... critical section ...
} // guard destroyed ⇒ mutex unlocked, even on exceptions
```

**Use RAII for:** file handles, sockets, transactions, memory, locks, timers, `std::unique_ptr`, `std::shared_ptr`, custom guards.

### Virtual destructors, vtables, and a common misconception

* **You *can* have virtual destructors in C++.** In fact, if a class is used polymorphically and may be deleted via a base pointer, its **destructor should be virtual**.
* **Overhead:** a virtual destructor implies a vtable pointer (as with any virtual function). The indirection cost is tiny and paid only when calling virtual functions/destructors.

```cpp
struct Base { virtual ~Base() = default; }; // correct for polymorphic bases
struct Derived : Base { ~Derived() override { /* cleanup */ } };

Base* p = new Derived;
delete p; // calls Derived::~Derived() via vtable — safe
```

* **What you cannot have:** *virtual constructors*. Construction can’t be virtual because the object’s dynamic type isn’t established until construction finishes.

**Vtable refresher:** A *vtable* is a per-class table of function pointers for virtual calls. Objects of classes with virtual functions typically store a hidden pointer to the vtable. Virtual calls (including to a virtual destructor) use this table at runtime.

### Zero-cost abstractions

“Abstraction without (runtime) penalty when not used.”

* **Examples:** iterators, range-for, templates, `std::array`, `std::span`, inlined small lambdas, `constexpr` functions — all compile down to simple code.
* **Not a blanket promise:** exceptions are *zero cost on the non-throw path*, but throws can be expensive; type erasure (e.g., `std::function`) can allocate.

### Other evergreen practices

* **Value semantics first;** add indirection only where needed.
* **Const-correctness** and `noexcept` where appropriate.
* **Pimpl** and **type erasure** (`std::function`, custom vtables) for ABI stability and compile-time isolation.
* Prefer `=default`, `=delete`, and explicit conversions.

---

## C++03 (The Classic Base)

### RAII with smart pointers (pre-standard `auto_ptr` caution)

```cpp
// Pre-C++11, use Boost smart_ptrs (or today: prefer C++11+ unique_ptr/shared_ptr)
```

### SFINAE & templates (foundations for later metaprogramming)

```cpp
// Simple enable-if style (using Boost in 03; shown here conceptually)
```

### Idioms

* **Pimpl**, **Non-copyable base** (pre-`=delete`), **Rule of Three** (dtor, copy-ctor, copy-assign).

```cpp
struct X {
    X(); ~X();
    X(const X&);            // copy-ctor
    X& operator=(const X&); // copy-assign
};
```

---

## C++11 (The Big Modernization)

**Headliners:** move semantics, `auto`, range-for, lambdas, smart pointers, `constexpr`, variadic templates, `thread`, `chrono`, `atomic`, `override`, `=default`/`=delete`.

```cpp
#include <memory>
#include <vector>
#include <thread>
#include <chrono>

// auto + range-for + move-aware vector
std::vector<std::unique_ptr<int>> make_vec() {
    std::vector<std::unique_ptr<int>> v;
    for (int i=0;i<3;++i) v.push_back(std::unique_ptr<int>(new int(i)));
    return v; // moves, not copies
}

// Lambdas
int sum = 0;
[&](int a, int b){ sum = a + b; }(2, 3);

// constexpr functions
constexpr int sq(int x){ return x*x; }
static_assert(sq(4) == 16, "");

// override / final, deleted/ defaulted
struct Base { virtual void f(){} };
struct D : Base {
    void f() override {}
    D() = default;
    D(const D&) = delete;
};

// Threads
void worker(){ /* ... */ }
int main(){ std::thread t(worker); t.join(); }
```

---

## C++14 (Polish & Convenience)

**Headliners:** generic lambdas, return type deduction, `std::make_unique`, variable templates, relaxed `constexpr`, digit separators.

```cpp
#include <memory>
#include <utility>

// make_unique
auto p = std::make_unique<int>(42);

// generic lambda (auto parameters)
auto apply = [](auto f, auto x){ return f(x); };

// return type deduction for normal functions
auto twice(int x){ return 2*x; }

// std::exchange utility
template<class T>
T replace(T& target, T value){ return std::exchange(target, value); }
```

---

## C++17 (Quality-of-Life + Libraries)

**Headliners:** structured bindings, `if`/`switch` with init, `constexpr if`, `std::optional/variant/any`, `string_view`, filesystem, fold expressions, inline variables, `std::byte`, parallel algorithms.

```cpp
#include <optional>
#include <variant>
#include <string_view>
#include <filesystem>
#include <map>

// structured bindings
std::map<int, const char*> m{{1, "one"}, {2, "two"}};
for (auto [k, v] : m) { /* use k, v */ }

// if with initializer
if (auto it = m.find(2); it != m.end()) { /* ... */ }

// constexpr if (compile-time branches)
template<class T>
auto zero_of() {
    if constexpr (std::is_pointer_v<T>) return nullptr;
    else return T{};
}

// optional
std::optional<int> parse(/*...*/);
int value = parse().value_or(-1);

// string_view (no allocation)
void takes_view(std::string_view sv);

// filesystem
for (auto& p : std::filesystem::directory_iterator(".")) {/* ... */}
```

---

## C++20 (Concepts, Ranges, Coroutines, Modules)

**Headliners:** concepts & requires-expressions, ranges, coroutines, modules, `consteval`/`constinit`, `<=>` spaceship, `std::span`, `std::source_location`, calendar/time zones, barriers/latches/semaphores, `std::format` (C++20, implemented widely around 20/23 timeframe).

```cpp
#include <concepts>
#include <ranges>
#include <span>
#include <compare>
#include <format>

// Concepts
template<std::integral I>
I add(I a, I b){ return a + b; }

// Ranges pipeline (lazy, composable)
#include <vector>
#include <iostream>

std::vector<int> v{1,2,3,4,5};
for (int x : v | std::views::filter([](int x){return x%2;})
               | std::views::transform([](int x){return x*x;}))
    std::cout << x << ' ';

// span (view over contiguous data)
void use(std::span<const int> s){ /* no copy */ }

// three-way comparison
struct P { int x,y; auto operator<=>(const P&) const = default; };

// consteval: must evaluate at compile time
consteval int ten(){ return 10; }
static_assert(ten()==10);

// formatted output
std::string s = std::format("Hello {}", 42);
```

**Coroutines (sketch):**

```cpp
// Minimal coroutine outline (requires a coroutine-ready type)
#include <coroutine>
struct Task {
    struct promise_type {
        Task get_return_object(){ return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception(){ std::terminate(); }
    };
};

Task example() { co_return; }
```

**Modules (sketch):**

```cpp
// math.ixx
export module math;
export int add(int a, int b){ return a+b; }

// main.cpp
import math;
int main(){ return add(2,3); }
```

---

## C++23 (Ergonomics & Gaps Filled)

**Headliners:** `std::expected`, `std::print`, `std::mdspan`, `std::stacktrace`, `std::move_only_function`, more ranges (e.g., `zip_view`, `chunk_by`, `stride`, `views::to`), `std::byteswap`, `std::flat_set/map` *not standardized in C++23* (avoid claiming), library fixes and constexpr-ization.

```cpp
#include <expected>
#include <print>
#include <stacktrace>
#include <utility>

// expected for error handling
std::expected<int, std::string> parse_int(std::string_view s) {
    try { return std::stoi(std::string(s)); }
    catch(...) { return std::unexpected("bad int"); }
}

int main(){
    if (auto e = parse_int("42")) {
        std::print("value = {}\n", *e);
    } else {
        std::print("error: {}\n", e.error());
    }

    // stacktrace
    std::print("{}\n", std::stacktrace::current());
}
```

**`mdspan` (multi-dimensional view) sketch:**

```cpp
#include <mdspan>
#include <vector>

std::vector<double> buf(3*4);
std::mdspan<double, std::dextents<size_t,2>> M(buf.data(), 3, 4);
M(1,2) = 3.14; // no allocation; shape/view only
```

**`move_only_function`:**

```cpp
#include <functional>

std::move_only_function<int(int)> f = [](int x){ return x+1; };
int y = f(41);
```

**Ranges goodies (examples may need your stdlib’s 23 support):**

```cpp
#include <ranges>
#include <vector>
#include <utility>

std::vector<int> a{1,2,3}, b{4,5,6};
for (auto [x,y] : std::views::zip(a,b)) { /* ... */ }

auto evens = a | std::views::filter([](int x){return x%2==0;});
auto vec   = std::ranges::to<std::vector>(evens); // materialize
```

---

## Performance & Maintainability Checklists

* **Prefer value types** with efficient moves; keep them small and `noexcept`-move where possible.
* **RAII everywhere** for exception safety and early returns.
* **Minimize allocations:** use `reserve`, `small_vector`/SBO techniques, `string_view`, `span`.
* **Use `constexpr`** and `consteval` to precompute.
* **Use `string_view`/`span`** instead of copying.
* **Use `optional`/`expected`** for explicit absence/errors; avoid magic sentinel values.
* **Use concepts** to produce clear error messages and constrain templates.
* **Rely on zero-cost abstractions** (ranges pipelines, iterators) and profile where needed.
* **Type erasure** (`std::function`, `move_only_function`) only when polymorphism is truly needed.
* **Filesystem & chrono** for correctness instead of hand-rolled code.

---

## Rule-of-Three/Five/Zero Quick Table

* **Rule of Three (C++03):** if you define any of destructor/copy-ctor/copy-assign, you likely need all three.
* **Rule of Five (C++11+):** add move-ctor and move-assign.
* **Rule of Zero:** design types so the compiler-generated special members are correct; hold resources via RAII members (smart pointers, containers), not raw owners.

```cpp
// Rule of Zero example
#include <string>
#include <vector>
struct User { std::string name; std::vector<int> scores; }; // all special members are fine
```

---

## Tiny Patterns That Age Well

* **Scope guards** (write your own or use GSL):

```cpp
struct Scope {
    std::function<void()> f; ~Scope(){ if (f) f(); }
};

void g(){ Scope on_exit{[](){ /* cleanup */ }}; }
```

* **Type erasure for plugins:**

```cpp
struct AnyDrawer {
    struct concept {
        virtual ~concept()=default; virtual void draw()=0;
    };
    template<class T> struct model : concept { T t; model(T t):t(std::move(t)){} void draw() override { t.draw(); } };

    std::unique_ptr<concept> self;
    template<class T> AnyDrawer(T t): self(std::make_unique<model<T>>(std::move(t))) {}
    void draw(){ self->draw(); }
};
```

* **Pimpl to stabilize headers:**

```cpp
// widget.h
#include <memory>
class widget {
    struct impl; std::unique_ptr<impl> p;
public:
    widget(); ~widget(); widget(widget&&) noexcept; widget& operator=(widget&&) noexcept;
    void do_stuff();
};
```

---

## When to worry about the vtable cost?

* Only if you have super-hot inner loops of tiny virtual calls. Prefer **devirtualization** (final classes, LTO) or **static polymorphism** (CRTP/templates). Otherwise, the clarity and correctness of virtual dispatch often outweighs micro-cost.

```cpp
// CRTP example for static polymorphism
template<class D>
struct Shape { void draw(){ static_cast<D*>(this)->draw_impl(); } };
struct Circle : Shape<Circle> { void draw_impl(){ /* ... */ } };
```

---

## Exceptions vs. Alternatives

* Exceptions remain idiomatic; on the non-throwing path they’re “zero cost.” If throws are common or you want explicit flow, use `std::expected` (C++23) or status codes.

```cpp
std::expected<int, std::error_code> read_int(/*...*/);
```

---

## Final Tips

* Prefer library facilities over hand-rolled code.
* Embrace modern features incrementally; keep interfaces minimal and clear.
* Benchmark with real data; confirm assumptions with profiles.

---

## Additional Focus Topics (as requested)

### Lambda expressions & capture

* **Syntax:** `[captures](params) mutable? noexcept? -> ret { body }`
* **Captures:** `[]` none, `[=]` by copy, `[&]` by reference, `[x]` copy x, `[&x]` ref x, **init-capture** `[p = std::move(ptr)]`, capture `this` (C++11) / `[*this]` (C++17) / default with `this` (C++20: `[=, this]`).

```cpp
int base = 10; 
auto sum = [base](int x){ return base + x; };           // copy capture
int acc = 0; 
auto add = [&acc](int x){ acc += x; };                  // ref capture
auto p = std::make_unique<int>(7);
auto take = [q = std::move(p)](){ return *q; };         // init-capture (move)
```

### `nullptr`

Strongly-typed null pointer literal; avoids overload ambiguity of `0`/`NULL`.

```cpp
void f(int); void f(const char*);
f(0);        // calls f(int)
f(nullptr);  // calls f(const char*)
```

### `constexpr`

Evaluate at compile time when possible; usable in contexts requiring constants.

```cpp
constexpr int fib(int n){ return n<=1? n : fib(n-1)+fib(n-2); }
static_assert(fib(10) == 55);
constexpr int N = 64; std::array<int,N> a{};           // size known at compile time
```

### `noexcept`

Marks functions that do not throw; improves optimizations and enables move in containers.

```cpp
template<class T>
void swap_noexcept(T& a, T& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b); // conditional noexcept based on T::swap
}
```

### Smart pointers (`std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`)

Ownership and lifetime helpers; prefer `make_unique`/`make_shared` and RAII.

```cpp
struct F { void operator()(FILE* f) const { if(f) std::fclose(f); } };
using file_ptr = std::unique_ptr<FILE, F>;

auto up = std::make_unique<int>(42);        // sole owner
auto sp = std::make_shared<std::vector<int>>(100); // shared owner
std::weak_ptr<std::vector<int>> wp = sp;    // does not extend lifetime
if (auto s = wp.lock()) { /* use *s safely */ }
file_ptr fp(std::fopen("data.txt","r"));  // custom deleter
```

### `std::array` vs `std::vector`

* `std::array<T,N>`: fixed size, stack-allocatable, no heap; size is part of type; trivially contiguous.
* `std::vector<T>`: dynamic size, heap storage, amortized growth, RAII-managed capacity.

```cpp
std::array<int,4> a{1,2,3,4};              // fixed, zero heap
std::vector<int> v; v.reserve(4); v.assign(a.begin(), a.end());
std::span<const int> view = a;              // view both equally
```

### Return type deduction (`auto`)

C++14: functions can deduce return type; C++11 needs trailing return.

```cpp
auto twice(int x) { return 2*x; }           // C++14+
// C++11: auto f(int x) -> int { return x; }
```

### `decltype(auto)`

Deduce *exact* type and value category of the returned expression (preserve references).

```cpp
template<class T>
decltype(auto) get(T&& t){ return std::forward<T>(t).front(); } // returns T::reference
```

### Inline variables (C++17)

A single ODR-friendly definition in headers (like inline functions).

```cpp
// header.h
struct Config { int x = 1; };
inline Config GLOBAL;   // one definition across TUs
```

### Nested namespaces (C++17)

```cpp
namespace net::proto::v1 { struct Msg{}; }
```

### `[[likely]]` / `[[unlikely]]` (C++20)

Branch prediction hints; do not force behavior.

```cpp
if ([[likely]] x > 0) { /* fast path */ } else [[unlikely]] { /* rare */ }
```

### Immediate functions (`consteval`, C++20)

Must be evaluated at compile time.

```cpp
consteval int id(int x){ return x; }
static_assert(id(3) == 3);
```

### Structured binding (C++17)

Decompose tuples/structs/pairs into names.

```cpp
std::pair<int,std::string> p{7, "seven"};
auto [n, s] = p; // copies by default; use & to bind by reference
```

### Direct-list-initialization of enums

Brace-init with underlying type checks (no narrowing). Works best with scoped enums.

```cpp
enum class Mode : int { A=0, B=1 };
Mode m{1}; // OK (within range); Mode m2{2}; // implementation-defined if out of range
```

### Attributes: `[[fallthrough]]`, `[[nodiscard]]`, `[[maybe_unused]]`

```cpp
[[nodiscard]] int compute();

switch (x) {
  case 1: action1(); [[fallthrough]];   // intentional fallthrough
  case 2: action2(); break;
}

void g() { [[maybe_unused]] int tmp = compute(); }     // silence unused warning
```

---

## C++11 Additional Features Overview (Extended)

### Move semantics (recap)

Transfers ownership instead of copying; supports efficient moves of temporaries.

```cpp
struct Movable {
    std::vector<int> data;
    Movable() = default;
    Movable(Movable&& other) noexcept : data(std::move(other.data)) {}
    Movable& operator=(Movable&& other) noexcept {
        data = std::move(other.data); return *this; }
};
```

### Special member functions for move semantics

If you define custom move ctor/assign, also consider copy versions; they work with compiler-generated ones if appropriate (Rule of Five).

```cpp
struct S {
    S() = default;
    S(const S&) = default;
    S(S&&) noexcept = default;
    S& operator=(const S&) = default;
    S& operator=(S&&) noexcept = default;
};
```

### Rvalue references

Allow binding to temporaries (`T&&`); used for moves and perfect forwarding.

```cpp
void f(int&& x){ std::cout << x; }
int main(){ f(10); }
```

### Initializer lists

Uniform initialization `{}` and `std::initializer_list` enable direct container init.

```cpp
#include <vector>
std::vector<int> v{1,2,3,4};
struct A { A(std::initializer_list<int> list){} };
A a{1,2,3};
```

### Static assertions

Compile-time checks with custom message.

```cpp
static_assert(sizeof(int) >= 4, "int too small");
```

### `auto` type deduction

Simplifies variable declarations.

```cpp
auto i = 42;            // int
auto p = new int(5);    // int*
for (auto& x : v) x++;   // deduce element type
```

### Lambda expressions (recap)

Defined inline, with capture list, parameters, and body.

```cpp
auto square = [](int x){ return x*x; };
```

### Type aliases

Alternative to typedef, clearer for templates.

```cpp
template<class T> using Vec = std::vector<T>;
Vec<int> v{1,2,3};
```

### `nullptr` (recap)

Typed null pointer literal.

```cpp
int* p = nullptr;
```

### `constexpr` (recap)

Compile-time evaluable functions and variables.

```cpp
constexpr int doubleVal(int x){ return 2*x; }
```

### Explicit virtual overrides

Make overrides explicit with `override` keyword to avoid mismatches.

```cpp
struct Base { virtual void f(); };
struct Derived : Base { void f() override; };
```

### `final` specifier

Prevent further overriding or inheritance.

```cpp
struct Base final {}; // no further derived classes
struct A { virtual void f() final; }; // no overrides
```

### Deleted functions

Explicitly disable unwanted operations.

```cpp
struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
```

### Range-based for loops

Iterate containers simply.

```cpp
for (auto& e : vec) std::cout << e << ' ';
```

### Converting constructors

Prevent implicit conversions by marking `explicit`.

```cpp
struct A { explicit A(int x){} };
A a1{5};   // OK
//A a2 = 5; // Error: explicit prevents implicit conversion
```

### Inline namespaces

Allow versioning inside namespaces.

```cpp
namespace api {
  inline namespace v2 { void f(); }
}
api::f(); // refers to v2::f by default
```

### Non-static data member initializers

Simplify constructors by giving members default values.

```cpp
struct Point {
  int x = 0;
  int y = 0;
};
```

### Right angle brackets

Template closing brackets no longer require space.

```cpp
std::vector<std::vector<int>> mat; // legal since C++11
```

### Trailing return types

Useful for templates and decltype-based returns.

```cpp
template<class T, class U>
auto add(T a, U b) -> decltype(a+b) { return a + b; }
```

### `noexcept` specifier (recap)

Indicates a function won’t throw exceptions.

```cpp
void g() noexcept { /* ... */ }
```

---

## C++14 Additional Features Overview

### Generic lambda expressions

Lambda parameters can use `auto` for generic types.

```cpp
auto add = [](auto a, auto b){ return a + b; };
int x = add(2,3);        // int
std::string s = add(std::string("Hi"), std::string("!")); // string
```

### Lambda capture initializers (init-capture)

Allow expressions and move captures inside lambdas.

```cpp
auto ptr = std::make_unique<int>(10);
auto lam = [p = std::move(ptr)](){ return *p; }; // captures by move
```

### Return type deduction (for normal functions)

No need for explicit return type when deducible from return statement.

```cpp
auto multiply(int a, int b){ return a * b; }  // deduced int
```

### `decltype(auto)` (recap)

Preserves exact type and value category, unlike plain `auto`.

```cpp
int i = 42;
auto f1(){ return i; }          // returns int (copy)
decltype(auto) f2(){ return (i); } // returns int& (reference preserved)
```

### Variable templates

Template variables parameterized by type.

```cpp
template<typename T> constexpr T pi = T(3.141592653589793);
double d = pi<double>; float f = pi<float>;
```

### `[[deprecated]]` attribute

Marks functions, classes, or variables as deprecated (compiler warning on use).

```cpp
[[deprecated("use newFoo() instead")]]
void oldFoo();

void test(){ oldFoo(); }  // warning: deprecated
```

---

## C++17 Additional Features Overview

### Fold expressions (variadic packs made simple)

```cpp
template<class... Ts>
auto sum(Ts... xs){ return (xs + ... + 0); }         // unary right fold

template<class... Ps>
bool all(Ps... ps){ return (true && ... && ps); }    // unary left fold

// binary fold with an operator and an initial
auto prod = (1 * ... * std::array{2,3,4}[0]); // example; typically (init op ... op pack)
```

### New rules for `auto` deduction from braced-init-list

* **C++17 change:** direct-list-init `auto x{1};` now deduces `int` (not `std::initializer_list<int>`).
* `auto x = {1};` still deduces `std::initializer_list<int>`.
* Multiple elements: `auto y{1,2};` and `auto y = {1,2};` deduce `std::initializer_list<int>` (homogeneous); mixed types are ill-formed.

```cpp
auto a{1};          // int  (C++17)
auto b = {1};       // std::initializer_list<int>
auto c{1,2};        // std::initializer_list<int>
// auto d{1,2.0};   // error: not homogeneous
```

### `constexpr` lambda

Lambdas are usable in constant expressions if their body is `constexpr`-friendly.

```cpp
constexpr auto sq = [](int x){ return x*x; };
static_assert(sq(5) == 25);
```

### Lambda capture `*this` by value

Copies the current object into the lambda (safe for async/lifetimes vs `this` pointer).

```cpp
struct Counter {
  int n{0};
  auto make_stepper() const {
    return [*this]() mutable { return ++n; }; // captures a copy of *this
  }
};
```

### Inline variables (recap)

One definition across TUs; perfect for header constants/singletons.

```cpp
// header
struct Config { int level = 1; };
inline Config global;
```

### Nested namespaces (recap)

Compact nested namespace declaration.

```cpp
namespace net::http::v1 { struct request{}; }
```

### Structured bindings (recap with refs)

Decompose and optionally bind by reference using `&`.

```cpp
std::tuple<int,std::string> t{7, "seven"};
auto [i, s] = t;                 // copies
auto& [ri, rs] = t;              // references elements
```

### `if constexpr`

Compile-time branching; prunes discarded branches for ill-formed code.

```cpp
template<class T>
auto size_of(const T& x){
  if constexpr (requires { x.size(); }) return x.size();
  else return sizeof(T);
}
```
