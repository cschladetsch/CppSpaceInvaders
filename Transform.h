#pragma once
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
