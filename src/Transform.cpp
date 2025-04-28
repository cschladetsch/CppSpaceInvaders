#include "Transform.h"
#include <cmath>

Transform::Transform() {
    CalculateMatrix();
}

Transform::~Transform() {
}

void Transform::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
    CalculateMatrix();
}

SDL_FPoint Transform::GetPosition() const {
    return position;
}

void Transform::SetRotation(float angle) {
    rotation = angle;
    CalculateMatrix();
}

float Transform::GetRotation() const {
    return rotation;
}

void Transform::SetScale(float x, float y) {
    scale.x = x;
    scale.y = y;
    CalculateMatrix();
}

SDL_FPoint Transform::GetScale() const {
    return scale;
}

void Transform::CalculateMatrix() {
    // Convert rotation to radians
    float radians = rotation * (M_PI / 180.0f);
    float cosA = cos(radians);
    float sinA = sin(radians);
    
    // Calculate transformation matrix
    // [a c tx]
    // [b d ty]
    // [0 0 1 ]
    
    // Scale and rotate
    matrix[0].x = cosA * scale.x;  // a
    matrix[0].y = sinA * scale.x;  // b
    matrix[1].x = -sinA * scale.y; // c
    matrix[1].y = cosA * scale.y;  // d
    
    // Translate
    matrix[2].x = position.x;      // tx
    matrix[2].y = position.y;      // ty
}

const SDL_FPoint* Transform::GetMatrix() const {
    return matrix;
}

void Transform::UpdateProjectionMatrix(float width, float height) {
    // Static method implementation
    // Since we can't access instance members, we can just store the current projection settings
    // in static variables that can be used elsewhere if needed
    static float projectionWidth = 800.0f;
    static float projectionHeight = 600.0f;
    
    projectionWidth = width;
    projectionHeight = height;
    
    // Note: This implementation doesn't affect any Transform instance
    // In a real implementation, you might want to update a global projection matrix
    // or have instances query these values
}
