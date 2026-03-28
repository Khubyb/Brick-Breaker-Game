# 🧱 Brick Breaker Game 🏗️

  A fully-featured brick breaker game implemented in C++ using the SFML (Simple and Fast Multimedia Library) graphics framework.  
This project demonstrates file I/O, collision detection, scoring system, and real-time graphics rendering.

![Language](https://img.shields.io/badge/Language-C++-blue) 
![Library](https://img.shields.io/badge/SFML-2.6.2-brightgreen)

# Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [Windows](#windows)
  - [Linux](#linux)
  - [macOS](#macos)
- [Compilation & Running](#compilation--running)
- [How to Play](#how-to-play)
- [Architecture Overview](#architecture-overview)
- [Module Documentation](#module-documentation)
- [Game Logic](#game-logic)
- [File Formats](#file-formats)
- [Customization](#customization)
- [Troubleshooting](#troubleshooting)
- [Future Enhancements](#future-enhancements)
- [Contributing](#contributing)

---

## Features
Core Game Mechanics

✅ Complete Brick Breaker Rules Implementation

Ball movement and collision detection
Paddle control (left/right movement)
Brick collision with destruction
Scoring system based on brick hits
Lives system (lose life when ball falls below paddle)
Level progression (optional increasing difficulty)
User Interface

🎨 Beautiful Graphical Interface

Custom paddle, ball, and brick sprites
Background textures for levels
Visual brick hit effects (e.g., breaking animation)
Smooth hover effects on menu buttons
Color-coded end game screens
Game Features

💾 Save/Load System

Save game state at any time (press 'S' or via pause menu)
Load previously saved games from main menu
Automatic game state persistence

📊 Statistics Tracking

Score counter
Lives counter
Persistent high scores across sessions
Statistics display in main menu

⚙️ Settings & Customization

Sound toggle (on/off)
Theme selection (default/alternate)
Persistent settings storage
Menu System

📱 Complete Menu Navigation

Main Menu (Start, Load, Settings, High Scores, Exit)
Pause Menu (Resume, Save, Exit)
End Game Screen (Play Again, Main Menu)
Settings Screen (Sound, Theme)
High Scores Display

## Project Structure
Brick Breaker_Game/

├── Source Files (.cpp)
│   ├── Main.cpp              # Main game loop and orchestration
│   ├── Bricks.cpp             # Stars Management
│   ├── Globals.cpp             # Globals Variables Management
│   ├── Paddle_ball.cpp         # ball Paddle Events
│   ├── Pause_ui.cpp            # Graphics rendering
│   ├── PF project.cpp              # Menu system
│   ├── Save_Load.cpp            # Save/load functionality
│   ├── Screens.cpp              # Screen
│   ├── Stars.cpp        # Stars tracking and making
│
├── Header Files (.h)
│   ├── Bricks.h              
│   ├── Config.h               
│   ├── Globals.h           
│   ├── Paddle_ball.h              
│   ├── Paddle_ui.h                
│   ├── Save_Load.h              
│   ├── Screens.h                
│   ├── Stars.h          
│
├── Assets
│   arial.ttf                      # Font
    bricks-fall-315300.wav
    Electronic-soft-jazz-background-music.wav
    ping-82822.wav
│
├── Data Files (Auto-generated)
│   ├── savegame.txt          # Saved game state
│   ├── highscores.dat        # Win statistics
│
├── README.md                 # This file


## Prerequisites
Required Software
C++ Compiler

Windows: MinGW-w64 (GCC) or MSVC
Linux: GCC or Clang
macOS: Clang (Xcode Command Line Tools)
SFML 2.6.2 (Simple and Fast Multimedia Library)

Download from: https://www.sfml-dev.org/download.php
Required modules: Graphics, Window, System, Audio
Build Tools

Make (optional, for makefile builds)
CMake (optional, for cross-platform builds)
System Requirements
OS: Windows 7+, Linux (any modern distro), macOS 10.12+
RAM: 256 MB minimum
Disk Space: 50 MB (including assets)
Display: 800x800 minimum resolution

## Installation
Step 1: Install SFML
Download SFML 2.6.2 for MinGW from https://www.sfml-dev.org/download/sfml/2.6.2/
Extract to C:/SFML-2.6.2/ (or any preferred location)
Add SFML bin directory to PATH:
C:/SFML-2.6.2/bin
Step 2: Clone/Download Project
git clone https://github.com/Khubyb/Brick-Breaker-Game.git
cd Brick Breaker-GAME-SFML-CPP
Step 3: Create Assets Directory
Ensure you all GAME FILES must inside the same folder.
### Windows
Step 1: Install SFML
Ubuntu/Debian:

sudo apt-get update
sudo apt-get install libsfml-dev
Fedora:

sudo dnf install SFML-devel
### Linux

Step 2: Install Build Tools
sudo apt-get install build-essential g++
Step 3: Clone Project
git clone https://github.com/Khubyb/Brick-Breaker-Game.git
cd sfml-Brick Breaker-game
### macOS
Step 1: Install Xcode Command Line Tools
xcode-select --install
Step 2: Install SFML via Homebrew
brew install sfml
Step 3: Clone Project
git clone https://github.com/Khubyb/Brick-Breaker-Game.git
cd sfml-Brick Breaker-game


## Compilation & Running
All Game Files Must inside a same Folder
## How to Play
Starting the Game

Launch the executable (brick_breaker or brick_breaker.exe)
Main Menu appears with the following options:

Start New Game – Begin a fresh Brick Breaker level
Load Game – Resume from saved state
Settings – Configure sound and theme
High Scores – View score statistics
Exit – Close the application
During Gameplay
Controlling the Paddle
Use Left / Right arrow keys to move the paddle
Keep the ball in play by bouncing it off the paddle
The ball will break bricks it collides with
Scoring
Each brick hit increases your score
When one bottom Bricks Row Eliminate then give bonus points or power-ups
Losing the ball costs one life
Game Controls
Key / Action	Function
Left / Right Arrow	Move paddle
Space	To Bounce Ball form the Paddle
Q	Quick save game
Resume – Continue playing
Save Game – Save current state to file
Exit to Main Menu – Abandon game and return to menu
Winning / Losing the Game

The game automatically detects:
✅ Level Complete – All bricks are destroyed
✅ Game Over – All lives are lost

When the game ends:

Score is recorded in statistics
End screen displays options to Play Again or Quit Game
## Architecture Overview
┌─────────────────┐
│  Program Start  │
└────────┬────────┘
         │
         ▼
┌─────────────────────────┐
│  Load Settings & Scores │
└────────┬────────────────┘
         │
         ▼
┌─────────────────┐
│   Main Menu     │◄──────────┐
└────────┬────────┘           │
         │                    │
         ▼                    │
┌─────────────────┐           │
│   Game Loop     │           │
│ (Event & Render)│           │
└────────┬────────┘           │
         │                    │
         ▼                    │
┌─────────────────┐           │
│ Paddle & Ball   │           │
│ Movement        │           │
└────────┬────────┘           │
         │                    │
         ▼                    │
┌─────────────────┐           │
│ Collision       │           │
│ Detection       │           │
│ (Ball & Bricks) │           │
└────────┬────────┘           │
         │                    │
         ▼                    │
┌─────────────────┐           │
│ Score / Lives   │           │
│ Update          │           │
└────────┬────────┘           │
         │                    │
         ▼                    │
┌─────────────────┐           │
│ Check Level End │───────────┘
│ or Game Over    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ End Game Screen │
└─────────────────┘


## Game Logic
Game Logic – Brick Breaker
1. Core Mechanics

The core logic of Brick Breaker revolves around player interaction, ball movement, and brick destruction. The main components are:

Paddle – Controlled by the player, moves horizontally along the bottom of the screen.
Ball – Moves freely in two dimensions and bounces off walls, paddle, and bricks.
Bricks – Arranged in rows; each has a certain number of hits before it breaks.

The game loop continuously updates the position of the ball and paddle, detects collisions, updates scores, and checks for win/loss conditions.

2. Ball Movement Logic

The ball moves with a velocity vector (dx, dy) that determines the direction and speed.

Updating Position: Each frame, the ball’s position is updated:
ball.x += dx
ball.y += dy
Wall Collision:
If the ball hits the left or right walls, dx = -dx (horizontal direction reverses).
If the ball hits the top wall, dy = -dy (vertical direction reverses).
Bottom Wall (Lose Life): If the ball goes below the paddle, the player loses a life and the ball resets above the paddle.
Angle Variation on Paddle Hit:
The angle of reflection depends on where the ball hits the paddle.
Hitting the paddle center → small vertical change (dy).
Hitting near edges → larger horizontal change (dx), making gameplay dynamic.
3. Paddle Control Logic

The paddle is controlled by the player via keyboard (left/right arrows) or mouse movement:

Keyboard Input: Moves paddle left or right within screen bounds.
Mouse Input: Paddle’s x-position follows the mouse’s x-coordinate.
Collision Check: When the ball hits the paddle, the vertical direction (dy) is reversed, and horizontal direction (dx) may adjust based on hit location.
4. Brick Logic

Bricks are stored in a 2D array or a list, each with attributes:

x, y → position
width, height → size
hitsRequired → number of hits to destroy (some bricks may take multiple hits)
scoreValue → points awarded for hitting/breaking

Collision Detection with Ball:

Check if the ball intersects a brick’s rectangle.
If collision occurs:
Reduce hitsRequired by 1.
If hitsRequired reaches 0, remove brick and add scoreValue to the player’s score.
Reverse the ball’s vertical direction (dy = -dy) for realistic bounce.
5. Power-Up Logic (Optional Advanced Feature)

Some bricks may drop power-ups when destroyed:

Power-ups fall vertically and are collected by the paddle.
Each power-up has a type, such as:
Expand Paddle – Makes paddle longer.
Multi-Ball – Spawns additional balls in play.
Speed-Up / Slow-Down – Increases or decreases ball speed.
Extra Life – Adds one life to the player.

Power-Up Collision:

Detect if paddle intersects the falling power-up.
Activate the effect immediately and remove the power-up from the screen.
6. Scoring System

Score calculation is straightforward:

Each brick has a scoreValue.
When destroyed, add scoreValue to the player’s score.
Bonus points may be added for collecting power-ups or completing levels faster.

Example:

score += brick.scoreValue
if powerUpCollected: score += bonusPoints
7. Level Progression

Levels increase in difficulty as the player progresses:

More bricks per row or more rows.
Stronger bricks requiring multiple hits.
Moving bricks for higher challenge.
Faster ball speed or reduced paddle size.

Next Level Trigger:

When all bricks in the current level are destroyed, automatically load the next level.
Optionally reset the ball above the paddle and increase speed slightly.
8. Lives and Game Over Logic
Players start with a fixed number of lives, e.g., 3.
Losing a life occurs when the ball falls below the paddle.
Game Over Condition: All lives are lost.
Display End Game Screen with final score and options: Play Again or Exit.
9. Collision Optimization

Efficient collision detection is important for smooth gameplay:

Only check collisions with bricks near the ball (reduce unnecessary checks).
Use bounding boxes (AABB) for ball and bricks for fast collision detection.
For multiple balls, iterate through each ball separately.
10. Game Loop Overview

The game loop follows a consistent structure each frame:

Input Handling – Read keyboard/mouse events to move the paddle.
Update Ball – Move ball and check collisions with walls, paddle, and bricks.
Update Power-Ups – Move falling power-ups and check collisions with paddle.
Update Score and Lives – Increment score for destroyed bricks, decrement lives if ball lost.
Render Graphics – Draw paddle, balls, bricks, background, score, and lives.
Check Win/Loss – If all bricks destroyed → next level, if lives = 0 → game over.
11. Optional Enhancements
Trajectory Prediction – Show ball path to assist players.
Dynamic Difficulty Adjustment – Ball speed increases as levels progress.
Particle Effects & Animations – Visual effects when bricks break or power-ups activate.
Save/Load Game State – Persist current level, score, and ball/paddle positions.
## Future Enhancements
Planned Features – Brick Breaker
💥 Power-Ups / Special Balls – Multi-ball, sticky paddle, speed-up, or slow-down effects
🧱 Brick Types – Different bricks: normal, strong (multiple hits), unbreakable, moving bricks
⏱ Timer / Time Attack Mode – Complete levels within a time limit
🎮 AI Paddle Opponent (Optional) – Competitive mode with automated paddle
🕹 Level Editor – Create and play custom brick layouts
🎵 Sound Effects – Ball hit, brick break, paddle bounce sounds
🎨 Themes / Skins – Multiple background, paddle, ball, and brick styles
📝 Score Export / Leaderboards – Save and display high scores
🌐 Online Multiplayer Mode (Optional) – Compete with other players
🔔 Hint System – Show ball trajectory or suggest power-up usage
🎥 Visual Effects / Animations – Particle effects when bricks break
## Contributing Ideas
Improve UI/UX design and animations for smoother gameplay
Add more power-ups and creative brick types
Implement advanced physics for realistic ball movement
Create a tutorial/help system for new players
Add accessibility features (colorblind mode, adjustable speed)
Optimize collision detection and game performance
Cross-platform testing and fixes (Windows, Linux, macOS)
