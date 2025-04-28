#pragma once
#include <SDL3/SDL.h>
#include "../include/Graphics.h"

class UFO {
public:
    UFO(Graphics* graphics);
    ~UFO();

    void Update(float deltaTime);
    void Render();
    
    void SetPosition(float x, float y);
    SDL_FPoint GetPosition() const { return position; }
    SDL_FRect GetBounds() const;
    
    bool IsDestroyed() const { return destroyed; }
    void Destroy() { destroyed = true; }
    
    int GetScoreValue() const { return scoreValue; }
    bool IsActive() const { return active; }
    
private:
    Graphics* graphics;
    SDL_FPoint position{0.0f, 0.0f};
    SDL_FPoint velocity{0.0f, 0.0f};
    
    float width = 50.0f;
    float height = 25.0f;
    
    bool destroyed = false;
    bool active = false;
    float spawnTimer = 0.0f;
    float spawnInterval = 15.0f;  // Spawn every 15 seconds
    
    int scoreValue = 100;  // Points for hitting the UFO
};
