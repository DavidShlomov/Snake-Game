# 🐍 Snake Game in C

A simple Snake game built in C to practice low-level programming concepts like pointers, working with libraries, and fundamental logic. This project serves as a hands-on way to better understand how things work under the hood in a systems-level language.

---

## 🎮 Features

- Classic Snake gameplay
- Randomly generated food
- Boundary walls for game area
- Intro animation using a GIF split into frames
- Keyboard controls for gameplay:
  - Arrow keys: Move the snake
  - Enter: Start the game
  - Esc: Exit the game

---

## 🛠️ Built With

- **C** – for core game logic
- **Standard C Libraries** – for I/O and system-level interaction
- **Linux terminal** – to render the game interface
- **Makefile** – for building, running, and cleaning up the project

---

## 🧱 How to Run

> 🐧 This game is designed for **Linux-based systems**.

1. **Clone the repository:**
   ```bash
   git clone https://github.com/DavidShlomov/Snake-Game
   cd snake-game
   ```

2. **Build the executable:**
   ```bash
   make
   ```

3. **Run the game:**
   ```bash
   make run
   ```

4. **Clean the compiled files:**
   ```bash
   make clean
   ```

---

## 📁 Project Structure

```bash
snake-game/
├── src/             # C source files and .h file
├── start/           # GIF frames for intro animation
├── font/            #font of the game
├── Makefile         # Build and run commands
└── README.md        # This file
```

---

## 💡 Learning Goals

- Master pointer manipulation in C
- Work with terminal and system libraries
- Practice handling input and drawing in a terminal window
- Understand Makefiles for managing C projects

---
