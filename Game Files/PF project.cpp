#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
//ar
// ---- Pause System Function Prototypes ----
void createPauseButton(sf::Font& font);
void createPauseMenu(sf::Font& font);
void drawPauseMenu(sf::RenderWindow& window);
int handlePauseMenuClick(sf::RenderWindow& window, sf::Music& bgMusic);


// ---------------------- Config ----------------------

const int ROWS = 10;
const int COLS = 10;
const unsigned int WINDOW_W = 800;
const unsigned int WINDOW_H = 600;
const float TOP_BAR_H = 70.f;

const float PADDLE_W = 120.f;
const float PADDLE_H = 20.f;
const float BALL_RADIUS = 10.f;

//ar
bool isPaused = false;

// Sound Toggles
bool musicOn = true;
bool wallPaddleSoundOn = true;
bool brickSoundOn = true;

// Pause Button
sf::RectangleShape pauseButton;
sf::Text pauseButtonText;

// Pause Menu Buttons
sf::RectangleShape resumeBtn, restartBtn, quitBtn, homeBtn;
sf::RectangleShape musicBtn, wallBtn, brickBtn;

sf::Text resumeTxt, restartTxt, quitTxt, homeTxt;
sf::Text musicTxt, wallTxt, brickTxt;


// ---------------------- NEW: Background Star Structure ----------------------for game(khu)
struct Star {
    sf::CircleShape shape;
    float speed;
    float brightness;
};

// ---------------------- Brick Patterns ----------------------

const bool PATTERN_FULL[COLS] = { true, true, true, true, true, true, true, true, true, true };
const bool PATTERN_HALF[COLS] = { true, false, true, false, true, true, false, true, false,true };
const bool PATTERN_ZIGZAG[COLS] = { false, true, false, true, false, true, false, true, false, true };
const bool PATTERN_ALTERNATE[COLS] = { true, true, false,false,true, true,false,false,true,true };
const bool PATTERN_THREE[COLS] = { true, true, true, false,true, false, true, true, true,true };
const bool PATTERN_EDGE[COLS] = { true, false, false, true, true, false, false, true,false,true };
const bool PATTERN_CENTER[COLS] = { false, true, true, true, true, true, true, true, true, false };
const bool PATTERN_DIAGONAL[COLS] = { true, true, false, false,true, true, false,false,true, true };
const bool PATTERN_DIAG2[COLS] = { true, true, true, true, true, true, true, true, true, true };
const bool PATTERN_RANDOM_FILL[COLS] = {}; // special pattern, filled randomly

// ---------------------- Array of all patterns ----------------------

const bool* PATTERNS[] = {
    PATTERN_FULL, PATTERN_HALF, PATTERN_ZIGZAG, PATTERN_ALTERNATE, PATTERN_THREE,
    PATTERN_EDGE, PATTERN_CENTER, PATTERN_DIAGONAL, PATTERN_DIAG2, PATTERN_RANDOM_FILL
};
const int NUM_PATTERNS = sizeof(PATTERNS) / sizeof(PATTERNS[0]);

