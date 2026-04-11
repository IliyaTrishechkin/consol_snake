# 🐍 Snake Game (C + ncurses)

A classic Snake game implemented in C using the **ncurses** library for terminal-based graphics.

---

## 📦 Dependencies

This project uses the **ncurses** library to create a text-based user interface (TUI).

**ncurses provides:**
- keyboard input handling (arrow keys, ESC, etc.)
- terminal screen control
- real-time UI updates
- interactive console applications

---

## 🐧 Installation (Linux)

Before building the project, install the ncurses development library.

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install libncurses5-dev libncursesw5-dev
```

---

## 🛠 Compilation

Compile the project using `gcc`:

```bash
gcc main.c snake_logic.c list_lib.c -lncurses -o snake
```

For better debugging:

```bash
gcc -Wall -Wextra main.c snake_logic.c list_lib.c -lncurses -o snake
```

---

## ▶️ Running

```bash
./snake
```

---

## 🎮 Controls

- ⬆️ Up Arrow — move up  
- ⬇️ Down Arrow — move down  
- ⬅️ Left Arrow — move left  
- ➡️ Right Arrow — move right  
- ⎋ Esc — exit the game  

---

## 📌 Notes

- Uses **ncurses** for rendering  
- Requires a terminal with TUI support  
- Recommended terminal size: **80x24** or larger  

---

## 🚀 Features

- Classic Snake gameplay  
- Terminal-based interface  
- Dynamic food spawning  
- Snake growth mechanics  
- Collision detection  

---

## 📂 Project Structure

```
.
├── main.c
├── snake_logic.c
├── snake_logic.h
├── list_lib.c
├── list_lib.h
└── README.md
```
