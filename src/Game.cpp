#include "Game.h"
#include "../include/Graphics.h"
#include "TextRenderer.h"
#include "../Entity/Player.h"
#include "../Entity/Enemy.h"
#include "../Entity/Bullet.h"
#include "../Entity/Barrier.h"
#include "../Entity/UFO.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

Game::Game(SDL_Window* window, SDL_Renderer* renderer)
    : window(window), renderer(renderer) {
}

Game::~Game() {
    // Cleanup happens in the member destructors
}

void Game::Initialize() {
    // Create graphics
    Graphics* graphics = new Graphics(renderer);
    
    // Create text renderer
    textRenderer = std::make_unique<TextRenderer>(graphics, renderer);
    
    // Attempt to load a font (falls back to primitive rendering if not found)
    textRenderer->LoadFont("assets/fonts/DejaVuSans.ttf", 24);
    
    // Create player
    player = std::make_unique<Player>(graphics);
    player->SetPosition(400.0f, 550.0f);
    
    // Create barriers
    CreateBarriers();
    
    // Create UFO
    ufo = std::make_unique<UFO>(new Graphics(renderer));
    
    // Initial enemy spawn
    SpawnEnemies();
    
    // Ensure we start with game in progress, not game over
    gameOver = false;
    score = 0;
    level = 1;
}

void Game::HandleEvent(const SDL_Event& event) {
    // Handle game-specific events
    if (event.type == SDL_EVENT_KEY_DOWN) {
        // In SDL3, key code handling is different
        if (event.key.scancode == SDL_SCANCODE_R && gameOver) {
            // Reset game on 'R' press when game over
            gameOver = false;
            if (score > highScore) {
                highScore = score;
            }
            score = 0;
            level = 1;
            enemies.clear();
            CreateBarriers(); // Recreate barriers
            SpawnEnemies();
            player->Reset();
        }
    }
    
    // Pass events to player for handling
    if (!gameOver && player) {
        player->HandleEvent(event);
    }
}

void Game::Update(float deltaTime) {
    if (gameOver || !player) {
        return;
    }
    
    gameTime += deltaTime;
    
    // Update player
    player->Update(deltaTime);
    
    // Update enemies
    for (auto& enemy : enemies) {
        enemy->Update(deltaTime);
    }
    
    // Update barriers
    for (auto& barrier : barriers) {
        barrier->Update(deltaTime);
    }
    
    // Update UFO
    if (ufo) {
        ufo->Update(deltaTime);
    }
    
    // Remove destroyed enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) { return enemy->IsDestroyed(); }),
        enemies.end()
    );
    
    // Check for game over condition
    if (player->IsDestroyed()) {
        gameOver = true;
        std::cout << "Game Over! Final Score: " << score << std::endl;
        return;
    }
    
    // Check if all enemies are destroyed
    if (enemies.empty()) {
        // Increase level and spawn new enemies
        level++;
        SpawnEnemies();
    }
    
    // Check for collisions
    CheckCollisions();
}

void Game::RenderScore() {
    // Render score at the top of the screen
    std::stringstream ss;
    ss << "SCORE: " << score << "   HIGH SCORE: " << highScore << "   LEVEL: " << level;
    textRenderer->DrawText(ss.str(), 400.0f, 20.0f, Color(255, 255, 255), true);
}

void Game::Render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);
    SDL_RenderClear(renderer);
    
    // Render player
    if (player && !player->IsDestroyed()) {
        player->Render();
    }
    
    // Render enemies
    for (auto& enemy : enemies) {
        enemy->Render();
    }
    
    // Render barriers
    for (auto& barrier : barriers) {
        barrier->Render();
    }
    
    // Render UFO
    if (ufo) {
        ufo->Render();
    }
    
    // Render score
    RenderScore();
    
    // Render game over message if needed
    if (gameOver) {
        // Game over overlay
        SDL_SetRenderDrawColor(renderer, 50, 0, 0, 180);
        SDL_FRect overlay = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &overlay);
        
        // Game over text
        textRenderer->DrawText("GAME OVER", 400.0f, 250.0f, Color(255, 255, 255), true);
        textRenderer->DrawText("PRESS R TO RESTART", 400.0f, 300.0f, Color(255, 255, 255), true);
        
        std::stringstream ss;
        ss << "FINAL SCORE: " << score;
        textRenderer->DrawText(ss.str(), 400.0f, 350.0f, Color(255, 255, 255), true);
    }
    
    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

