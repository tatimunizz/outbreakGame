## Outbreak Game

A classic brick‑breaker clone written in **C** with the **raylib** multimedia library.  
Break all the bricks, keep the ball alive, and score as many points as possible!

## A note from me :D

I developed this game to practice C programming while taking CISC1001 – Programming Science at the University of Macau. It wasn't mandatory; I took the initiative because I'm a fan of game development, I had known about Raylib for a while, and I was wanting to play with it.

I chose to make a copy of the Breakout game because, first, I think it's fun, and second, the game was simple enough for me to focus on implementing what I was learning in class while also learning Raylib on my own.

I first drew a sketch of the game in a notebook and figured out everything I needed before coding. Then, I decided to write it in a single file because it was small and simple enough to understand the progression of the code.

I had prior programming experience (more specifically, I used to program games in Java), so the real challenge was understanding the C language syntax and its possibilities.

### 🎮 Gameplay

|  |  |
|---|---|
| **Goal**  | Destroy every brick on the screen by bouncing a ball off your paddle.  |
| **Lives**  | You start with 3 lives (displayed as red bars). The ball is lost if it falls below the screen.  |
| **Points**  | Each brick gives 3 points. The current score is shown in the top status bar.  |
| **Game Over**  | Lose all lives → defeat screen. Clear all bricks → victory screen.  |
| **Pause**  | Press **P** at any time to pause/resume the game.  |
| **Restart**  | After a win or loss, press **ENTER** to play again.  |

**Controls**

| Key | Action |
|---|---|
| `←` `→` | Move paddle left / right |
| `SPACE` | Launch the ball (first time) |
| `P` | Toggle pause |
| `ENTER` | Restart after game ends |

**Screens**

- **GAMEPLAY** – Main playing field with bricks, paddle, ball, and status bar.
- **ENDGAME** – Displays either victory or defeat, with a blinking prompt to press ENTER.

---

### 🛠️ Technologies

| Technology | Purpose |
|---|---|
| **C (C99)** | Core game logic |
| **raylib** (included as a sub‑directory) | Graphics, input, window management |
| **CMake** (≥ 3.0) | Build system for desktop & web |

No external assets (images, sounds) are used – everything is drawn with raylib’s shape‑drawing functions.

---

### 📁 Project Structure

```
outbreakGame/
├── src/
│   └── main.c             # Entire game code (~112 lines)
```

---

### 🔨 Building

#### Desktop (Windows, Linux, macOS)

1. **Clone the repository**  
   ```bash
   git clone https://github.com/tatimunizz/outbreakGame.git
   cd outbreakGame
   ```

2. **Make sure raylib is available**  
   The project expects raylib source inside `libs/raylib`. If the directory is empty, initialise the submodule:
   ```bash
   git submodule update --init --recursive
   ```

3. **Generate build files with CMake**  
   ```bash
   mkdir build && cd build
   cmake ..
   ```

4. **Compile**  
   ```bash
   cmake --build .
   ```

5. **Run the executable**  
   The binary will be named `raylib_template` (or `raylib_template.exe` on Windows).

---

### ⚙️ Configuration

All game parameters are defined at the top of `src/main.c`:

| Constant | Value | Description |
|---|---|---|
| `SCREEN_WIDTH` | 800 | Window width |
| `SCREEN_HEIGHT` | 450 | Window height |
| `BEGIN_LIFE` | 3 | Initial number of lives |
| `BRICKS_COLUMNS` | 15 | Columns of bricks |
| `BRICKS_LINES` | 6 | Rows of bricks |
| `TOTAL_BRICKS` | 90 | Total bricks to destroy |
| `POINTS` | 3 | Points per brick |

You can tweak these values to change the game difficulty or layout.

---

### 📄 License

This project is distributed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

---

### 👤 Author

**Tatiana Muniz** – [tatimunizz](https://github.com/tatimunizz)
