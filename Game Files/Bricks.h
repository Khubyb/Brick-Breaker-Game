
#ifndef BRICKS_H
#define BRICKS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "config.h"

// Brick patterns and functions
extern const bool* PATTERNS[];
extern const int NUM_PATTERNS;

void shiftBricksDown(bool bricksAlive[ROWS][COLS], sf::RectangleShape bricks[ROWS][COLS]);

bool handleBricks(
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool bricksAlive[ROWS][COLS],
    sf::RectangleShape bricks[ROWS][COLS],
    int& score,
    sf::Sound& brickHitSound);

void drawBricks(sf::RenderWindow& window, sf::RectangleShape bricks[ROWS][COLS], bool bricksAlive[ROWS][COLS]);

#endif 



