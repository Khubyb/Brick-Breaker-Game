#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <SFML/Graphics.hpp>

void saveGame(
    sf::RectangleShape& paddle,
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool isLaunched,
    int score, int lives,
    float currentSpeed,
    bool bricksAlive[10][10]
);

bool loadGame(
    sf::RectangleShape& paddle,
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool& isLaunched,
    int& score, int& lives,
    float& currentSpeed,
    bool bricksAlive[10][10]
);

int loadHighScore();
void saveHighScore(int hs);

#endif
