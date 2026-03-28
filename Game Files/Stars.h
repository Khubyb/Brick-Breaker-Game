
#ifndef STARS_H
#define STARS_H

#include <SFML/Graphics.hpp>

void initStarsForScreen(sf::CircleShape stars[], int count);
void initGameStars(sf::CircleShape stars[], float sx[], float sy[], float speed[], int bright[], int count);
void updateGameStars(sf::CircleShape stars[], float sx[], float sy[], float speed[], int bright[], int count, float currentSpeed);
void drawStars(sf::RenderWindow& window, sf::CircleShape stars[], int count);

#endif
