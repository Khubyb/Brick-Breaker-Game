#include "stars.h"
#include "config.h"
#include <cstdlib>

void initStarsForScreen(sf::CircleShape stars[], int count)
{
    for (int i = 0; i < count; ++i)
    {
        float size = 1.f + (std::rand() % 100) / 100.0f;
        stars[i] = sf::CircleShape(size);
        stars[i].setPosition(std::rand() % WINDOW_W, std::rand() % WINDOW_H);
        stars[i].setFillColor(sf::Color(255, 255, 255));
    }
}

void initGameStars(sf::CircleShape stars[], float sx[], float sy[], float speed[], int bright[], int count)
{
    for (int i = 0; i < count; ++i)
    {
        float size = 1.f + (std::rand() % 100) / 100.0f;
        stars[i] = sf::CircleShape(size);
        sx[i] = (float)(std::rand() % WINDOW_W);
        sy[i] = (float)(std::rand() % WINDOW_H);
        stars[i].setPosition(sx[i], sy[i]);
        bright[i] = 150 + std::rand() % 106;
        stars[i].setFillColor(sf::Color(255, 255, 255, bright[i]));
        speed[i] = 0.1f + (std::rand() % 100) / 500.0f;
    }
}

void updateGameStars(sf::CircleShape stars[], float sx[], float sy[], float speed[], int bright[], int count, float currentSpeed)
{
    for (int i = 0; i < count; ++i)
    {
        sy[i] += speed[i] * currentSpeed;
        if (sy[i] > WINDOW_H) sy[i] = -10.f;
        stars[i].setPosition(sx[i], sy[i]);

        if ((std::rand() % 100) < 2)
        {
            bright[i] = 150 + std::rand() % 106;
            stars[i].setFillColor(sf::Color(255, 255, 255, bright[i]));
        }
    }
}

void drawStars(sf::RenderWindow& window, sf::CircleShape stars[], int count)
{
    for (int i = 0; i < count; ++i)
        window.draw(stars[i]);
}
