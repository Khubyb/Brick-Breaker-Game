
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void createPauseButton(sf::Font& font);
void createPauseMenu(sf::Font& font);
void drawPauseMenu(sf::RenderWindow& window);
int handlePauseMenuClick(sf::RenderWindow& window, sf::Music& bgMusic);
void drawPauseButton(sf::RenderWindow& window);

#endif
