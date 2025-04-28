#!/bin/bash
# Fix header includes in header files

echo "Fixing include paths in header files..."

# First, update Barrier.h to use the correct include path for Graphics.h
if [ -f "include/Barrier.h" ]; then
    echo "Updating include/Barrier.h..."
    # Replace the relative path include with a direct include
    sed -i 's/#include "\.\.\/src\/Graphics.h"/#include "Graphics.h"/' include/Barrier.h
fi

# Check if the Graphics.h file exists in the include directory
if [ ! -f "include/Graphics.h" ]; then
    echo "Graphics.h not found in include directory, looking for it elsewhere..."
    GRAPHICS_HEADER=$(find . -name "Graphics.h" | grep -v "build/" | head -1)
    if [ -n "$GRAPHICS_HEADER" ]; then
        echo "Found Graphics.h at: $GRAPHICS_HEADER"
        echo "Copying to include directory..."
        cp "$GRAPHICS_HEADER" include/
    else
        echo "Graphics.h not found. Creating a minimal version..."
        cat > include/Graphics.h << 'EOF'
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
EOF
    fi
fi

echo "Fixing includes in all header files..."
# Also check other header files for potentially problematic includes
for header in include/*.h; do
    # Replace any relative path includes with direct includes
    sed -i 's/#include "\.\.\/src\/\([A-Za-z]*\.h\)"/#include "\1"/' "$header"
    sed -i 's/#include "\.\.\/\([A-Za-z]*\.h\)"/#include "\1"/' "$header"
    sed -i 's/#include "Entity\/\([A-Za-z]*\.h\)"/#include "\1"/' "$header"
    sed -i 's/#include "Core\/\([A-Za-z]*\.h\)"/#include "\1"/' "$header"
    echo "Updated $header"
done

echo "Fix complete. Try building again with:"
echo "cd build && cmake .. && make"
