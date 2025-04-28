#include "../../Entity/Enemy.h"
#include "../../Entity/Bullet.h"
#include <algorithm>
#include <random>
#include <memory>

Enemy::Enemy(Graphics* graphics) : graphics(graphics) {
}

Enemy::~Enemy() {
    // Clean up bullets
    bullets.clear();
}

void Enemy::Update(float deltaTime) {
    if (destroyed) return;
    
    // Update position based on velocity
    position.x += velocity.x * deltaTime;
    
    // Check screen boundaries
    if (position.x <= width * 0.5f || position.x >= 800 - width * 0.5f) {
        // Reverse direction and move down
        velocity.x = -velocity.x;
        position.y += dropAmount;
    }
    
    // Update shoot cooldown
    if (shootCooldown > 0.0f) {
        shootCooldown -= deltaTime;
    }
    
    // Random shooting - greatly reduced probability
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    if (shootCooldown <= 0.0f && dist(gen) < shootProbability) {
        Shoot();
    }
    
    // Update bullets
    UpdateBullets(deltaTime);
}

void Enemy::Render() {
    if (destroyed) return;
    
    // Draw enemy as a rectangle
    Color enemyColor(255, 0, 0);  // Red
    SDL_FRect enemyRect = {
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
    
    graphics->DrawRect(enemyRect, enemyColor, true);
    
    // Add some detail to make it look like an alien
    float eyeSize = width * 0.2f;
    Color eyeColor(255, 255, 255);  // White
    
    SDL_FRect leftEye = {
        position.x - width * 0.25f - eyeSize * 0.5f,
        position.y - height * 0.25f - eyeSize * 0.5f,
        eyeSize,
        eyeSize
    };
    
    SDL_FRect rightEye = {
        position.x + width * 0.25f - eyeSize * 0.5f,
        position.y - height * 0.25f - eyeSize * 0.5f,
        eyeSize,
        eyeSize
    };
    
    graphics->DrawRect(leftEye, eyeColor, true);
    graphics->DrawRect(rightEye, eyeColor, true);
    
    // Render bullets
    for (auto& bullet : bullets) {
        bullet->Render();
    }
}

void Enemy::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

SDL_FRect Enemy::GetBounds() const {
    return SDL_FRect{
        position.x - width * 0.5f,
        position.y - height * 0.5f,
        width,
        height
    };
}

void Enemy::Shoot() {
    auto bullet = std::make_unique<Bullet>(graphics);
    bullet->SetPosition(position.x, position.y + height * 0.5f);
    bullet->SetVelocity(0.0f, 300.0f);  // Shoot downward
    bullets.push_back(std::move(bullet));
    
    shootCooldown = 5.0f;  // Increased cooldown between shots from 2.0f to 5.0f
}

void Enemy::UpdateBullets(float deltaTime) {
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
