#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <string>
#include "Graphics.h" // For Color type
#include "TextRenderer.h" // Font-based TextRenderer

// Forward declarations
class Player;
class Enemy;
class Bullet;
class Barrier;
class UFO;

class Game {
public:
    Game(SDL_Window* window, SDL_Renderer* renderer);
    ~Game();

    void Initialize();
    void HandleEvent(const SDL_Event& event);
    void Update(float deltaTime);
    void Render();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Barrier>> barriers;
    std::unique_ptr<UFO> ufo;
    
    // Text renderer
    std::unique_ptr<TextRenderer> textRenderer;
    
    float gameTime = 0.0f;

    // Game state
    bool gameOver = false;
    int score = 0;
    int highScore = 0;
    int level = 1;
    
    // Game parameters
    const float enemySpawnTime = 5.0f;
    float enemySpawnTimer = 0.0f;

    void SpawnEnemies();
    void CreateBarriers();
    void CheckCollisions();
    void RenderScore();
};
