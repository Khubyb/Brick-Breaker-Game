#include "screens.h"
#include "stars.h"
#include "config.h"
#include <string>

int showStartScreen(sf::RenderWindow& window, sf::Font& font)
{

    sf::CircleShape stars[START_SCREEN_STARS];
    initStarsForScreen(stars, START_SCREEN_STARS);

    sf::CircleShape moon(60.f);
    moon.setPosition(600.f, 80.f);
    moon.setFillColor(sf::Color(255, 255, 200));

    sf::CircleShape crater1(12.f); crater1.setPosition(620.f, 100.f); crater1.setFillColor(sf::Color(200, 200, 150));
    sf::CircleShape crater2(8.f); crater2.setPosition(650.f, 130.f); crater2.setFillColor(sf::Color(200, 200, 150));
    sf::CircleShape crater3(10.f); crater3.setPosition(610.f, 150.f); crater3.setFillColor(sf::Color(200, 200, 150));

    sf::Text title1("BRICK", font, 70);
    title1.setStyle(sf::Text::Bold);
    title1.setFillColor(sf::Color(255, 215, 0));            
    title1.setOutlineColor(sf::Color::Black);              
    title1.setOutlineThickness(4);                          
    title1.setPosition(
        WINDOW_W / 2.f - title1.getLocalBounds().width / 2.f,
        100.f
    );

    sf::Text title2("BREAKER", font, 70);
    title2.setStyle(sf::Text::Bold);
    title2.setFillColor(sf::Color(255, 140, 0));            
    title2.setOutlineColor(sf::Color::Black);
    title2.setOutlineThickness(4);
    title2.setPosition(
        WINDOW_W / 2.f - title2.getLocalBounds().width / 2.f,
        180.f
    );

    sf::Text subtitle("SPACE OUTLAW", font, 32);
    subtitle.setStyle(sf::Text::Bold);
    subtitle.setFillColor(sf::Color(255, 255, 0));          
    subtitle.setOutlineColor(sf::Color(0, 0, 0, 200));      
    subtitle.setOutlineThickness(3);
    subtitle.setPosition(
        WINDOW_W / 2.f - subtitle.getLocalBounds().width / 2.f,
        280.f
    );


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

    // Continue button
    sf::RectangleShape continueButton(sf::Vector2f(200.f, 60.f));
    continueButton.setFillColor(sf::Color(100, 180, 100));
    continueButton.setOutlineThickness(3.f);
    continueButton.setOutlineColor(sf::Color::White);
    continueButton.setPosition(WINDOW_W / 2.f - 100.f, 460.f);

    sf::Text continueText("CONTINUE", font, 32);
    continueText.setFillColor(sf::Color::White);
    continueText.setPosition(
        continueButton.getPosition().x + continueButton.getSize().x / 2.f - continueText.getLocalBounds().width / 2.f,
        continueButton.getPosition().y + continueButton.getSize().y / 2.f - continueText.getLocalBounds().height / 2.f - 5.f
    );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed) { window.close(); return 0; }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (playButton.getGlobalBounds().contains(mousePos)) return 1;
                if (continueButton.getGlobalBounds().contains(mousePos)) return 2;
            }


        }

        // Hover effect
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (playButton.getGlobalBounds().contains(mousePos))
            playButton.setFillColor(sf::Color(100, 180, 255));
        else
            playButton.setFillColor(sf::Color(70, 160, 240));

        if (continueButton.getGlobalBounds().contains(mousePos))
            continueButton.setFillColor(sf::Color(140, 220, 140));
        else
            continueButton.setFillColor(sf::Color(100, 180, 100));

        window.clear(sf::Color(10, 10, 40));

        drawStars(window, stars, START_SCREEN_STARS);
        window.draw(moon);
        window.draw(crater1); window.draw(crater2); window.draw(crater3);
        window.draw(title1); window.draw(title2); window.draw(subtitle);
        window.draw(playButton); window.draw(playText);
        window.draw(continueButton); window.draw(continueText);
        window.display();
    }

    return 0;
}


