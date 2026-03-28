#ifndef PADDLE_BALL_H
#define PADDLE_BALL_H

#include <SFML/Graphics.hpp>

// Paddle
void handlePaddle(sf::RectangleShape& paddle);

// Ball
void handleBall(
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool& isLaunched,
    sf::RectangleShape& paddle,
    int& lives,
    float& currentSpeed
);

#endif
