# Space Invaders Clone

A modern C++23 implementation of the classic Space Invaders arcade game using SDL3.

<div align="center">
  <img src="assets/images/screenshot.png" alt="Space Invaders Screenshot" width="600">
  <p><i>Note: Screenshot will be updated once game is finalized.</i></p>
</div>

## ?? Overview

This project reimagines the iconic 1978 arcade game Space Invaders with modern C++ and SDL3. It maintains the classic gameplay while adding modern features and clean, well-structured code that can serve as a learning resource for game development.

## ? Features

- **Classic Gameplay**: Authentic Space Invaders experience with modern enhancements
- **Defensive Barriers**: Take cover behind destructible barriers that degrade as they're hit
- **Bonus UFOs**: Special UFO enemies periodically fly across the top of the screen for bonus points
- **Progressive Difficulty**: Increasing challenge with each level
- **Scoring System**: Score tracking with persistent high scores
- **Authentic Limitations**: True to the original, only 2 player bullets allowed on screen simultaneously
- **Modern Font Rendering**: Crisp text rendering when SDL_ttf is available

## ??? Controls

| Key | Action |
|-----|--------|
| Left Arrow / A | Move left |
| Right Arrow / D | Move right |
| Space | Fire bullet |
| R | Restart game (after Game Over) |

## ?? Building the Game

### Prerequisites

- **Compiler**: C++23 compatible compiler (GCC 11+, Clang 12+, MSVC 2022+)
- **Build System**: CMake 3.20 or higher
- **Libraries**: SDL3

### Optional Dependencies

- **SDL3_ttf**: For improved text rendering (see [FONT_SETUP.md](FONT_SETUP.md) for installation)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/CppSpaceInvaders.git
cd CppSpaceInvaders

# Create build directory and build
mkdir -p build && cd build
cmake ..
make

# Run the game
./bin/CppSpaceInvaders
```

### Quick Build (using provided script)

```bash
./r
```

## ?? Project Structure

```
CppSpaceInvaders/
ÃÄÄ include/                # Header files
³   ÃÄÄ Entity/             # Game entity headers
³   ³   ÃÄÄ Barrier.h       # Defensive barriers
³   ³   ÃÄÄ Bullet.h        # Projectiles
³   ³   ÃÄÄ Enemy.h         # Alien invaders
³   ³   ÃÄÄ Player.h        # Player ship
³   ³   ÀÄÄ UFO.h           # Bonus UFO
³   ÃÄÄ Game.h              # Game state management
³   ÃÄÄ Graphics.h          # Rendering utilities
³   ÃÄÄ TextRenderer.h      # Font and text rendering
³   ÀÄÄ Transform.h         # Transformation utilities
ÃÄÄ src/                    # Implementation files
³   ÃÄÄ Entity/             # Entity implementations
³   ÃÄÄ Game.cpp            # Game logic implementation
³   ÃÄÄ Graphics.cpp        # Rendering implementation
³   ÃÄÄ main.cpp            # Entry point
³   ÃÄÄ TextRenderer.cpp    # Text rendering implementation
³   ÀÄÄ Transform.cpp       # Transform implementation
ÃÄÄ assets/                 # Game resources
³   ÀÄÄ fonts/              # Font files for text rendering
ÃÄÄ CMakeLists.txt          # Build configuration
ÀÄÄ r                       # Build & run script
```

## ?? Implementation Details

### Core Game Loop

The game follows a standard game loop pattern:
1. **Input Processing**: Handle keyboard input for player movement and shooting
2. **Update Game State**: Update positions of all entities, handle collisions
3. **Render**: Draw the current game state to the screen

### Key Technical Features

- **Entity System**: Clean separation of game entities with consistent interfaces
- **Collision Detection**: Efficient collision checking between entities
- **Resource Management**: Smart pointers for automatic resource cleanup
- **Rendering System**: Abstraction over SDL3 for simplified rendering
- **Text System**: Flexible text rendering with fallback mechanism

## ?? Future Enhancements

- [ ] Sound effects and music
- [ ] Multiple enemy types
- [ ] Power-ups system
- [ ] Level editor
- [ ] Multiplayer support

## ?? License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ?? Acknowledgments

- Original Space Invaders game by Tomohiro Nishikado (1978)
- SDL3 development team for their excellent cross-platform library
- Open source community for inspiration and resources

---

<div align="center">
  <p>Made with ?? by [Your Name]</p>
</div>
