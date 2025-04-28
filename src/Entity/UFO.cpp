#include "../../Entity/UFO.h"
#include <random>

UFO::UFO(Graphics* graphics) : graphics(graphics) {
    // Initialize with random spawn timer
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(5.0f, spawnInterval);
    spawnTimer = dist(gen);
    
    // UFO starts inactive
    active = false;
    destroyed = false;
}

UFO::~UFO() {
}

void UFO::Update(float deltaTime) {
    if (destroyed) return;
    
    if (!active) {
        // Update spawn timer
        spawnTimer -= deltaTime;
        if (spawnTimer <= 0.0f) {
            // Activate UFO
            active = true;
            // Start off-screen to the left
            position.x = -width;
            position.y = 50.0f;
            velocity.x = 150.0f;  // Move right
            
            // Reset timer for next spawn
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(10.0f, spawnInterval);
            spawnTimer = dist(gen);
        }
    } else {
        // Update position
        position.x += velocity.x * deltaTime;
        
        // Check if UFO has moved off-screen
        if (position.x > 800 + width) {
            active = false;
        }
    }
}

void UFO::Render() {
    if (!active || destroyed) return;
    
    // Draw UFO as an ellipse (approximated with a rectangle)
    Color ufoColor(255, 0, 255);  // Purple
    SDL_FRect ufoRect = {
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
    
    graphics->DrawRect(ufoRect, ufoColor, true);
    
    // Draw a "cockpit" in the middle
    Color cockpitColor(150, 150, 255);  // Light blue
    SDL_FRect cockpitRect = {
        position.x - width * 0.2f,
        position.y - height * 0.25f,
        width * 0.4f,
        height * 0.5f
    };
    
    graphics->DrawRect(cockpitRect, cockpitColor, true);
}

void UFO::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

SDL_FRect UFO::GetBounds() const {
    return SDL_FRect{
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
}
