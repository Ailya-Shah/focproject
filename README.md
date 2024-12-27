
**PLEASE VISIT GAME-DIRECTORY FOLDER TO VIEW THE FILES INVOLVED IN DIRECT INTERACTION WITH GAME THE GAME CODE IS PRESENT IN WW.CPP**
---

# Murder Mystery Game

This is a C++ game built using SFML (Simple and Fast Multimedia Library). Players unscramble jumbled words, solve puzzles, and gather clues to identify the murderer in a thrilling murder mystery setting. The game supports two difficulty levels: **Intermediate** and **Hard**.

## Features
- **Word Unscrambling Puzzles:** Solve jumbled words related to a murder mystery.
- **Hint System:** Get character-based hints to help solve puzzles.
- **Scoring System:** Earn points based on your performance, with penalties for incorrect tries and excessive hints.
- **Interactive Graphics:** A visually immersive gameplay experience using SFML graphics.
- **Audio Effects:** Background music to enhance the atmosphere.
- **Clue Discovery:** Gather clues from suspects to solve the case.
- **Leaderboard:** Save and display scores for all players.

---

## Prerequisites
1. **SFML**: Download and install SFML 2.5.1 or later from [SFML's official website](https://www.sfml-dev.org/).
2. **C++ Compiler**: A compiler like GCC or MSVC that supports C++17 or later.
3. **Files Required**:
   - `Arial.ttf` (Font file)
   - `simon.png` (Background image)
   - `mysterious.wav` (Background audio)

---

## How to Build and Run
### Windows
1. Install SFML and set up the required libraries in your IDE (e.g., Visual Studio or Code::Blocks).
2. Compile the program:
   ```bash
   g++ ww.cpp -o ww -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
   ```
3. Run the game:
   ```bash
   ./ww
   ```

### Linux
1. Install SFML:
   ```bash
   sudo apt-get install libsfml-dev
   ```
2. Compile the program:
   ```bash
   g++ ww.cpp -o ww -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
   ```
3. Run the game:
   ```bash
   ./ww
   ```

---

## Gameplay Instructions
1. **Start the Game**:
   - Launch the game and choose your difficulty level: `intermediate` or `hard`.
   - Enter your username when prompted.

2. **Solve Puzzles**:
   - Unscramble jumbled words by typing your guesses.
   - Press `Enter` to submit your answer.
   - Press `H` to reveal a hint.

3. **Identify the Murderer**:
   - After solving puzzles, review clues from suspects.
   - Make your final guess about the identity of the murderer.

4. **View Scores**:
   - After completing the game, view your score and the leaderboard.

---

## File Descriptions
- **`murder_mystery.cpp`**: Main game code.
- **`scores.txt`**: Stores player scores persistently.
- **`Arial.ttf`**: Font file for in-game text rendering.
- **`simon.png`**: Background image for the game window.
- **`mysterious.wav`**: Background music for ambiance.

---

## Code Overview
### Key Components
1. **Puzzles**:
   - Jumbled words and clues are defined as part of the `Puzzle` struct.

2. **Suspects and Murderer**:
   - Suspects and the murderer have clues to aid in the investigation.

3. **Difficulty Levels**:
   - `intermediate` and `hard` modes offer different point configurations and puzzles.

4. **SFML Graphics**:
   - Game window, text rendering, and events are managed using SFML.

5. **Scoring**:
   - Points are updated based on performance (tries, hints, and correctness).

6. **Audio**:
   - Background music loops during gameplay for immersive experience.

---

## Controls
- **Keyboard**:
  - `Enter`: Submit your guess.
  - `H`: Get a hint for the current puzzle.
  - `Esc` / Close Window: Exit the game.

---

## Example Gameplay
1. Player chooses `intermediate` mode.
2. Jumbled word: **iomtve**
   - Clue: "I drive people to commit crimes. What am I?"
   - Correct Answer: **motive**
3. Player enters **motive**, earns points, and proceeds to the next puzzle.
4. After solving all puzzles, player reviews clues from suspects and identifies the murderer.

---

## Troubleshooting
- **Font Error**: Ensure `Arial.ttf` is in the same directory as the executable.
- **Image/Audio Loading Error**: Verify `simon.png` and `mysterious.wav` are correctly placed in the same directory as the executable.
- **SFML Not Found**: Ensure SFML is installed and linked properly in your project setup.

---

## Future Enhancements
- Add more puzzles and difficulty levels.
- Introduce timed challenges.
- Implement a graphical suspect selection interface.

---

## CREDITS
-Ailya Zainab 523506
-Iman Naeem 525378
-Laiba Nasir 510419
-Maha Mohsin 54258

Enjoy solving the case and catching the murderer! 🕵️‍♂️🔍
