#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "../include/Graphics.h"

class Bullet;

class Player {
public:
    Player(Graphics* graphics);
    ~Player();

    void HandleEvent(const SDL_Event& event);
    void Update(float deltaTime);
    void Render();
    void Reset();
    
    void SetPosition(float x, float y);
    SDL_FPoint GetPosition() const { return position; }
    SDL_FRect GetBounds() const;
    
    bool IsDestroyed() const { return lives <= 0; }
    void Destroy() { lives = 0; }
    void TakeDamage();
    
    const std::vector<std::unique_ptr<Bullet>>& GetBullets() const { return bullets; }

private:
    Graphics* graphics;
    SDL_FPoint position{0.0f, 0.0f};
    SDL_FPoint velocity{0.0f, 0.0f};
    
    float width = 40.0f;
    float height = 30.0f;
    float moveSpeed = 300.0f;
    float shootCooldown = 0.0f;
    int lives = 3;
    
    // Key states
    bool moveLeft = false;
    bool moveRight = false;
    bool isShooting = false;
    
    std::vector<std::unique_ptr<Bullet>> bullets;
    
    void Shoot();
    void UpdateBullets(float deltaTime);
};
