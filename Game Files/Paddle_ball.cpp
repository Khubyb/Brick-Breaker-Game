#include "paddle_ball.h"
#include "config.h" 
// Paddle

void handlePaddle(sf::RectangleShape& paddle) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 0.f)
        paddle.move(-5.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x + paddle.getSize().x < WINDOW_W)
        paddle.move(5.f, 0.f);
}

void handleBall(
    sf::CircleShape& ball,
    sf::Vector2f& ballVelocity,
    bool& isLaunched,
    sf::RectangleShape& paddle,
    int& lives,
    float& currentSpeed)

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

        return; // no further movement while not launched....

    }


    // Move ball
    ball.move(ballVelocity);

    // Wall collisions
    if (ball.getPosition().x <= WALL_LEFT)
    {
        ball.setPosition(WALL_LEFT, ball.getPosition().y);
        ballVelocity.x = std::abs(ballVelocity.x);
    }

    if (ball.getPosition().x + ball.getRadius() * 2.f >= WALL_RIGHT)
    {
        ball.setPosition(WALL_RIGHT - ball.getRadius() * 2.f, ball.getPosition().y);
        ballVelocity.x = -std::abs(ballVelocity.x);
    }

    if (ball.getPosition().y <= TOP)
    {
        ball.setPosition(ball.getPosition().x, TOP);
        ballVelocity.y = std::abs(ballVelocity.y);
    }

    // Paddle collision
    if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()) && ballVelocity.y > 0.f)
    {
        float disX = (ball.getPosition().x + ball.getRadius()) - paddle.getPosition().x;
        float relativeHit = disX / paddle.getSize().x;
        if (relativeHit < 0.f) relativeHit = 0.f;
        if (relativeHit > 1.f) relativeHit = 1.f;

        // Reverse vertical velocity
        ballVelocity.y = -currentSpeed;

        // Slight horizontal variation based on hit position
        float maxHorizontal = currentSpeed * 0.5f;
        ballVelocity.x = (relativeHit - 0.5f) * 2.f * maxHorizontal;

        // Nudge ball above paddle
        ball.setPosition(ball.getPosition().x, paddle.getPosition().y - ball.getRadius() * 2.f - 0.5f);

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