// ---------------------- NEW: Game Over Screen Function ----------------------(khu)
bool showGameOverScreen(sf::RenderWindow& window, sf::Font& font, int finalScore)
{
    // Create the same space background as start screen
    std::vector<sf::CircleShape> stars;
    for (int i = 0; i < 100; ++i) {
        sf::CircleShape star(1.0f);
        star.setPosition(std::rand() % WINDOW_W, std::rand() % WINDOW_H);
        star.setFillColor(sf::Color::White);
        stars.push_back(star);
    }

    // Create colorful moon with gradient effect (same as start screen)
    sf::CircleShape moon(60.f);
    moon.setPosition(600.f, 80.f);
    moon.setFillColor(sf::Color(255, 255, 200)); // Soft yellow color

    // Add moon craters for more realistic look
    sf::CircleShape crater1(12.f);
    crater1.setPosition(620.f, 100.f);
    crater1.setFillColor(sf::Color(200, 200, 150));

    sf::CircleShape crater2(8.f);
    crater2.setPosition(650.f, 130.f);
    crater2.setFillColor(sf::Color(200, 200, 150));

    sf::CircleShape crater3(10.f);
    crater3.setPosition(610.f, 150.f);
    crater3.setFillColor(sf::Color(200, 200, 150));

    // Create block of trio (three colored blocks)
    sf::RectangleShape trioBlock1(sf::Vector2f(80.f, 80.f));
    trioBlock1.setFillColor(sf::Color(255, 60, 100)); // Red
    trioBlock1.setPosition(WINDOW_W / 2.f - 130.f, 100.f);
    trioBlock1.setOutlineThickness(2.f);
    trioBlock1.setOutlineColor(sf::Color::White);

    sf::RectangleShape trioBlock2(sf::Vector2f(80.f, 80.f));
    trioBlock2.setFillColor(sf::Color(80, 255, 100)); // Green
    trioBlock2.setPosition(WINDOW_W / 2.f - 40.f, 100.f);
    trioBlock2.setOutlineThickness(2.f);
    trioBlock2.setOutlineColor(sf::Color::White);

    sf::RectangleShape trioBlock3(sf::Vector2f(80.f, 80.f));
    trioBlock3.setFillColor(sf::Color(100, 160, 255)); // Blue
    trioBlock3.setPosition(WINDOW_W / 2.f + 50.f, 100.f);
    trioBlock3.setOutlineThickness(2.f);
    trioBlock3.setOutlineColor(sf::Color::White);

    // Game Over text
    sf::Text gameOverText("GAME OVER", font, 48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(WINDOW_W / 2.f - gameOverText.getLocalBounds().width / 2.f, 220.f);

    // Brick Breaker title
    sf::Text brickBreakerText("BRICK BREAKER", font, 36);
    brickBreakerText.setFillColor(sf::Color::Yellow);
    brickBreakerText.setStyle(sf::Text::Bold);
    brickBreakerText.setPosition(WINDOW_W / 2.f - brickBreakerText.getLocalBounds().width / 2.f, 290.f);

    // Final Score display
    sf::Text scoreText("FINAL SCORE: " + std::to_string(finalScore), font, 32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(WINDOW_W / 2.f - scoreText.getLocalBounds().width / 2.f, 350.f);

    // Restart button
    sf::RectangleShape restartButton(sf::Vector2f(200.f, 50.f));
    restartButton.setFillColor(sf::Color(70, 160, 240));
    restartButton.setOutlineThickness(2.f);
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setPosition(WINDOW_W / 2.f - 100.f, 420.f);

    sf::Text restartText("PLAY AGAIN", font, 28);
    restartText.setFillColor(sf::Color::White);
    restartText.setStyle(sf::Text::Bold);
    restartText.setPosition(
        restartButton.getPosition().x + restartButton.getSize().x / 2.f - restartText.getLocalBounds().width / 2.f,
        restartButton.getPosition().y + restartButton.getSize().y / 2.f - restartText.getLocalBounds().height / 2.f - 5.f
    );

    // Quit button
    sf::RectangleShape quitButton(sf::Vector2f(200.f, 50.f));
    quitButton.setFillColor(sf::Color(160, 60, 80));
    quitButton.setOutlineThickness(2.f);
    quitButton.setOutlineColor(sf::Color::White);
    quitButton.setPosition(WINDOW_W / 2.f - 100.f, 490.f);

    sf::Text quitText("QUIT GAME", font, 28);
    quitText.setFillColor(sf::Color::White);
    quitText.setStyle(sf::Text::Bold);
    quitText.setPosition(
        quitButton.getPosition().x + quitButton.getSize().x / 2.f - quitText.getLocalBounds().width / 2.f,
        quitButton.getPosition().y + quitButton.getSize().y / 2.f - quitText.getLocalBounds().height / 2.f - 5.f
    );

    // Game over screen loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }

            // Check for mouse click on buttons
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    // Check if restart button is clicked
                    if (restartButton.getGlobalBounds().contains(mousePos))
                    {
                        return true; // Restart the game
                    }

                    // Check if quit button is clicked
                    if (quitButton.getGlobalBounds().contains(mousePos))
                    {
                        return false; // Quit the game
                    }
                }
            }

            // Also allow keyboard controls
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::Enter)
                {
                    return true; // Restart game
                }
                if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Escape)
                {
                    return false; // Quit game
                }
            }
        }

        // Button hover effects
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (restartButton.getGlobalBounds().contains(mousePos))
        {
            restartButton.setFillColor(sf::Color(100, 180, 255)); // Lighter blue on hover
        }
        else
        {
            restartButton.setFillColor(sf::Color(70, 160, 240)); // Original blue
        }

        if (quitButton.getGlobalBounds().contains(mousePos))
        {
            quitButton.setFillColor(sf::Color(180, 80, 100)); // Lighter red on hover
        }
        else
        {
            quitButton.setFillColor(sf::Color(160, 60, 80)); // Original red
        }

        // Render game over screen with same space background as start screen
        window.clear(sf::Color(10, 10, 40)); // Dark blue space background

        // Draw stars
        for (const auto& star : stars) {
            window.draw(star);
        }

        // Draw colorful moon and craters
        window.draw(moon);
        window.draw(crater1);
        window.draw(crater2);
        window.draw(crater3);

        // Draw all game over screen elements
        window.draw(trioBlock1);
        window.draw(trioBlock2);
        window.draw(trioBlock3);
        window.draw(gameOverText);
        window.draw(brickBreakerText);
        window.draw(scoreText);
        window.draw(restartButton);
        window.draw(restartText);
        window.draw(quitButton);
        window.draw(quitText);

        window.display();
    }

    return false;
}

