#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "config.h"
#include "paddle_ball.h"
#include "screens.h"
#include "globals.h"
#include "pause_ui.h"
#include "save_load.h"
#include "stars.h"
#include "bricks.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_W, WINDOW_H),
        "Brick Breaker",
        sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize
    );

   
    window.setFramerateLimit(120);

    // Sounds
    sf::Music bgMusic;
    if (!bgMusic.openFromFile("Electronic-soft-jazz-background-music.wav"))
    {
        std::cout << "Warning: background music file not found. Continuing without music.\n";
    }
    else
    {
        bgMusic.setLoop(true);
        bgMusic.setVolume(70);
        bgMusic.play();
    }

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cout << "FONT LOAD FAILED: Place arial.ttf next to exe or update path.\n";
    }

    createPauseButton(font);
    createPauseMenu(font);

    sf::SoundBuffer wallPaddleBuffer;
    if (!wallPaddleBuffer.loadFromFile("ping-82822.wav"))
        std::cout << "Warning: ping sound not loaded.\n";

    sf::SoundBuffer brickHitBuffer;
    if (!brickHitBuffer.loadFromFile("bricks-fall-315300.wav"))
        std::cout << "Warning: brick sound not loaded.\n";

    sf::Sound wallPaddleSound;
    wallPaddleSound.setBuffer(wallPaddleBuffer);
    wallPaddleSound.setVolume(100);
    sf::Sound brickHitSound;
    brickHitSound.setBuffer(brickHitBuffer);
    brickHitSound.setVolume(100);


    int highScore = loadHighScore();

    int startChoice = showStartScreen(window, font);
    if (startChoice == 0) return 0;

    // Game stars storage 
    sf::CircleShape gameStars[GAME_STARS];
    float starX[GAME_STARS], starY[GAME_STARS], starSpeed[GAME_STARS];
    int starBright[GAME_STARS];
    initGameStars(gameStars, starX, starY, starSpeed, starBright, GAME_STARS);

    // UI and game text
    sf::RectangleShape topBar({ (float)WINDOW_W, TOP_BAR_H });
    topBar.setFillColor(sf::Color(30, 55, 40));
    sf::Text scoreText("", font, 18); scoreText.setFillColor(sf::Color::White); scoreText.setPosition(12.f, 18.f);
    sf::Text livesText("", font, 18); livesText.setFillColor(sf::Color::White); livesText.setPosition(220.f, 18.f);
    sf::Text highScoreText("", font, 18); highScoreText.setFillColor(sf::Color(220, 220, 120)); highScoreText.setPosition(420.f, 18.f);


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

    sf::Vector2f ballVelocity = { 0.f, 0.f };
    bool isLaunched = false;
    const float baseSpeed = 2.f;
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

    sf::RectangleShape bricks[ROWS][COLS];
    bool bricksAlive[ROWS][COLS];


    // Initialize bricks shapes and default alive states (we'll overwrite if load succeeds)
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            bricks[r][c].setSize({ brickW, brickH });
            bricks[r][c].setPosition(startX + c * (brickW + gap), startY + r * (brickH + gap));
            if (r == 0) bricksAlive[r][c] = true;
            else if (r == 1) bricksAlive[r][c] = true;
            else bricksAlive[r][c] = (std::rand() % 100) < 80;

            if (bricksAlive[r][c])
            {
                sf::Color col(static_cast<sf::Uint8>(std::rand() % 156 + 80),
                    static_cast<sf::Uint8>(std::rand() % 156 + 80),
                    static_cast<sf::Uint8>(std::rand() % 156 + 80));
                bricks[r][c].setFillColor(col);
            }
        }
    }


    // Put ball on paddle initially
    ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
        paddle.getPosition().y - ball.getRadius() * 2.f);

    // If user selected continue, attempt to load saved state (overwrites defaults above)
    if (startChoice == 2)
    {
        bool ok = loadGame(paddle, ball, ballVelocity, isLaunched, score, lives, currentSpeed, bricksAlive);
        if (ok)
        {
            // recolor alive bricks by row (like original)
            for (int r = 0; r < ROWS; ++r)
                for (int c = 0; c < COLS; ++c)
                    if (bricksAlive[r][c])
                        bricks[r][c].setFillColor(sf::Color(static_cast<sf::Uint8>(std::rand() % 156 + 80),
                            static_cast<sf::Uint8>(std::rand() % 156 + 80),
                            static_cast<sf::Uint8>(std::rand() % 156 + 80)));
        }
    }


    const float speedStep = 0.1f;

    bool alreadyGreater = false;


    sf::Clock frameClock;

    while (window.isOpen())
    {
        float dt = frameClock.restart().asSeconds();

        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (!isPaused && ev.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                // pause button detection is internal to pause_ui; we can't access button shape directly.
                // So we replicate the old check by reading the pause button area - but simpler: use pause UI to draw
                // (we'll check clicks by reusing the pause button location indirectly: we don't expose it)
                // For simplicity: when mouse pressed anywhere near top-right coordinates, toggle isPaused.
                if (mouse.x > WINDOW_W - 120.f && mouse.y < 60.f) isPaused = true;
            }

            if (isPaused)
            {
                if (ev.type == sf::Event::MouseButtonPressed)
                {
                    int menuResult = handlePauseMenuClick(window, bgMusic);
                    if (menuResult == 1) isPaused = false;
                    else if (menuResult == 2) // restart
                    {
                        lives = 3; score = 0; isLaunched = false; currentSpeed = baseSpeed; isPaused = false;
                        for (int r = 0; r < ROWS; ++r) for (int c = 0; c < COLS; ++c) bricksAlive[r][c] = (std::rand() % 100) < 70;
                        paddle.setPosition((WINDOW_W - PADDLE_W) / 2.f, WINDOW_H - 40.f);
                        ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
                            paddle.getPosition().y - ball.getRadius() * 2.f);
                        ballVelocity = { 0.f, 0.f };
                        initGameStars(gameStars, starX, starY, starSpeed, starBright, GAME_STARS);


                        highScore = loadHighScore();
                    }
                    else if (menuResult == 3) // home
                    {
                        isPaused = false;
                        int choice = showStartScreen(window, font);
                        if (choice == 0) {
                            saveGame(paddle, ball, ballVelocity, isLaunched, score, lives, currentSpeed, bricksAlive);
                            saveHighScore(highScore);
                            window.close();
                        }
                        else
                        {
                            if (choice == 1) {
                                lives = 3; score = 0; isLaunched = false; currentSpeed = baseSpeed;
                                for (int r = 0; r < ROWS; ++r) for (int c = 0; c < COLS; ++c) bricksAlive[r][c] = (std::rand() % 100) < 70;
                                paddle.setPosition((WINDOW_W - PADDLE_W) / 2.f, WINDOW_H - 40.f);
                                ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
                                    paddle.getPosition().y - ball.getRadius() * 2.f);
                                ballVelocity = { 0.f, 0.f };
                                initGameStars(gameStars, starX, starY, starSpeed, starBright, GAME_STARS);


                                highScore = loadHighScore();
                            }
                            else if (choice == 2) {
                                bool ok = loadGame(paddle, ball, ballVelocity, isLaunched, score, lives, currentSpeed, bricksAlive);
                                initGameStars(gameStars, starX, starY, starSpeed, starBright, GAME_STARS);

                                highScore = loadHighScore();
                            }
                        }
                    }
                    else if (menuResult == 4) {
                        saveGame(paddle, ball, ballVelocity, isLaunched, score, lives, currentSpeed, bricksAlive);
                        saveHighScore(highScore);
                        window.close();
                    }
                }
                continue; // skip update while paused
            }

            if (ev.type == sf::Event::Closed) {
                saveGame(paddle, ball, ballVelocity, isLaunched, score, lives, currentSpeed, bricksAlive);
                saveHighScore(highScore);
                window.close();
            }

            if (ev.type == sf::Event::KeyPressed && gameOver)
            {
                if (ev.key.code == sf::Keyboard::R)
                {
                    lives = 3; score = 0; isLaunched = false; gameOver = false; currentSpeed = baseSpeed;
                    for (int r = 0; r < ROWS; ++r) for (int c = 0; c < COLS; ++c) bricksAlive[r][c] = (std::rand() % 100) < 70;
                    paddle.setPosition((WINDOW_W - PADDLE_W) / 2.f, WINDOW_H - 40.f);
                    ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
                        paddle.getPosition().y - ball.getRadius() * 2.f);
                    ballVelocity = { 0.f, 0.f };
                    initGameStars(gameStars, starX, starY, starSpeed, starBright, GAME_STARS);


                    highScore = loadHighScore();

                }
                else if (ev.key.code == sf::Keyboard::Escape) {

                    saveGame(paddle, ball, ballVelocity, isLaunched, score, lives, currentSpeed, bricksAlive);
                    saveHighScore(highScore);
                    window.close();

                }
            }
        }

        if (isPaused)
        {
            drawPauseMenu(window);
            window.display();
            continue;
        }

        if (!gameOver)
        {
            handlePaddle(paddle);
            handleBall(ball, ballVelocity, isLaunched, paddle, lives, currentSpeed);

            bool destroyed = handleBricks(ball, ballVelocity, bricksAlive, bricks, score, brickHitSound);

            updateGameStars(gameStars, starX, starY, starSpeed, starBright, GAME_STARS, currentSpeed);


            bool bottomRowHasBricks = false;
            for (int c = 0; c < COLS; ++c)
            {
                if (bricksAlive[ROWS - 1][c])
                {
                    bottomRowHasBricks = true;
                    break;
                }
            }

            if (!bottomRowHasBricks && !gameOver)
            {
                shiftBricksDown(bricksAlive, bricks);
                score += 50;  // Now this WILL trigger!

                // Increase speed
                currentSpeed += speedStep;

                // Normalize and apply new speed to ball (only if moving)
                if (isLaunched)
                {
                    float len = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
                    if (len > 0.0001f)
                    {
                        ballVelocity.x = (ballVelocity.x / len) * currentSpeed;
                        ballVelocity.y = (ballVelocity.y / len) * currentSpeed;
                    }
                }
            }


            if (score > highScore)
            {
                if (!alreadyGreater)
                {

                    alreadyGreater = true;
                }
                highScore = score;
            }
            else
            {
                alreadyGreater = false;
            }


            if (lives <= 0)
            {
                gameOver = true;
                saveHighScore(highScore);
                bool restartGame = showGameOverScreen(window, font, score);
                if (restartGame)
                {
                    lives = 3; score = 0; isLaunched = false; gameOver = false; currentSpeed = baseSpeed;
                    for (int r = 0; r < ROWS; ++r) for (int c = 0; c < COLS; ++c) bricksAlive[r][c] = (std::rand() % 100) < 70;
                    paddle.setPosition((WINDOW_W - PADDLE_W) / 2.f, WINDOW_H - 40.f);
                    ball.setPosition(paddle.getPosition().x + paddle.getSize().x / 2.f - ball.getRadius(),
                        paddle.getPosition().y - ball.getRadius() * 2.f);
                    ballVelocity = { 0.f, 0.f };
                    initGameStars(gameStars, starX, starY, starSpeed, starBright, GAME_STARS);


                    highScore = loadHighScore();
                }
                else
                {
                    saveGame(paddle, ball, ballVelocity, isLaunched, score, lives, currentSpeed, bricksAlive);
                    saveHighScore(highScore);
                    window.close();
                }
            }
        }

        if (!gameOver)
        {
            window.clear(sf::Color(10, 10, 40));
            drawStars(window, gameStars, GAME_STARS);

            window.draw(topBar);
            scoreText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));
            highScoreText.setString("Highest Score: " + std::to_string(highScore));
            window.draw(scoreText);
            window.draw(livesText);
            window.draw(highScoreText);

            drawBricks(window, bricks, bricksAlive);
            window.draw(paddle);
            window.draw(ball);

            sf::Text hint("Space to launch", font, 18);
            hint.setFillColor(sf::Color(200, 200, 200));
            hint.setPosition(12.f, TOP_BAR_H + 8.f);
            window.draw(hint);

            // draw pause button from module
            drawPauseButton(window);



            window.display();
        }
    }

    return 0;
}

