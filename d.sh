#!/bin/bash
# Direct approach to fix the Space Invaders project

echo "Diagnosing project..."
echo "====================="

# Find all header files
echo "Finding all header files:"
find . -name "*.h" | grep -v "build/"

# Find all source files
echo -e "\nFinding all source files:"
find . -name "*.cpp" | grep -v "build/"

# Check the content of Barrier.cpp
echo -e "\nChecking Barrier.cpp include statement:"
grep -n "#include" src/Entity/Barrier.cpp || echo "File not found or no include statements"

# Create necessary directories
echo -e "\nCreating include directory..."
mkdir -p include

# Find and copy Barrier.h if it exists
echo -e "\nLooking for Barrier.h..."
BARRIER_HEADER=$(find . -name "Barrier.h" | grep -v "build/" | head -1)
if [ -n "$BARRIER_HEADER" ]; then
    echo "Found Barrier.h at: $BARRIER_HEADER"
    echo "Copying to include directory..."
    cp "$BARRIER_HEADER" include/
else
    echo "Barrier.h not found! Creating a minimal version..."
    cat > include/Barrier.h << 'EOF'
#pragma once
#include <SDL3/SDL.h>
#include "Graphics.h"

// A barrier consists of multiple "bricks" that can be individually destroyed
class Barrier {
public:
    Barrier(Graphics* graphics, float x, float y);
    ~Barrier();

    void Update(float deltaTime);
    void Render();
    
    struct Brick {
        SDL_FRect rect;
        bool destroyed = false;
    };
    
    const std::vector<Brick>& GetBricks() const { return bricks; }
    void DamageBrick(int index);
    SDL_FPoint GetPosition() const { return position; }
    
private:
    Graphics* graphics;
    SDL_FPoint position{0.0f, 0.0f};
    
    float width = 80.0f;
    float height = 60.0f;
    float brickSize = 10.0f;  // Size of individual bricks
    
    std::vector<Brick> bricks;
    
    void CreateBricks();
};
EOF
fi

# Find and copy Graphics.h if it exists
echo -e "\nLooking for Graphics.h..."
GRAPHICS_HEADER=$(find . -name "Graphics.h" | grep -v "build/" | head -1)
if [ -n "$GRAPHICS_HEADER" ]; then
    echo "Found Graphics.h at: $GRAPHICS_HEADER"
    echo "Copying to include directory..."
    cp "$GRAPHICS_HEADER" include/
fi

# Update CMakeLists.txt
echo -e "\nUpdating CMakeLists.txt..."
sed -i 's/target_include_directories([^)]*)/target_include_directories(${PROJECT_NAME} PRIVATE\n    ${CMAKE_CURRENT_SOURCE_DIR}\/include\n    ${SDL3_INCLUDE_DIRS})/' CMakeLists.txt || echo "Could not update CMakeLists.txt (might be already updated)"

echo -e "\nFix complete. Try running:"
echo "cd build && cmake .. && make"