// ---------------------- NEW: Background Functions ----------------------(Here i do for game background)(khu)

// Create stars for the night sky background
std::vector<Star> createStars(int count) {
    std::vector<Star> stars;
    for (int i = 0; i < count; ++i) {
        Star star;
        float size = 1.0f + (std::rand() % 100) / 100.0f; // Size between 1.0 and 2.0
        star.shape = sf::CircleShape(size);
        star.shape.setPosition(std::rand() % WINDOW_W, std::rand() % WINDOW_H);

        // Varying brightness for stars
        star.brightness = 150 + std::rand() % 106; // 150-255
        star.shape.setFillColor(sf::Color(255, 255, 255, star.brightness));

        // Different speeds for parallax effect
        star.speed = 0.1f + (std::rand() % 100) / 500.0f; // 0.1-0.3

        stars.push_back(star);
    }
    return stars;
}

// Update stars for parallax scrolling effect
void updateStars(std::vector<Star>& stars, float currentSpeed) {
    for (auto& star : stars) {
        // Move stars based on their speed and current game speed
        star.shape.move(0, star.speed * currentSpeed);

        // Reset star position if it goes off screen
        if (star.shape.getPosition().y > WINDOW_H) {
            star.shape.setPosition(std::rand() % WINDOW_W, -10);
        }

        // Twinkling effect - randomly change brightness
        if (std::rand() % 100 < 2) { // 2% chance each frame
            star.brightness = 150 + std::rand() % 106;
            star.shape.setFillColor(sf::Color(255, 255, 255, star.brightness));
        }
    }
}

// Draw the background with stars
void drawBackground(sf::RenderWindow& window, std::vector<Star>& stars) {
    // Draw night sky gradient (dark blue to black)
    sf::RectangleShape sky(sf::Vector2f(WINDOW_W, WINDOW_H));
    sky.setFillColor(sf::Color(10, 10, 40));
    window.draw(sky);

    // Draw stars
    for (const auto& star : stars) {
        window.draw(star.shape);
    }

}

