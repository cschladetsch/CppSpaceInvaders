# Space Invaders Clone

A modern C++23 implementation of the classic Space Invaders arcade game using SDL3.

## Demo

[Demo](resources/Demo.gif)

## Features

- Classic Space Invaders gameplay with modern enhancements
- Destructible barriers for protection
- UFO bonus enemies that appear periodically
- Scoring system with high score tracking
- Increasing difficulty with level progression
- Bullet limit (only 2 bullets on screen at once) for strategic gameplay
- Proper font rendering for text (when SDL\_ttf is available)

## Controls

- **Left/Right Arrow** or **A/D keys**: Move player ship
- **Space**: Fire bullets
- **R**: Restart game after game over

## Building the Game

### Prerequisites

- C++23 compatible compiler (GCC 11+, Clang 12+, MSVC 2022+)
- CMake 3.20 or higher
- SDL3 library

### Optional Dependencies

- SDL3\_ttf for better text rendering

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/cschladetsch/CppSpaceInvaders.git
   cd CppSpaceInvaders
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure and build:
   ```bash
   cmake ..
   make
   ```

4. Run the game:
   ```bash
   ./CppSpaceInvaders
   ```

### Using the Run Script

For convenience, a run script is included that handles building and running in one step:

```bash
./r
```

## File Structure

- `src/`: Main game source code
  - `main.cpp`: Entry point and game loop
  - `Game.cpp/h`: Game state and management
  - `Graphics.cpp/h`: Rendering utilities
  - `TextRenderer.cpp/h`: Font and text rendering
- `Entity/`: Game entity implementations
  - `Player.cpp/h`: Player ship implementation
  - `Enemy.cpp/h`: Enemy ships implementation
  - `Bullet.cpp/h`: Projectile implementation
  - `Barrier.cpp/h`: Defensive barriers implementation
  - `UFO.cpp/h`: Bonus UFO implementation
- `Transform.cpp/h`: Transformation utilities
- `assets/`: Game resources
  - `fonts/`: Font files for text rendering
- `Shaders/`: HLSL shader files (for DirectX version)

## Text Rendering

The game supports proper font-based text rendering if SDL3_ttf is available. If not, it falls back to a simpler primitive rendering method. See [FONT_SETUP.md](FONT_SETUP.md) for instructions on installing SDL3_ttf and setting up fonts.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Original Space Invaders game by Tomohiro Nishikado (1978)
- SDL3 development team for the excellent cross-platform library
