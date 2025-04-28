#include "Graphics.h"
#include <iostream>

Graphics::Graphics(SDL_Renderer* renderer)
    : renderer(renderer) {
}

Graphics::~Graphics() {
    // Clean up texture cache
    for (auto& [path, texture] : textureCache) {
        SDL_DestroyTexture(texture);
    }
    textureCache.clear();
}

void Graphics::Clear(const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void Graphics::Present() {
    SDL_RenderPresent(renderer);
}

void Graphics::DrawRect(const SDL_FRect& rect, const Color& color, bool filled) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    if (filled) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderRect(renderer, &rect);
    }
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
}

SDL_Texture* Graphics::LoadTexture(const std::string& path) {
    // Check if texture is already loaded
    auto it = textureCache.find(path);
    if (it != textureCache.end()) {
        return it->second;
    }
    
    // Load the texture
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        std::cerr << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!texture) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    
    // Cache and return the texture
    textureCache[path] = texture;
    return texture;
}

void Graphics::DrawTexture(SDL_Texture* texture, const SDL_FRect& destRect, 
                          const SDL_FRect* srcRect, float angle, 
                          const SDL_FPoint* center, SDL_FlipMode flip) {
    if (!texture) return;
    
    SDL_RenderTextureRotated(renderer, texture, srcRect, &destRect, angle, center, flip);
}