// ---------------------- NEW: Start Screen Function with Colorful Space Background ----------------------(Here i do for play button background)(khu)
bool showStartScreen(sf::RenderWindow& window, sf::Font& font)
{
    // Create space background with stars
    std::vector<sf::CircleShape> stars;
    for (int i = 0; i < 100; ++i) {
        sf::CircleShape star(1.0f);
        star.setPosition(std::rand() % WINDOW_W, std::rand() % WINDOW_H);
        star.setFillColor(sf::Color::White);
        stars.push_back(star);
    }

    // Create colorful moon with gradient effect
    sf::CircleShape moon(60.f);
    moon.setPosition(600.f, 80.f);
    moon.setFillColor(sf::Color(255, 255, 200)); // Soft yellow color

    // Add moon craters for more realistic look
    sf::CircleShape crater1(12.f);
    crater1.setPosition(620.f, 100.f);
    crater1.setFillColor(sf::Color(200, 200, 150));

    sf::CircleShape crater2(8.f);
    crater2.setPosition(650.f, 130.f);
    crater2.setFillColor(sf::Color(200, 200, 150));

    sf::CircleShape crater3(10.f);
    crater3.setPosition(610.f, 150.f);
    crater3.setFillColor(sf::Color(200, 200, 150));

    // Create title texts
    sf::Text title1("BRICK", font, 60);
    title1.setFillColor(sf::Color::White);
    title1.setStyle(sf::Text::Bold);
    title1.setPosition(WINDOW_W / 2.f - title1.getLocalBounds().width / 2.f, 120.f);

    sf::Text title2("BREAKER", font, 60);
    title2.setFillColor(sf::Color::White);
    title2.setStyle(sf::Text::Bold);
    title2.setPosition(WINDOW_W / 2.f - title2.getLocalBounds().width / 2.f, 190.f);

    sf::Text subtitle("SPACE OUTLAW", font, 30);
    subtitle.setFillColor(sf::Color::Yellow);
    subtitle.setPosition(WINDOW_W / 2.f - subtitle.getLocalBounds().width / 2.f, 270.f);

    // Create play button
    sf::RectangleShape playButton(sf::Vector2f(200.f, 60.f));
    playButton.setFillColor(sf::Color(70, 160, 240));
    playButton.setOutlineThickness(3.f);
    playButton.setOutlineColor(sf::Color::White);
    playButton.setPosition(WINDOW_W / 2.f - 100.f, 380.f);

    sf::Text playText("PLAY", font, 36);
    playText.setFillColor(sf::Color::White);
    playText.setStyle(sf::Text::Bold);
    playText.setPosition(
        playButton.getPosition().x + playButton.getSize().x / 2.f - playText.getLocalBounds().width / 2.f,
        playButton.getPosition().y + playButton.getSize().y / 2.f - playText.getLocalBounds().height / 2.f - 5.f
    );

    // Start screen loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }

            // Check for mouse click on play button
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    // Check if play button is clicked
                    if (playButton.getGlobalBounds().contains(mousePos))
                    {
                        return true; // Start the game
                    }
                }
            }

            // Also allow starting with Enter key
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
                {
                    return true; // Start the game
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    return false;
                }
            }
        }

        // Button hover effect
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (playButton.getGlobalBounds().contains(mousePos))
        {
            playButton.setFillColor(sf::Color(100, 180, 255)); // Lighter blue on hover
        }
        else
        {
            playButton.setFillColor(sf::Color(70, 160, 240)); // Original blue
        }

        // Render start screen
        window.clear(sf::Color(10, 10, 40)); // Dark blue space background

        // Draw stars
        for (const auto& star : stars) {
            window.draw(star);
        }

        // Draw colorful moon and craters
        window.draw(moon);
        window.draw(crater1);
        window.draw(crater2);
        window.draw(crater3);

        // Draw all start screen elements
        window.draw(title1);
        window.draw(title2);
        window.draw(subtitle);
        window.draw(playButton);
        window.draw(playText);

        window.display();
    }

    return false;
}

// ---------------------- Shift bricks down with patterns ----------------------
void shiftBricksDown(bool bricksAlive[ROWS][COLS], sf::RectangleShape bricks[ROWS][COLS])
{
    // Move all rows down by 1
    for (int r = ROWS - 1; r > 0; --r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            bricksAlive[r][c] = bricksAlive[r - 1][c];
            bricks[r][c].setFillColor(bricks[r - 1][c].getFillColor());
        }
    }

    // Choose a random pattern for the new top row
    int choice = std::rand() % NUM_PATTERNS;

    for (int c = 0; c < COLS; ++c)
    {
        bool alive;

        // Last pattern is RANDOM_FILL
        if (choice == NUM_PATTERNS - 1)
        {
            alive = (std::rand() % 100) < 70; // 70% chance for random brick
        }
        else
        {
            alive = PATTERNS[choice][c];
        }

        bricksAlive[0][c] = alive;

        if (alive)
        {
            // Assign random pastel-ish color for each brick
            sf::Color col(std::rand() % 156 + 80, std::rand() % 156 + 80, std::rand() % 156 + 80);
            bricks[0][c].setFillColor(col);
        }
    }
}

//ar

void createPauseButton(sf::Font& font)
{
    pauseButton.setSize({ 80.f, 35.f });
    pauseButton.setFillColor(sf::Color(70, 160, 240));
    pauseButton.setOutlineThickness(2.f);
    pauseButton.setOutlineColor(sf::Color::White);
    pauseButton.setPosition(WINDOW_W - 100.f, 15.f);

    pauseButtonText.setFont(font);
    pauseButtonText.setString("PAUSE");
    pauseButtonText.setCharacterSize(18);
    pauseButtonText.setFillColor(sf::Color::White);
    pauseButtonText.setPosition(
        pauseButton.getPosition().x + 10.f,
        pauseButton.getPosition().y + 6.f
    );
}

