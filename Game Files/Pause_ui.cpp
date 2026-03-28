#include "pause_ui.h"
#include "globals.h"
#include "config.h"
#include <string>

// local static UI elements (private to this translation unit)
static sf::RectangleShape pauseButton;
static sf::Text pauseButtonText;
static sf::RectangleShape resumeBtn, restartBtn, quitBtn, homeBtn;
static sf::RectangleShape musicBtn, wallBtn, brickBtn;
static sf::Text resumeTxt, restartTxt, quitTxt, homeTxt;
static sf::Text musicTxt, wallTxt, brickTxt;

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
            b.setPosition(WINDOW_W / 2.f - 125.f, y);
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
            t.setPosition(WINDOW_W / 2.f - 120.f, y + 10);
        };

    makeTxt(resumeTxt, "Resume", startY);
    makeTxt(restartTxt, "Restart", startY + 63);
    makeTxt(homeTxt, "Home", startY + 130);
    makeTxt(quitTxt, "Quit", startY + 195);
    makeTxt(musicTxt, "Music: ON", startY + gap * 4);
    makeTxt(wallTxt, "Paddle Sound: ON", startY + gap * 5);
    makeTxt(brickTxt, "Brick Sound: ON", startY + gap * 6);
}

void drawPauseMenu(sf::RenderWindow& window)
{
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

int handlePauseMenuClick(sf::RenderWindow& window, sf::Music& bgMusic)
{
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    auto clicked = [&](sf::RectangleShape& b) { return b.getGlobalBounds().contains(mouse); };

    if (clicked(resumeBtn)) return 1;
    if (clicked(restartBtn)) return 2;
    if (clicked(homeBtn)) return 3;
    if (clicked(quitBtn)) return 4;

    if (clicked(musicBtn))
    {
        musicOn = !musicOn;
        musicTxt.setString(musicOn ? "Music: ON" : "Music: OFF");
        if (musicOn) bgMusic.play();
        else bgMusic.pause();
    }

    if (clicked(wallBtn))
    {
        wallPaddleSoundOn = !wallPaddleSoundOn;
        wallTxt.setString(wallPaddleSoundOn ? "Paddle Sound: ON" : "Paddle Sound: OFF");
    }

    if (clicked(brickBtn))
    {
        brickSoundOn = !brickSoundOn;
        brickTxt.setString(brickSoundOn ? "Brick Sound: ON" : "Brick Sound: OFF");
    }

    return 0;
}

void drawPauseButton(sf::RenderWindow& window)
{
    window.draw(pauseButton);
    window.draw(pauseButtonText);
}
