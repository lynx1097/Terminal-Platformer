
> [!WARNING]
> This Game was originally implemented and compiled in Pure C++ (cmake/clang) and later was transcompiled to web assembly so it may not run perfectly as native in the browser version .

<div align="center">

<h1>💣 DEMOLITION RUN</h1>

<p><i>A grid-based terminal roguelite — written in C++, compiled to WebAssembly, and played in the browser.</i></p>

<br/>
<a href="https://lynx1097.github.io/Terminal-Platformer/">
  <img src="/screenshots/play.png" alt="▶  Play in Browser" height="320"/>
</a>

<br/><br/>

<img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white"/>
<img src="https://img.shields.io/badge/Build-Emscripten-E36002?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Runtime-WebAssembly-654FF0?style=for-the-badge&logo=webassembly&logoColor=white"/>
<img src="https://img.shields.io/badge/Terminal-xterm.js-2A9D3C?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Course-Programming%20III%20%7C%20GIU-B00000?style=for-the-badge"/>

</div>



---

## 📖 About

**Demolition Run** is a 15 × 15 grid roguelite rendered entirely in ANSI-escaped terminal art. Navigate the battlefield, collect ammo and potions, engage monsters in melee and ranged combat, and detonate weapon specials to clear the map — all inside a terminal (or browser tab).

The game was developed from scratch in **C++** for the *Programming III* course at the **German International University (GIU)**, then compiled to **WebAssembly** via **Emscripten** and wired to a browser terminal using **xterm.js** — no server, no plugins, no installation. Just a single HTML file and a `.wasm` binary.

---

## 🖼️ Screenshots

<div align="center">
<table>
  <tr>
    <td align="center">
      <!-- ▼ Replace src with actual screenshot ▼ -->
      <img src="/screenshots/screenshot-1.jpeg" height="210" alt="Start Screen"/><br/>
      <sub><b>Hero &amp; Weapon Selection</b></sub>
    </td>
    <td align="center">
      <!-- ▼ Replace src with actual screenshot ▼ -->
      <img src="/screenshots/screenshot-2.png" height="210" alt="Gameplay"/><br/>
      <sub><b>Grid Combat &amp; Exploration</b></sub>
    </td>
  </tr>
</table>
</div>

---

## 🎮 Game Overview

### Heroes

<table>
<tr>
  <th>Hero</th>
  <th>Symbol</th>
  <th>Special &nbsp;<kbd>X</kbd></th>
</tr>
<tr>
  <td><b>Demolition Hero</b></td>
  <td align="center"><code>@</code> (cyan)</td>
  <td>Carries <b>two weapons simultaneously</b> and hot-swaps between them</td>
</tr>
<tr>
  <td><b>Medic Hero</b></td>
  <td align="center"><code>@</code> (green)</td>
  <td>Single-use self-heal restoring <b>+50 HP</b></td>
</tr>
</table>

### Weapons

<table>
<tr>
  <th>Weapon</th>
  <th>Bullets</th>
  <th>Damage</th>
  <th>Range</th>
  <th>Special &nbsp;<kbd>G</kbd></th>
</tr>
<tr>
  <td><b>Pistol</b></td>
  <td align="center">16</td>
  <td align="center">30</td>
  <td align="center">2</td>
  <td>—</td>
</tr>
<tr>
  <td><b>Bazooka</b></td>
  <td align="center">4</td>
  <td align="center">150</td>
  <td align="center">8</td>
  <td>Diagonal blast — sweeps all cells in an <b>X pattern</b> across the full grid</td>
</tr>
<tr>
  <td><b>Rifle</b></td>
  <td align="center">30</td>
  <td align="center">20</td>
  <td align="center">4</td>
  <td>—</td>
</tr>
</table>

### Monsters

<table>
<tr>
  <th>Monster</th>
  <th>Symbol</th>
  <th>Behaviour</th>
</tr>
<tr>
  <td><b>Tank Monster</b></td>
  <td align="center"><code>▲</code></td>
  <td>Armoured — all incoming damage is first absorbed by a <b>shield layer</b> before HP is reduced</td>
</tr>
<tr>
  <td><b>Ghost Monster</b></td>
  <td align="center"><code>◆</code></td>
  <td>Phases out every 3rd round — becomes <b>invisible on the map</b> but still occupies its cell</td>
</tr>
</table>

### Items

| Item | Symbol | Effect |
|------|:------:|--------|
| **Potion** | `♥` | Restores 10 – 50 HP on pickup |
| **Ammo** | `⊕` | Adds 5 – 20 bullets to the current weapon |

---

## 🕹️ Controls

<div align="center">

