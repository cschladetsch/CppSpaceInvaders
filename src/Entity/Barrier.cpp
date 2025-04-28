#include "Barrier.h"

Barrier::Barrier(Graphics* graphics, float x, float y) : graphics(graphics) {
    position.x = x;
    position.y = y;
    CreateBricks();
}

Barrier::~Barrier() {
}

void Barrier::Update(float deltaTime) {
    // Barriers are static, so no update logic needed
}

void Barrier::Render() {
    Color barrierColor(0, 200, 0);  // Dark green
    
    // Render each brick that's not destroyed
    for (const auto& brick : bricks) {
        if (!brick.destroyed) {
            graphics->DrawRect(brick.rect, barrierColor, true);
        }
    }
}

void Barrier::CreateBricks() {
    // Create a rectangular barrier made of smaller bricks
    const int rows = (int)(height / brickSize);
    const int cols = (int)(width / brickSize);
    
    // Create a barrier shape (inverted U)
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            // Skip bricks to create an opening in the middle of the bottom row
            if (row == rows - 1 && col > cols/3 && col < 2*cols/3) {
                continue;
            }
            
            // Calculate brick position
            float brickX = position.x - width/2 + col * brickSize + brickSize/2;
            float brickY = position.y - height/2 + row * brickSize + brickSize/2;
            
            // Add the brick
            Brick brick;
            brick.rect = {
                brickX - brickSize/2,
                brickY - brickSize/2,
                brickSize,
                brickSize
            };
            brick.destroyed = false;
            bricks.push_back(brick);
        }
    }
}

void Barrier::DamageBrick(int index) {
    if (index >= 0 && index < (int)bricks.size()) {
        bricks[index].destroyed = true;
    }
}