void Game::SpawnEnemies() {
    const int rowCount = 3 + (level - 1) / 2; // More rows with higher levels
    const int colCount = 8;
    const float startX = 100.0f;
    const float startY = 50.0f;
    const float spacingX = 70.0f;
    const float spacingY = 50.0f;
    
    // Make sure we create a Graphics object that persists
    Graphics* graphics = new Graphics(renderer);
    
    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < colCount; col++) {
            auto enemy = std::make_unique<Enemy>(graphics);
            enemy->SetPosition(
                startX + col * spacingX,
                startY + row * spacingY
            );
            enemies.push_back(std::move(enemy));
        }
    }
}

void Game::CreateBarriers() {
    barriers.clear();
    
    // Create 4 barriers
    const int barrierCount = 4;
    const float barrierY = 450.0f;
    const float spacing = 160.0f;
    const float startX = 150.0f;
    
    for (int i = 0; i < barrierCount; i++) {
        float x = startX + i * spacing;
        barriers.push_back(std::make_unique<Barrier>(new Graphics(renderer), x, barrierY));
    }
}

void Game::CheckCollisions() {
    if (player == nullptr || gameOver) return;
    
    // Get player bullets
    const auto& playerBullets = player->GetBullets();
    
    // Check collision between player bullets and enemies
    for (auto& enemy : enemies) {
        for (auto& bullet : playerBullets) {
            if (!bullet->IsDestroyed() && !enemy->IsDestroyed()) {
                SDL_FRect bulletRect = bullet->GetBounds();
                SDL_FRect enemyRect = enemy->GetBounds();
                
                if (SDL_HasRectIntersectionFloat(&bulletRect, &enemyRect)) {
                    enemy->Destroy();
                    bullet->Destroy();
                    score += 10 * level; // More points in higher levels
                }
            }
        }
        
        // Check collision between enemy and player (if enemy reaches bottom)
        if (!enemy->IsDestroyed() && enemy->GetPosition().y > 500) {
            player->Destroy();
            gameOver = true;
        }
        
        // Check collision between enemy bullets and player
        const auto& enemyBullets = enemy->GetBullets();
        for (auto& bullet : enemyBullets) {
            if (!bullet->IsDestroyed() && !player->IsDestroyed()) {
                SDL_FRect bulletRect = bullet->GetBounds();
                SDL_FRect playerRect = player->GetBounds();
                
                if (SDL_HasRectIntersectionFloat(&bulletRect, &playerRect)) {
                    player->TakeDamage();
                    bullet->Destroy();
                    if (player->IsDestroyed()) {
                        gameOver = true;
                    }
                }
            }
            
            // Check collision between enemy bullets and barriers
            if (!bullet->IsDestroyed()) {
                for (auto& barrier : barriers) {
                    const auto& bricks = barrier->GetBricks();
                    SDL_FRect bulletRect = bullet->GetBounds();
                    
                    for (size_t i = 0; i < bricks.size(); i++) {
                        if (!bricks[i].destroyed) {
                            if (SDL_HasRectIntersectionFloat(&bulletRect, &bricks[i].rect)) {
                                barrier->DamageBrick(i);
                                bullet->Destroy();
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Check collision between player bullets and barriers
    for (auto& bullet : playerBullets) {
        if (!bullet->IsDestroyed()) {
            for (auto& barrier : barriers) {
                const auto& bricks = barrier->GetBricks();
                SDL_FRect bulletRect = bullet->GetBounds();
                
                for (size_t i = 0; i < bricks.size(); i++) {
                    if (!bricks[i].destroyed) {
                        if (SDL_HasRectIntersectionFloat(&bulletRect, &bricks[i].rect)) {
                            barrier->DamageBrick(i);
                            bullet->Destroy();
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Check collision between player bullets and UFO
    if (ufo && ufo->IsActive() && !ufo->IsDestroyed()) {
        for (auto& bullet : playerBullets) {
            if (!bullet->IsDestroyed()) {
                SDL_FRect bulletRect = bullet->GetBounds();
                SDL_FRect ufoRect = ufo->GetBounds();
                
                if (SDL_HasRectIntersectionFloat(&bulletRect, &ufoRect)) {
                    ufo->Destroy();
                    bullet->Destroy();
                    score += ufo->GetScoreValue() * level;
                }
            }
        }
    }
}
