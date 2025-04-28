#!/usr/bin/env python3
"""
Final fix script - creates all necessary header files and fixes all includes
"""

import os
import re
import glob

# Essential header templates
headers = {
    "Bullet.h": """#pragma once
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
""",

    "Enemy.h": """#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "Graphics.h"

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
    float shootProbability = 0.0005f;
    
    bool destroyed = false;
    
    std::vector<std::unique_ptr<Bullet>> bullets;
    
    void Shoot();
    void UpdateBullets(float deltaTime);
};
""",

    "Player.h": """#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "Graphics.h"

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
""",

    "UFO.h": """#pragma once
#include <SDL3/SDL.h>
#include "Graphics.h"

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
""",

    "Graphics.h": """#pragma once
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
""",

    "Transform.h": """#pragma once
#include <SDL3/SDL.h>

// Simple Transform class for 2D transformations
class Transform {
public:
    Transform();
    ~Transform();

    // Set position
    void SetPosition(float x, float y);
    SDL_FPoint GetPosition() const;

    // Set rotation
    void SetRotation(float angle);
    float GetRotation() const;

    // Set scale
    void SetScale(float x, float y);
    SDL_FPoint GetScale() const;

    // Calculate the transformation matrix based on position, rotation, and scale
    void CalculateMatrix();
    const SDL_FPoint* GetMatrix() const;
    
    // Update projection matrix - static method for screen dimensions
    static void UpdateProjectionMatrix(float width, float height);

private:
    SDL_FPoint position = {0.0f, 0.0f};
    float rotation = 0.0f;    // In degrees
    SDL_FPoint scale = {1.0f, 1.0f};

    // Transformation matrix (2x3 matrix for 2D transformation)
    SDL_FPoint matrix[3]; // [a, b, c, d, tx, ty] for the transformation matrix
};
""",

    "Barrier.h": """#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Graphics.h"

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
"""
}

def create_or_update_headers():
    """Create or update all essential header files"""
    print("Creating/updating header files...")
    
    # Make sure include directory exists
    os.makedirs("include", exist_ok=True)
    
    # Create each header file
    for header_name, content in headers.items():
        file_path = os.path.join("include", header_name)
        with open(file_path, "w") as file:
            file.write(content)
        print(f"  Created/updated: {file_path}")

def fix_source_includes():
    """Fix include directives in all source files"""
    print("Fixing include directives in source files...")
    
    count = 0
    # Find all .cpp files and fix their includes
    for root, dirs, files in os.walk("src"):
        for file in files:
            if file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                with open(file_path, 'r') as f:
                    content = f.read()
                
                # Fix all types of includes
                fixed = content
                patterns = [
                    r'#include\s+"\.\.\/([^"]+)"',        # "../something.h"
                    r'#include\s+"Entity\/([^"]+)"',      # "Entity/something.h"
                    r'#include\s+"Core\/([^"]+)"',        # "Core/something.h"
                    r'#include\s+"src\/([^"]+)"'          # "src/something.h"
                ]
                
                for pattern in patterns:
                    fixed = re.sub(pattern, r'#include "\1"', fixed)
                
                if fixed != content:
                    with open(file_path, 'w') as f:
                        f.write(fixed)
                    count += 1
                    print(f"  Fixed includes in: {file_path}")
    
    print(f"  Fixed {count} source files")

def main():
    print("FINAL PROJECT FIX")
    print("=================")
    
    # Create all essential header files
    create_or_update_headers()
    
    # Fix source file includes
    fix_source_includes()
    
    print("\nFix complete! Try building your project with:")
    print("cd build && cmake .. && make")

if __name__ == "__main__":
    main()
