#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "../src/Graphics.h"

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