bool showGameOverScreen(sf::RenderWindow& window, sf::Font& font, int finalScore)
{
    sf::CircleShape stars[START_SCREEN_STARS];
    initStarsForScreen(stars, START_SCREEN_STARS);

    sf::CircleShape moon(60.f); moon.setPosition(600.f, 80.f); moon.setFillColor(sf::Color(255, 255, 200));
    sf::CircleShape crater1(12.f); crater1.setPosition(620.f, 100.f); crater1.setFillColor(sf::Color(200, 200, 150));
    sf::CircleShape crater2(8.f); crater2.setPosition(650.f, 130.f); crater2.setFillColor(sf::Color(200, 200, 150));
    sf::CircleShape crater3(10.f); crater3.setPosition(610.f, 150.f); crater3.setFillColor(sf::Color(200, 200, 150));

    sf::RectangleShape trioBlock1(sf::Vector2f(80.f, 80.f)); trioBlock1.setFillColor(sf::Color(255, 60, 100)); trioBlock1.setPosition(WINDOW_W / 2.f - 130.f, 100.f);
    sf::RectangleShape trioBlock2(sf::Vector2f(80.f, 80.f)); trioBlock2.setFillColor(sf::Color(80, 255, 100)); trioBlock2.setPosition(WINDOW_W / 2.f - 40.f, 100.f);
    sf::RectangleShape trioBlock3(sf::Vector2f(80.f, 80.f)); trioBlock3.setFillColor(sf::Color(100, 160, 255)); trioBlock3.setPosition(WINDOW_W / 2.f + 50.f, 100.f);

    sf::Text gameOverText("GAME OVER", font, 48); gameOverText.setFillColor(sf::Color::Red); gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(WINDOW_W / 2.f - gameOverText.getLocalBounds().width / 2.f, 220.f);

    sf::Text brickBreakerText("BRICK BREAKER", font, 36); brickBreakerText.setFillColor(sf::Color::Yellow); brickBreakerText.setStyle(sf::Text::Bold);
    brickBreakerText.setPosition(WINDOW_W / 2.f - brickBreakerText.getLocalBounds().width / 2.f, 290.f);

    sf::Text scoreText(("FINAL SCORE: " + std::to_string(finalScore)), font, 32); scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(WINDOW_W / 2.f - scoreText.getLocalBounds().width / 2.f, 350.f);

    sf::RectangleShape restartButton(sf::Vector2f(200.f, 50.f)); restartButton.setFillColor(sf::Color(70, 160, 240)); restartButton.setPosition(WINDOW_W / 2.f - 100.f, 420.f);
    sf::Text restartText("PLAY AGAIN", font, 28); restartText.setFillColor(sf::Color::White);
    restartText.setPosition(restartButton.getPosition().x + restartButton.getSize().x / 2.f - restartText.getLocalBounds().width / 2.f,
        restartButton.getPosition().y + restartButton.getSize().y / 2.f - restartText.getLocalBounds().height / 2.f - 5.f);

    sf::RectangleShape quitButton(sf::Vector2f(200.f, 50.f)); quitButton.setFillColor(sf::Color(160, 60, 80)); quitButton.setPosition(WINDOW_W / 2.f - 100.f, 490.f);
    sf::Text quitText("QUIT GAME", font, 28); quitText.setFillColor(sf::Color::White);
    quitText.setPosition(quitButton.getPosition().x + quitButton.getSize().x / 2.f - quitText.getLocalBounds().width / 2.f,
        quitButton.getPosition().y + quitButton.getSize().y / 2.f - quitText.getLocalBounds().height / 2.f - 5.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) { window.close(); return false; }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (restartButton.getGlobalBounds().contains(mousePos)) return true;
                if (quitButton.getGlobalBounds().contains(mousePos)) return false;
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::Enter) return true;
                if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Escape) return false;
            }
        }
        // hover effect
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (restartButton.getGlobalBounds().contains(mousePos)) restartButton.setFillColor(sf::Color(100, 180, 255));
        else restartButton.setFillColor(sf::Color(70, 160, 240));
        if (quitButton.getGlobalBounds().contains(mousePos)) quitButton.setFillColor(sf::Color(180, 80, 100));
        else quitButton.setFillColor(sf::Color(160, 60, 80));

        window.clear(sf::Color(10, 10, 40));
        drawStars(window, stars, START_SCREEN_STARS);
        window.draw(moon); window.draw(crater1); window.draw(crater2); window.draw(crater3);
        window.draw(trioBlock1); window.draw(trioBlock2); window.draw(trioBlock3);
        window.draw(gameOverText); window.draw(brickBreakerText); window.draw(scoreText);
        window.draw(restartButton); window.draw(restartText); window.draw(quitButton); window.draw(quitText);
        window.display();
    }

    return false;
}