| Action | Input |
|--------|:-----:|
| Move | <kbd>W</kbd> <kbd>A</kbd> <kbd>S</kbd> <kbd>D</kbd> |
| Enter firing mode | <kbd>F</kbd> |
| Aim &amp; fire | <kbd>F</kbd> → <kbd>W</kbd> / <kbd>A</kbd> / <kbd>S</kbd> / <kbd>D</kbd> |
| Hero special ability | <kbd>X</kbd> |
| Gun special ability | <kbd>G</kbd> |
| Quit | <kbd>Q</kbd> |

</div>

> **Combat tip —** stepping into a monster's cell triggers melee. The hero deals weapon damage (reduced by any shield), while the monster retaliates for a flat 10 HP. Ranged fire costs one bullet and stops at the first monster hit in line.

---

## 🏗️ Object-Oriented Architecture

The entire game entity system is built on a single inheritance hierarchy rooted at `Object`. Every cell on the 15 × 15 grid stores an `Object*`, allowing heroes, monsters, items, and empty tiles to coexist in the same array and be queried polymorphically.

```
Object
├── Item
│   ├── Potion
│   └── Ammo
└── Character
    ├── Hero
    │   ├── DemolitionHero
    │   └── MedicHero
    └── Monster
        ├── TankMonster
        └── GhostMonster

Gun  (independent hierarchy)
├── Pistol
├── Bazooka
└── Rifle
```

The `Gun` hierarchy is kept deliberately separate from `Object` — guns are owned resources of heroes, not grid entities, so they live outside the map and are managed through raw pointer members on the `Hero` class.

---

## 🧠 C++ Skills Demonstrated

<details>
<summary><b>📦 &nbsp;Inheritance &amp; Polymorphism</b></summary>
<br/>

The project features a **multi-level inheritance chain** across two independent hierarchies (`Object` and `Gun`). Virtual methods are declared at the base and selectively overridden at each level, allowing the game loop to drive all entities through base-class pointers without ever knowing the concrete type.

```cpp
// Gun base — empty default body acts as a soft interface
virtual void useSpecial(Hero& h, Object* grid[15][15]) {}

// Bazooka override — full diagonal grid sweep
void useSpecial(Hero& h, Object* grid[15][15]) override {
    // sweeps northeast, northwest, southwest, southeast
}
```

`getInfo()` is declared virtual on `Hero` and overridden by both `DemolitionHero` and `MedicHero` to print hero-type-specific state — weapon slots, healing amount — through the same base pointer:

```cpp
friend ostream& operator<<(ostream& os, Hero& h) {
    return h.getInfo(os); // dispatches to the correct subclass at runtime
}
```

</details>

<details>
<summary><b>🔒 &nbsp;Encapsulation</b></summary>
<br/>

All internal state is kept `private` or `protected`, exposed only through narrow getter/setter interfaces. No external code ever touches a field directly:

```cpp
class Gun {
private:
    string name;
    int bullets, damage, range;
public:
    int  getBullets()  const { return bullets; }
    void setBullets(int b)   { bullets = b;    }
    int  getDamage()   const { return damage;  }
    int  getRange()    const { return range;   }
};
```

`TankMonster` encapsulates its shield value behind `getShield()` / `setShield()`, so the combat logic in `Hero::operator-` can reduce the shield without ever accessing the raw field:

```cpp
int absorbed = min(dmg, tank.getShield());
tank.setShield(tank.getShield() - absorbed);
dmg -= absorbed;
```

</details>

<details>
<summary><b>🔧 &nbsp;Constructors &amp; Member Initializer Lists</b></summary>
<br/>

Every class initializes its members through **constructor chaining via initializer lists**, delegating up the hierarchy cleanly rather than assigning in the body:

```cpp
// Ammo → Item → Object: full chain via initializer lists
Ammo(int b) : Item('A'), bullets(b) {}

// GhostMonster → Monster → Character → Object
GhostMonster(char t, int x, int y, int hp)
    : Monster(t, x, y, hp), isVisible(true) {}

// DemolitionHero → Hero → Character → Object
DemolitionHero(char t, int x, int y, int hp)
    : Hero(t, x, y, hp), weapon2(nullptr) {}
```

This guarantees that base sub-objects are fully constructed before the derived constructor body runs — a critical correctness property when base classes hold virtual dispatch tables.

</details>

<details>
<summary><b>💥 &nbsp;Destructors &amp; Resource Cleanup</b></summary>
<br/>

Every class that owns heap-allocated resources defines an explicit **virtual destructor** to ensure the correct cleanup chain fires when an object is deleted through a base pointer.

`Hero` frees both weapon slots, with a guard to prevent double-deletion when `current` and `weapon` point to the same object:

```cpp
virtual ~Hero() {
    if (weapon != nullptr) delete weapon;
    if (current != nullptr && current != weapon) delete current;
}
```

`DemolitionHero` extends this to also release the second weapon slot it exclusively owns:

```cpp
~DemolitionHero() {
    if (weapon2 != nullptr) delete weapon2;
}
```

`Game`'s destructor performs the most delicate teardown. Because the hero pointer lives **both** as a standalone member and as a raw pointer inside the grid, its cell is explicitly nulled before the grid sweep to prevent a double-free:

