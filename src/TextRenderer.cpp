#include "TextRenderer.h"
#include <iostream>

TextRenderer::TextRenderer(Graphics* graphics, SDL_Renderer* renderer) 
    : graphics(graphics), renderer(renderer) {
    
#ifndef NO_SDL_TTF
    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
    }
#endif
}

TextRenderer::~TextRenderer() {
#ifndef NO_SDL_TTF
    // Free cached textures
    for (auto& [key, texture] : textCache) {
        SDL_DestroyTexture(texture);
    }
    textCache.clear();
    
    // Close font
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    // Quit SDL_ttf
    TTF_Quit();
#endif
}

bool TextRenderer::LoadFont(const std::string& path, int fontSize) {
#ifndef NO_SDL_TTF
    // Close previous font if exists
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    // Load new font
    font = TTF_OpenFont(path.c_str(), fontSize);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    
    return true;
#else
    // SDL_ttf not available
    return false;
#endif
}

void TextRenderer::DrawText(const std::string& text, float x, float y, const Color& color, bool centered) {
#ifndef NO_SDL_TTF
    if (font) {
        // Check if text is already cached
        std::string cacheKey = text + "_" + std::to_string(color.r) + "_" + 
                            std::to_string(color.g) + "_" + 
                            std::to_string(color.b) + "_" + 
                            std::to_string(color.a);
        
        SDL_Texture* texture = nullptr;
        
        // Try to find in cache
        auto it = textCache.find(cacheKey);
        if (it != textCache.end()) {
            texture = it->second;
        } else {
            // Create SDL color
            SDL_Color sdlColor = { color.r, color.g, color.b, color.a };
            
            // Render text to surface
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), sdlColor);
            if (!surface) {
                std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
                DrawTextFallback(text, x, y, color, centered);
                return;
            }
            
            // Create texture from surface
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            
            if (!texture) {
                std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
                DrawTextFallback(text, x, y, color, centered);
                return;
            }
            
            // Cache the texture
            textCache[cacheKey] = texture;
        }
        
        // Get texture dimensions
        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        
        // Calculate position
        float textX = x;
        float textY = y;
        
        if (centered) {
            textX -= width / 2.0f;
            textY -= height / 2.0f;
        }
        
        // Draw texture
        SDL_FRect destRect = { textX, textY, (float)width, (float)height };
        SDL_RenderTexture(renderer, texture, nullptr, &destRect);
        
        return;
    }
#endif
    
    // Fallback to primitive rendering
    DrawTextFallback(text, x, y, color, centered);
}

SDL_FPoint TextRenderer::GetTextSize(const std::string& text) {
#ifndef NO_SDL_TTF
    if (font) {
        int width, height;
        TTF_SizeText(font, text.c_str(), &width, &height);
        return { (float)width, (float)height };
    }
#endif
    
    // Fallback size calculation
    return { text.length() * (charWidth + charSpacing), charHeight };
}

void TextRenderer::DrawTextFallback(const std::string& text, float x, float y, const Color& color, bool centered) {
    float totalWidth = text.length() * (charWidth + charSpacing);
    
    // Calculate start position
    float startX = x;
    if (centered) {
        startX -= totalWidth / 2;
    }
    
    // Draw each character
    for (size_t i = 0; i < text.length(); i++) {
        float charX = startX + i * (charWidth + charSpacing);
        DrawCharFallback(text[i], charX, centered ? y - charHeight/2 : y, color);
    }
}

void TextRenderer::DrawCharFallback(char c, float x, float y, const Color& color) {
    // Simplified fallback character rendering using basic shapes
    if (c >= 'A' && c <= 'Z') {
        // Draw uppercase letter with a simple rectangle
        SDL_FRect rect = { x, y, charWidth, charHeight };
        graphics->DrawRect(rect, color, false);
        
        // Draw a line inside to make it look a bit different
        graphics->DrawLine(x, y + charHeight/2, x + charWidth, y + charHeight/2, color);
    } 
    else if (c >= 'a' && c <= 'z') {
        // Draw lowercase letter with a smaller rectangle
        SDL_FRect rect = { x, y + charHeight*0.25f, charWidth, charHeight*0.75f };
        graphics->DrawRect(rect, color, false);
    }
    else if (c >= '0' && c <= '9') {
        // Draw number with a simple box
        SDL_FRect rect = { x, y, charWidth, charHeight };
        graphics->DrawRect(rect, color, false);
    }
    else if (c == ' ') {
        // Space character - do nothing
    }
    else {
        // Draw other characters as a small box
        SDL_FRect rect = { x + charWidth/4, y + charHeight/4, charWidth/2, charHeight/2 };
        graphics->DrawRect(rect, color, true);
    }
}
