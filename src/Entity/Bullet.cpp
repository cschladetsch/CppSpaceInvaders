#include "../../Entity/Bullet.h"

Bullet::Bullet(Graphics* graphics) : graphics(graphics) {
}

Bullet::~Bullet() {
}

void Bullet::Update(float deltaTime) {
    if (destroyed) return;
    
    // Update position based on velocity
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Bullet::Render() {
    if (destroyed) return;
    
    // Draw bullet as a small rectangle
    Color bulletColor(255, 255, 0);  // Yellow
    SDL_FRect bulletRect = {
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
    
    graphics->DrawRect(bulletRect, bulletColor, true);
}

void Bullet::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Bullet::SetVelocity(float x, float y) {
    velocity.x = x;
    velocity.y = y;
}

SDL_FRect Bullet::GetBounds() const {
    return SDL_FRect{
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
}

bool Bullet::IsOutOfBounds() const {
    // Check if bullet is outside the screen
    return position.y < -height || position.y > 600 + height ||
           position.x < -width || position.x > 800 + width;
}