void createPauseMenu(sf::Font& font)
{
    auto makeBtn = [&](sf::RectangleShape& b, float y)
        {
            b.setSize({ 250.f, 50.f });
            b.setFillColor(sf::Color(60, 60, 120));
            b.setOutlineThickness(3);
            b.setOutlineColor(sf::Color::White);
            b.setPosition(WINDOW_W / 2 - 125.f, y);
        };
    float gap = 65.f;
    float startY = 70.f;
    makeBtn(resumeBtn, startY);
    makeBtn(restartBtn, startY + gap);
    makeBtn(homeBtn, startY + gap * 2);
    makeBtn(quitBtn, startY + gap * 3);
    makeBtn(musicBtn, startY + gap * 4);
    makeBtn(wallBtn, startY + gap * 5);
    makeBtn(brickBtn, startY + gap * 6);


    auto makeTxt = [&](sf::Text& t, const std::string& s, float y)
        {
            t.setFont(font);
            t.setString(s);
            t.setCharacterSize(26);
            t.setFillColor(sf::Color::White);
            t.setPosition(WINDOW_W / 2 - 120.f, y + 10);
        };

    makeTxt(resumeTxt, "Resume", startY);
    makeTxt(restartTxt, "Restart", startY + 63);
    makeTxt(homeTxt, "Home", startY + 130);
    makeTxt(quitTxt, "Quit", startY + 195);

    makeTxt(musicTxt, "Music: ON", startY + gap * 4);
    makeTxt(wallTxt, "Paddle Sound: ON", startY + gap * 5);
    makeTxt(brickTxt, "Brick Sound: ON", startY + gap * 6);
}


// ---------------------- Paddle ----------------------
void handlePaddle(sf::RectangleShape& paddle)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 0.f)
        paddle.move(-3.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x + paddle.getSize().x < (float)WINDOW_W)
        paddle.move(3.f, 0.f);
}

// ---------------------- Ball handling ----------------------
void handleBall(
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool& isLaunched,
    sf::RectangleShape& paddle,
    int& lives,
    float& currentSpeed, sf::Sound& wallPaddleSound)
{
    const float WALL_LEFT = 0.f;
    const float WALL_RIGHT = WINDOW_W;
    const float TOP = TOP_BAR_H;
    const float BOTTOM = WINDOW_H;

    if (!isLaunched)
    {
        // Stick ball exactly to paddle center
        ball.setPosition(
            paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
            paddle.getPosition().y - ball.getRadius() * 2.f
        );

        // Launch straight up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            isLaunched = true;
            ballVelocity = { 0.f, -currentSpeed };
        }
        return; // no further movement while not launched
    }

    // Move ball
    ball.move(ballVelocity);

    // Wall collisions
    if (ball.getPosition().x <= WALL_LEFT)
    {
        ball.setPosition(WALL_LEFT, ball.getPosition().y);
        ballVelocity.x = std::abs(ballVelocity.x);
        if (wallPaddleSoundOn) wallPaddleSound.play();   // <-- play ping
    }
    if (ball.getPosition().x + ball.getRadius() * 2.f >= WALL_RIGHT)
    {
        ball.setPosition(WALL_RIGHT - ball.getRadius() * 2.f, ball.getPosition().y);
        ballVelocity.x = -std::abs(ballVelocity.x);
        if (wallPaddleSoundOn) wallPaddleSound.play();   // <-- play ping
    }
    if (ball.getPosition().y <= TOP)
    {
        ball.setPosition(ball.getPosition().x, TOP);
        ballVelocity.y = std::abs(ballVelocity.y);
        if (wallPaddleSoundOn) wallPaddleSound.play();   // <-- play ping
    }

    // Paddle collision
    if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()) && ballVelocity.y > 0.f)
    {
        float hitX = (ball.getPosition().x + ball.getRadius()) - paddle.getPosition().x;
        float relativeHit = hitX / paddle.getSize().x;
        if (relativeHit < 0.f) relativeHit = 0.f;
        if (relativeHit > 1.f) relativeHit = 1.f;

        // Reverse vertical velocity
        ballVelocity.y = -currentSpeed;

        // Slight horizontal variation based on hit position
        float maxHorizontal = currentSpeed * 0.5f;
        ballVelocity.x = (relativeHit - 0.5f) * 2.f * maxHorizontal;

        // Nudge ball above paddle
        ball.setPosition(ball.getPosition().x, paddle.getPosition().y - ball.getRadius() * 2.f - 0.5f);
        if (wallPaddleSoundOn) wallPaddleSound.play();  // <-- play ping
    }

    // Ball lost
    if (ball.getPosition().y > BOTTOM)
    {
        lives--;
        isLaunched = false;

        // Reset ball
        ball.setPosition(
            paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
            paddle.getPosition().y - ball.getRadius() * 2.f
        );

        ballVelocity = { 0.f, -currentSpeed };
    }
}

// ---------------------- Bricks collision ----------------------
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

