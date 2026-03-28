#include "bricks.h"
#include "config.h"
#include "globals.h"
#include <cstdlib>
#include <algorithm>

// define patterns
const bool PATTERN_FULL[COLS] = { true, true, true, true, true, true, true, true, true, true };
const bool PATTERN_HALF[COLS] = { true, false, true, false, true, true, false, true, false, true };
const bool PATTERN_ZIGZAG[COLS] = { false, true, false, true, false, true, false, true, false, true };
const bool PATTERN_ALTERNATE[COLS] = { true, true, false, false, true, true, false, false, true, true };
const bool PATTERN_THREE[COLS] = { true, true, true, false, true, false, true, true, true, true };
const bool PATTERN_EDGE[COLS] = { true, false, true, true, true, true, true, true, false, true };
const bool PATTERN_CENTER[COLS] = { false, true, true, true, true, true, true, true, true, false };
const bool PATTERN_DIAGONAL[COLS] = { true, true, false, true, true, false, true, true,false,true };
const bool PATTERN_DIAG2[COLS] = { true, true, true, true, true, true, true, true, true, true };

const bool* PATTERNS[] = {
    PATTERN_FULL, PATTERN_HALF, PATTERN_ZIGZAG, PATTERN_ALTERNATE, PATTERN_THREE,
    PATTERN_EDGE, PATTERN_CENTER, PATTERN_DIAGONAL, PATTERN_DIAG2
};

const int NUM_PATTERNS = sizeof(PATTERNS) / sizeof(PATTERNS[0]);


void shiftBricksDown(bool bricksAlive[ROWS][COLS], sf::RectangleShape bricks[ROWS][COLS])
{
    // Move rows down
    for (int r = ROWS - 1; r > 0; --r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            bricksAlive[r][c] = bricksAlive[r - 1][c];
            bricks[r][c].setFillColor(bricks[r - 1][c].getFillColor());
        }
    }

    // New top row: choose pattern or random fill
    int choice = std::rand() % (NUM_PATTERNS + 1); // allow extra for random fill
    for (int c = 0; c < COLS; ++c)
    {
        bool alive;
        if (choice == NUM_PATTERNS) // random fill case
            alive = (std::rand() % 100) < 70;
        else
            alive = PATTERNS[choice][c];

        bricksAlive[0][c] = alive;
        if (alive)
        {
            sf::Color col(static_cast<sf::Uint8>(std::rand() % 156 + 80),
                static_cast<sf::Uint8>(std::rand() % 156 + 80),
                static_cast<sf::Uint8>(std::rand() % 156 + 80));
            bricks[0][c].setFillColor(col);
        }
    }
}


bool handleBricks(
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool bricksAlive[ROWS][COLS],
    sf::RectangleShape bricks[ROWS][COLS],
    int& score, sf::Sound& brickHitSound)

{

    float ballLeft = ball.getPosition().x;
    float ballRight = ballLeft + ball.getRadius() * 2.f;
    float ballTop = ball.getPosition().y;
    float ballBottom = ballTop + ball.getRadius() * 2.f;


    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            if (!bricksAlive[r][c]) continue;

            float brickLeft = bricks[r][c].getPosition().x;
            float brickRight = brickLeft + bricks[r][c].getSize().x;
            float brickTop = bricks[r][c].getPosition().y;
            float brickBottom = brickTop + bricks[r][c].getSize().y;

            if (ballRight > brickLeft && ballLeft < brickRight &&
                ballBottom > brickTop && ballTop < brickBottom)
            {
                float overlapLeft = ballRight - brickLeft;
                float overlapRight = brickRight - ballLeft;
                float overlapTop = ballBottom - brickTop;
                float overlapBottom = brickBottom - ballTop;
                float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

                const float cornerThreshold = 3.f;
                if (minOverlap < cornerThreshold)
                    continue;

                bricksAlive[r][c] = false;
                score += 10;

                // brickSoundOn is in globals; bell played by caller (we still use the buffer/sound passed)
                if (brickSoundOn) brickHitSound.play();

                if (minOverlap == overlapLeft || minOverlap == overlapRight)
                    ballVelocity.x = -ballVelocity.x;
                else
                    ballVelocity.y = -ballVelocity.y;

                return true;
            }
        }
    }

    return false;

}



void drawBricks(sf::RenderWindow& window, sf::RectangleShape bricks[ROWS][COLS], bool bricksAlive[ROWS][COLS])
{
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            if (!bricksAlive[r][c]) continue;
            window.draw(bricks[r][c]);

            int stripes = 3;
            float stripeH = bricks[r][c].getSize().y / (stripes + 1); //30/3==10.
            for (int i = 1; i <= stripes; ++i)
            {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = { bricks[r][c].getPosition().x, bricks[r][c].getPosition().y + i * stripeH };
                line[1].position = { bricks[r][c].getPosition().x + bricks[r][c].getSize().x, bricks[r][c].getPosition().y + i * stripeH };
                line[0].color = sf::Color(255, 255, 255, 120);
                line[1].color = sf::Color(255, 255, 255, 120);
                window.draw(line);

            }
        }
    }

}