```cpp
~Game() {
    if (hero != nullptr)
        grid[hero->getPositionX()][hero->getPositionY()] = nullptr;
    delete hero;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            delete grid[i][j];  // safe — hero's cell is already nullptr
}
```

</details>

<details>
<summary><b>🖊️ &nbsp;Raw Pointers &amp; Dynamic Memory</b></summary>
<br/>

The entire map is a raw `Object*` 2D array — no wrappers, no containers:

```cpp
Object* grid[15][15];
```

Every entity on the map is heap-allocated. When the hero moves, an item is picked up, or a monster dies, the old object is **explicitly freed and replaced**:

```cpp
delete grid[rx][ry];
grid[rx][ry] = new TankMonster('T', 0, 0, hp, hp / 2);
```

Runtime type identification uses the lightweight `getType()` character tag, followed by a C-style downcast to the concrete type — a direct, zero-overhead approach appropriate for a performance-critical game loop:

```cpp
if (grid[x][y]->getType() == 'T') {
    TankMonster& tank = (TankMonster&)(*grid[x][y]);
    tank.setShield(tank.getShield() - absorbed);
}
```

Pointer ownership is explicit and enforced by convention: the `Game` object owns the grid, `Hero` owns its guns, and `DemolitionHero` owns the additional weapon slot. There is no shared ownership — each pointer has exactly one owner responsible for its deletion.

</details>

<details>
<summary><b>👥 &nbsp;Friend Functions &amp; Operator Overloading</b></summary>
<br/>

Non-member `operator<<` overloads are declared `friend` to read private fields and produce formatted output without breaking encapsulation:

```cpp
// Prints: Name, Range, Bullets, Damage
friend ostream& operator<<(ostream& os, Gun& g);
```

The three gameplay interaction operators are also `friend` free functions, keeping the call-site syntax expressive and readable:

```cpp
*hero + potion;   // HP restored         → operator+(Hero&, const Potion&)
*hero + ammo;     // Bullets refilled    → operator+(Hero&, const Ammo&)
*hero - monster;  // Melee combat round  → operator-(Hero&, Monster&)
```

The subtraction operator encapsulates the full melee resolution — hero damage, shield absorption, and monster health reduction — in a single, intention-revealing expression.

</details>

---

## 🌐 Web Deployment Pipeline

Bringing a raw C++ terminal game to the browser without rewriting any game logic required three composable layers:

```
  C++ Source (.h / .cpp)
        │
        │  emcc (Emscripten compiler)
        ▼
  game.wasm  +  game.js  (Emscripten module loader)
        │
        │  xterm.js  (ANSI terminal emulator in <canvas>)
        │  ccall()   (JS → WASM keystroke bridge)
        ▼
  index.html  ─────▶  Browser  (zero install, zero server)
```

<br/>

**1 · Emscripten → WebAssembly**

Emscripten compiles the C++ source to a `.wasm` binary paired with a `game.js` bootstrapper. Crucially, Emscripten's in-browser **virtual filesystem** and **TTY layer** intercept `stdout`/`stderr` at the byte level — meaning every `cout` statement in C++ routes directly to the browser terminal, character by character, with no code changes.

**2 · xterm.js — ANSI Terminal in the Browser**

The game's TUI is built purely with **ANSI escape sequences**: 24-bit RGB color, cursor positioning, alternate screen mode, and Unicode box-drawing characters. `xterm.js` is the only browser terminal emulator that faithfully renders all of these in a `<canvas>` element, producing output visually identical to a native terminal.

**3 · Input Bridge — `ccall()`**

A custom C function `sendInputToGame` is exported from the WASM module and exposed to JavaScript. `xterm.js` captures every keypress and forwards it over Emscripten's `ccall()` bridge, giving the game single-character raw input — exactly what the native `getch()` call would deliver:

```js
term.onData(function(data) {
    Module.ccall('sendInputToGame', 'null', ['number'], [data.charCodeAt(0)]);
});
```

**4 · UTF-8 Safe Output**

The raw byte stream from C++ is decoded with a **streaming `TextDecoder`** before being written to the terminal. Using `{ stream: true }` ensures that multi-byte UTF-8 sequences spanning multiple `put_char` calls — such as the Unicode box-drawing and symbol characters used throughout the UI — are decoded correctly and never corrupted:

```js
const utf8Decoder = new TextDecoder('utf-8');

// Inside Emscripten's stdout hook:
term.write(utf8Decoder.decode(new Uint8Array([charData]), { stream: true }));
```

---

## 📚 Course Context

> **Programming III (C++)** — German International University (GIU)  
> Final course project demonstrating applied object-oriented design, manual memory management, and polymorphic program architecture in C++.

---

<div align="center">
  <sub>Built with terminal art, raw pointers, and a healthy fear of memory leaks.</sub>
</div>