#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "../include/Graphics.h"

class Bullet;

class Enemy {
public:
    Enemy(Graphics* graphics);
    ~Enemy();

    void Update(float deltaTime);
    void Render();
    
    void SetPosition(float x, float y);
    SDL_FPoint GetPosition() const { return position; }
    SDL_FRect GetBounds() const;
    
    bool IsDestroyed() const { return destroyed; }
    void Destroy() { destroyed = true; }
    
    const std::vector<std::unique_ptr<Bullet>>& GetBullets() const { return bullets; }

private:
    Graphics* graphics;
    SDL_FPoint position{0.0f, 0.0f};
    SDL_FPoint velocity{50.0f, 0.0f};  // Initial movement to the right
    
    float width = 30.0f;
    float height = 30.0f;
    
    float moveSpeed = 50.0f;
    float dropAmount = 15.0f;
    float shootCooldown = 0.0f;
    float shootProbability = 0.0005f;  // Reduced from 0.005f to 0.0005f
    
    bool destroyed = false;
    
    std::vector<std::unique_ptr<Bullet>> bullets;
    
    void Shoot();
    void UpdateBullets(float deltaTime);
};
