#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

struct Color {
    Uint8 r, g, b, a;
    
    Color(Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255)
        : r(r), g(g), b(b), a(a) {}
};

class Graphics {
public:
    Graphics(SDL_Renderer* renderer);
    ~Graphics();

    // Basic drawing functions
    void Clear(const Color& color = Color(0, 0, 0, 255));
    void Present();
    
    // Primitive drawing functions
    void DrawRect(const SDL_FRect& rect, const Color& color, bool filled = true);
    void DrawLine(float x1, float y1, float x2, float y2, const Color& color);
    
    // Texture management
    SDL_Texture* LoadTexture(const std::string& path);
    void DrawTexture(SDL_Texture* texture, const SDL_FRect& destRect, 
                     const SDL_FRect* srcRect = nullptr, float angle = 0.0f, 
                     const SDL_FPoint* center = nullptr, SDL_FlipMode flip = SDL_FLIP_NONE);
    
    // Getters
    SDL_Renderer* GetRenderer() const { return renderer; }

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textureCache;
};
