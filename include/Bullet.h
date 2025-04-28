#pragma once
#include <SDL3/SDL.h>
#include "Graphics.h"

class Bullet {
public:
    Bullet(Graphics* graphics);
    ~Bullet();

    void Update(float deltaTime);
    void Render();
    
    void SetPosition(float x, float y);
    void SetVelocity(float x, float y);
    SDL_FPoint GetPosition() const { return position; }
    SDL_FRect GetBounds() const;
    
    bool IsDestroyed() const { return destroyed; }
    void Destroy() { destroyed = true; }
    bool IsOutOfBounds() const;

private:
    Graphics* graphics;
    SDL_FPoint position{0.0f, 0.0f};
    SDL_FPoint velocity{0.0f, 0.0f};
    
    float width = 5.0f;
    float height = 15.0f;
    bool destroyed = false;
};