// ---------------------- Draw bricks ----------------------
void drawBricks(sf::RenderWindow& window, sf::RectangleShape bricks[ROWS][COLS], bool bricksAlive[ROWS][COLS])
{
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            if (!bricksAlive[r][c]) continue;
            window.draw(bricks[r][c]);

            // decorative stripes
            int stripes = 2;
            float stripeH = bricks[r][c].getSize().y / (stripes + 1);
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


//ar

void drawPauseMenu(sf::RenderWindow& window)
{
    // Background dim
    sf::RectangleShape dim(sf::Vector2f(WINDOW_W, WINDOW_H));
    dim.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(dim);

    window.draw(resumeBtn); window.draw(resumeTxt);
    window.draw(restartBtn); window.draw(restartTxt);
    window.draw(homeBtn); window.draw(homeTxt);
    window.draw(quitBtn); window.draw(quitTxt);

    window.draw(musicBtn); window.draw(musicTxt);
    window.draw(wallBtn); window.draw(wallTxt);
    window.draw(brickBtn); window.draw(brickTxt);
}

//ar

int handlePauseMenuClick(sf::RenderWindow& window, sf::Music& bgMusic)
{
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    auto clicked = [&](sf::RectangleShape& b)
        {
            return b.getGlobalBounds().contains(mouse);
        };

    // Resume
    if (clicked(resumeBtn)) return 1;

    // Restart
    if (clicked(restartBtn)) return 2;

    // Home
    if (clicked(homeBtn)) return 3;

    // Quit
    if (clicked(quitBtn)) return 4;

    // Toggle Music
    if (clicked(musicBtn))
    {
        musicOn = !musicOn;
        musicTxt.setString(musicOn ? "Music: ON" : "Music: OFF");
        if (musicOn) bgMusic.play();
        else bgMusic.pause();
    }

    // Toggle Wall/Paddle Sound
    if (clicked(wallBtn))
    {
        wallPaddleSoundOn = !wallPaddleSoundOn;
        wallTxt.setString(wallPaddleSoundOn ? "Wall/Paddle Sound: ON" : "Wall/Paddle Sound: OFF");
    }

    // Toggle Brick Sound
    if (clicked(brickBtn))
    {
        brickSoundOn = !brickSoundOn;
        brickTxt.setString(brickSoundOn ? "Brick Sound: ON" : "Brick Sound: OFF");
    }

    return 0; // nothing
}


// ---------------------- MAIN ----------------------
int main()
{
    std::srand((unsigned)std::time(nullptr));

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Brick Breaker");
    window.setFramerateLimit(120);


    //----------sound--------ar
    sf::Music bgMusic;
    if (!bgMusic.openFromFile("Electronic-soft-jazz-background-music.wav"))
        return -1;

    bgMusic.setLoop(true);
    bgMusic.setVolume(70); // 0 to 100
    bgMusic.play();

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        std::cout << "FONT LOAD FAILED: Place arial.ttf next to exe\n";
    // ar

    createPauseButton(font);
    createPauseMenu(font);


    sf::SoundBuffer wallPaddleBuffer;
    wallPaddleBuffer.loadFromFile("ping-82822.wav");

    // Sound for hitting bricks    ar
    sf::SoundBuffer brickHitBuffer;
    brickHitBuffer.loadFromFile("bricks-fall-315300.wav");

    // ---------------------- STEP 4: Sound Objects ----------------------
   // These MUST be placed here
    sf::Sound wallPaddleSound;
    wallPaddleSound.setBuffer(wallPaddleBuffer);
    wallPaddleSound.setVolume(100);

    sf::Sound brickHitSound;
    brickHitSound.setBuffer(brickHitBuffer);
    brickHitSound.setVolume(100);


    // NEW: Show start screen first with colorful space background
    bool startGame = showStartScreen(window, font);
    if (!startGame)
    {
        return 0; // Exit if user closed window from start screen
    }

    // NEW: Create background stars for game(khu)
    std::vector<Star> backgroundStars = createStars(150);

    // ORIGINAL GAME CODE BELOW - COMPLETELY UNCHANGED
    sf::RectangleShape topBar({ (float)WINDOW_W, TOP_BAR_H });
    topBar.setFillColor(sf::Color(30, 55, 40));

    sf::Text scoreText("", font, 18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(12.f, 18.f);

    sf::Text livesText("", font, 18);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(220.f, 18.f);

    sf::Text infoText("", font, 20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(12.f, TOP_BAR_H + 8.f);


    // Paddle
    sf::RectangleShape paddle({ PADDLE_W, PADDLE_H });
    paddle.setFillColor(sf::Color(70, 160, 240));
    paddle.setOutlineThickness(3.f);
    paddle.setOutlineColor(sf::Color::White);
    paddle.setPosition((WINDOW_W - PADDLE_W) / 2.f, WINDOW_H - 40.f);

    // Ball
    sf::CircleShape ball(BALL_RADIUS);
    ball.setFillColor(sf::Color(240, 100, 160));
    ball.setOutlineThickness(3.f);
    ball.setOutlineColor(sf::Color::White);

    sf::Vector2f ballVelocity;
    bool isLaunched = false;
    float baseSpeed = 2.f;
    float currentSpeed = baseSpeed;
    int lives = 3;
    int score = 0;
    bool gameOver = false;

    // Bricks
    float brickW = (WINDOW_W - 120.f) / (float)COLS;
    float brickH = 20.f;
    float startX = 45.f;
    float startY = TOP_BAR_H + 40.f;
    float gap = 3.f;

    // ---------------------- Attractive Initial Bricks ----------------------

    sf::RectangleShape bricks[ROWS][COLS];
    bool bricksAlive[ROWS][COLS];

    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            bricks[r][c].setSize({ brickW, brickH });
            bricks[r][c].setPosition(startX + c * (brickW + gap), startY + r * (brickH + gap));

            // Define which bricks are alive for the first 10 rows
            if (r == 0) bricksAlive[r][c] = PATTERN_FULL[c];            // Full row
            else if (r == 1) bricksAlive[r][c] = PATTERN_HALF[c];       // Alternating
            else if (r == 2) bricksAlive[r][c] = PATTERN_ZIGZAG[c];     // Zigzag
            else if (r == 3) bricksAlive[r][c] = PATTERN_ALTERNATE[c];  // Alternate start with false
            else if (r == 4) bricksAlive[r][c] = PATTERN_THREE[c];      // Group of three
            else if (r == 5) bricksAlive[r][c] = PATTERN_EDGE[c];       // Edges
            else if (r == 6) bricksAlive[r][c] = PATTERN_CENTER[c];     // Center focus
            else if (r == 7) bricksAlive[r][c] = PATTERN_DIAGONAL[c];   // Diagonal left
            else if (r == 8) bricksAlive[r][c] = PATTERN_DIAG2[c];      // Diagonal right
            else bricksAlive[r][c] = (std::rand() % 100) < 90;          // Last row random

            if (bricksAlive[r][c])
            {
                // Give each row a distinct color theme for visual appeal
                sf::Color col;
                switch (r)
                {
                case 0: col = sf::Color(255, 60, 100); break; // red
                case 1: col = sf::Color(255, 120, 100); break; // orange
                case 2: col = sf::Color(255, 220, 100); break; // yellow
                case 3: col = sf::Color(80, 255, 100); break; // green
                case 4: col = sf::Color(100, 245, 230); break; // cyan
                case 5: col = sf::Color(100, 60, 255); break; // blue
                case 6: col = sf::Color(150, 100, 255); break; // purple
                case 7: col = sf::Color(255, 100, 255); break; // pink
                case 8: col = sf::Color(200, 200, 200); break; // light gray
                default: col = sf::Color(std::rand() % 156 + 80, std::rand() % 156 + 80, std::rand() % 156 + 80); break;
                }
                bricks[r][c].setFillColor(col);


            }
        }
    }




    ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
        paddle.getPosition().y - ball.getRadius() * 2.f);

    const float speedStep = 0.1f;

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            //ar
            if (!isPaused && ev.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (pauseButton.getGlobalBounds().contains(mouse))
                {
                    isPaused = true;
                }
            }
            //ar

            if (isPaused)
            {
                if (ev.type == sf::Event::MouseButtonPressed)
                {


                    int menuResult = handlePauseMenuClick(window, bgMusic);

                    if (menuResult == 1)
                    {
                        isPaused = false;
                    }

                    else if (menuResult == 2) // Restart the game
                    {
                        // RESET EVERYTHING EXACTLY LIKE YOUR GAME OVER SCREEN RESET
                        lives = 3;
                        score = 0;
                        isLaunched = false;
                        currentSpeed = baseSpeed;
                        isPaused = false;

                        for (int r = 0; r < ROWS; r++)
                            for (int c = 0; c < COLS; c++)
                                bricksAlive[r][c] = (std::rand() % 100) < 70;

                        paddle.setPosition((WINDOW_W - PADDLE_W) / 2, WINDOW_H - 40);
                        ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2 - ball.getRadius(),
                            paddle.getPosition().y - ball.getRadius() * 2);

                        ballVelocity = { 0.f, -currentSpeed };

                        backgroundStars = createStars(150);
                    }

                    else if (menuResult == 3) // HOME
                    {
                        isPaused = false;

                        bool startGame = showStartScreen(window, font);
                        if (!startGame)
                            window.close();
                        else
                        {
                            // reset again
                            lives = 3;
                            score = 0;
                            isLaunched = false;
                            currentSpeed = baseSpeed;

                            backgroundStars = createStars(150);
                        }
                    }

                    else if (menuResult == 4) // Quit
                    {
                        window.close();
                    }



                }
                continue; // Skip game update
            }

            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::KeyPressed && gameOver)
            {
                if (ev.key.code == sf::Keyboard::R)
                {
                    // Restart
                    lives = 3;
                    score = 0;
                    isLaunched = false;
                    gameOver = false;
                    currentSpeed = baseSpeed;

                    for (int r = 0; r < ROWS; ++r)
                        for (int c = 0; c < COLS; ++c)
                            bricksAlive[r][c] = (std::rand() % 100) < 70;

                    paddle.setPosition((WINDOW_W - PADDLE_W) / 2.f, WINDOW_H - 40.f);
                    ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
                        paddle.getPosition().y - ball.getRadius() * 2.f);
                    ballVelocity = { 0.f, -currentSpeed };

                    // NEW: Reset stars on restart for game(khu)
                    backgroundStars = createStars(150);
                }
                else if (ev.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        //ar
        if (isPaused)
        {
            drawPauseMenu(window);
            window.display();
            continue;
        }

        if (!gameOver)
        {
            handlePaddle(paddle);
            handleBall(ball, ballVelocity, isLaunched, paddle, lives, currentSpeed, wallPaddleSound);

            bool destroyed = handleBricks(ball, ballVelocity, bricksAlive, bricks, score, brickHitSound);

            // NEW: Update background stars with current game speed for game(khu)
            updateStars(backgroundStars, currentSpeed);

            // check bottom row empty
            bool bottomHasAlive = false;
            for (int c = 0; c < COLS; ++c) if (bricksAlive[ROWS - 1][c]) { bottomHasAlive = true; break; }

            if (!bottomHasAlive)
            {
                shiftBricksDown(bricksAlive, bricks);
                currentSpeed += speedStep;
                if (isLaunched)
                {
                    sf::Vector2f dir = ballVelocity;
                    ballVelocity = dir * currentSpeed;
                }
            }

            // bricks reaching paddle -> lose
            bool brickTooLow = false;
            for (int r = 0; r < ROWS && !brickTooLow; ++r)
            {
                for (int c = 0; c < COLS; ++c)
                {
                    if (!bricksAlive[r][c]) continue;
                    if (bricks[r][c].getPosition().y + bricks[r][c].getSize().y >= paddle.getPosition().y)
                    {
                        brickTooLow = true;
                        break;
                    }
                }
            }
            if (brickTooLow) lives = 0;

            if (lives <= 0)
            {
                gameOver = true;

                // NEW: Show game over screen when player loses for score scren(khu)
                bool restartGame = showGameOverScreen(window, font, score);
                if (restartGame)
                {
                    // Restart the game
                    lives = 3;
                    score = 0;
                    isLaunched = false;
                    gameOver = false;
                    currentSpeed = baseSpeed;

                    for (int r = 0; r < ROWS; ++r)
                        for (int c = 0; c < COLS; ++c)
                            bricksAlive[r][c] = (std::rand() % 100) < 70;

                    paddle.setPosition((WINDOW_W - PADDLE_W) / 2.f, WINDOW_H - 40.f);
                    ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
                        paddle.getPosition().y - ball.getRadius() * 2.f);
                    ballVelocity = { 0.f, -currentSpeed };

                    // Reset stars on restart
                    backgroundStars = createStars(150);
                }
                else
                {
                    window.close();
                }
            }
        }

        // Only render the game if we're not in the game over screen
        if (!gameOver)
        {
            // NEW: Render background first for game(khu)
            drawBackground(window, backgroundStars);

            // render
            window.draw(topBar);

            scoreText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));
            window.draw(scoreText);
            window.draw(livesText);

            drawBricks(window, bricks, bricksAlive);
            window.draw(paddle);
            window.draw(ball);

            // REMOVED: The old game over display that showed on the game screen(khu)
            // Only show hint when game is active(khu)
            sf::Text hint("Space to launch", font, 18);
            hint.setFillColor(sf::Color(200, 200, 200));
            hint.setPosition(12.f, TOP_BAR_H + 8.f);
            window.draw(hint);

            //ar

            window.draw(pauseButton);
            window.draw(pauseButtonText);



            window.display();
        }
    }

    return 0;
}