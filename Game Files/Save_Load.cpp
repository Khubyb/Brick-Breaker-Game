#include "save_load.h"
#include "config.h"
#include <fstream>

const char SAVE_FILE[] = "savegame.txt";
const char HIGH_FILE[] = "highscore.txt";

void saveGame(
    sf::RectangleShape& paddle,
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool isLaunched,
    int score, int lives,
    float currentSpeed,
    bool bricksAlive[ROWS][COLS]
)
{
    std::ofstream out(SAVE_FILE);
    if (!out) return;

    float px = paddle.getPosition().x;
    float py = paddle.getPosition().y;
    float bx = ball.getPosition().x;
    float by = ball.getPosition().y;

    out << px << " " << py << "\n";
    out << bx << " " << by << "\n";
    out << ballVelocity.x << " " << ballVelocity.y << "\n";
    out << isLaunched << "\n";
    out << score << "\n";
    out << lives << "\n";
    out << currentSpeed << "\n";

    // Save bricks matrix
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            out << bricksAlive[r][c] << " ";
        }
        out << "\n";
    }

    out.close();
}

bool loadGame(
    sf::RectangleShape& paddle,
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool& isLaunched,
    int& score, int& lives,
    float& currentSpeed,
    bool bricksAlive[ROWS][COLS]
)
{
    std::ifstream in(SAVE_FILE);
    if (!in) return false;

    float px, py, bx, by;

    in >> px >> py;
    in >> bx >> by;
    in >> ballVelocity.x >> ballVelocity.y;
    in >> isLaunched;
    in >> score;
    in >> lives;
    in >> currentSpeed;

    // Load bricks matrix
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            in >> bricksAlive[r][c];
        }
    }

    in.close();

    paddle.setPosition(px, py);
    ball.setPosition(bx, by);

    return true;
}

int loadHighScore()
{
    std::ifstream in(HIGH_FILE);
    if (!in) return 0;

    int hs;
    in >> hs;

    in.close();
    return hs;
}

void saveHighScore(int hs)
{
    std::ofstream out(HIGH_FILE);
    if (!out) return;

    out << hs;

    out.close();
}
