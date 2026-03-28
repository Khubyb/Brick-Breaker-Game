#ifndef SCREENS_H
#define SCREENS_H

#include <SFML/Graphics.hpp>

int showStartScreen(sf::RenderWindow& window, sf::Font& font);
bool showGameOverScreen(sf::RenderWindow& window, sf::Font& font, int finalScore);

#endif

