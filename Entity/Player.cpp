#include "Player.h"
#include "Bullet.h"
#include <algorithm>
#include <iostream>
#include <memory>

Player::Player(Graphics* graphics) : graphics(graphics) {
}

Player::~Player() {
    // Clean up bullets
    bullets.clear();
}

void Player::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.scancode) {
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A:
                moveLeft = true;
                break;
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D:
                moveRight = true;
                break;
            case SDL_SCANCODE_SPACE:
                isShooting = true;
                break;
        }
    }
    else if (event.type == SDL_EVENT_KEY_UP) {
        switch (event.key.scancode) {
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A:
                moveLeft = false;
                break;
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D:
                moveRight = false;
                break;
            case SDL_SCANCODE_SPACE:
                isShooting = false;
                break;
        }
    }
    
    // Debug output to verify key events are being received
    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
        std::cout << "Key event: " << (event.type == SDL_EVENT_KEY_DOWN ? "DOWN" : "UP") 
                  << " scancode: " << event.key.scancode << std::endl;
    }
}

void Player::Update(float deltaTime) {
    // Reset velocity
    velocity.x = 0.0f;
    
    // Apply movement based on key states
    if (moveLeft) {
        velocity.x = -moveSpeed;
    }
    
    if (moveRight) {
        velocity.x = moveSpeed;
    }
    
    // Update position based on velocity
    position.x += velocity.x * deltaTime;
    
    // Constrain player to screen boundaries
    position.x = std::max(width * 0.5f, std::min(position.x, 800.0f - width * 0.5f));
    
    // Handle shooting
    if (isShooting && shootCooldown <= 0.0f) {
        Shoot();
    }
    
    // Update shoot cooldown
    if (shootCooldown > 0.0f) {
        shootCooldown -= deltaTime;
    }
    
    // Update bullets
    UpdateBullets(deltaTime);
}

void Player::Render() {
    // Draw player ship as a simple rectangle
    Color playerColor(0, 255, 0);  // Green
    SDL_FRect playerRect = {
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
    
    graphics->DrawRect(playerRect, playerColor, true);
    
    // Draw a triangle on top to make it look like a ship
    float triangleHeight = height * 0.5f;
    graphics->DrawLine(
        position.x, position.y - height * 0.5f - triangleHeight,
        position.x - width * 0.5f, position.y - height * 0.5f,
        playerColor
    );
    graphics->DrawLine(
        position.x, position.y - height * 0.5f - triangleHeight,
        position.x + width * 0.5f, position.y - height * 0.5f,
        playerColor
    );
    
    // Render bullets
    for (auto& bullet : bullets) {
        bullet->Render();
    }
}

void Player::Reset() {
    // Reset player state
    lives = 3;
    position.x = 400.0f;
    position.y = 550.0f;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    shootCooldown = 0.0f;
    
    // Clear bullets
    bullets.clear();
}

void Player::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

SDL_FRect Player::GetBounds() const {
    return SDL_FRect{
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
}

void Player::TakeDamage() {
    lives--;
}

void Player::Shoot() {
    // Limit to only 2 bullets on screen at a time
    if (bullets.size() >= 2) {
        return;
    }
    
    auto bullet = std::make_unique<Bullet>(graphics);
    bullet->SetPosition(position.x, position.y - height * 0.5f);
    bullet->SetVelocity(0.0f, -500.0f);  // Shoot upward
    bullets.push_back(std::move(bullet));
    
    shootCooldown = 0.2f;  // Cooldown between shots
}

void Player::UpdateBullets(float deltaTime) {
    // Update all bullets
    for (auto& bullet : bullets) {
        bullet->Update(deltaTime);
    }
    
    // Remove destroyed or out-of-screen bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<Bullet>& bullet) { 
                return bullet->IsDestroyed() || bullet->IsOutOfBounds(); 
            }),
        bullets.end()
    );
}
