#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include "Graphics.h"

// Include SDL_ttf if available
#ifndef NO_SDL_TTF
#include <SDL3/SDL_ttf.h>
#endif

class TextRenderer {
public:
    TextRenderer(Graphics* graphics, SDL_Renderer* renderer);
    ~TextRenderer();
    
    // Load font from path
    bool LoadFont(const std::string& path, int fontSize);
    
    // Draw text with specified alignment
    void DrawText(const std::string& text, float x, float y, const Color& color, bool centered = true);
    
    // Get dimensions of text
    SDL_FPoint GetTextSize(const std::string& text);

private:
    Graphics* graphics;
    SDL_Renderer* renderer;
    
#ifndef NO_SDL_TTF
    TTF_Font* font = nullptr;
    std::unordered_map<std::string, SDL_Texture*> textCache;
#endif
    
    // Fallback method when SDL_ttf is not available
    void DrawTextFallback(const std::string& text, float x, float y, const Color& color, bool centered);
    
    // Text dimensions for fallback rendering
    const float charWidth = 12.0f;
    const float charHeight = 20.0f;
    const float charSpacing = 2.0f;
    
    // Draw a single character with primitive shapes (fallback method)
    void DrawCharFallback(char c, float x, float y, const Color& color);
};
