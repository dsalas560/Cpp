#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <random>
#include <sstream>
#include <iomanip>
#include <limits>

enum class GameState {
    START,
    WAITING,
    READY,
    RESULT
};

int main()
{
    // Window setup
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Reaction Time Test");
    
    // Font setup
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        return -1;
    }
    
    // Text setup - SFML 3.0 requires font in constructor
    sf::Text text(font);
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::White);
    text.setString("Click anywhere to start");
    
    // Best score text
    sf::Text bestScoreText(font);
    bestScoreText.setCharacterSize(30);
    bestScoreText.setFillColor(sf::Color::Yellow);
    bestScoreText.setPosition(sf::Vector2f(10.0f, 10.0f));
    
    // Game state
    GameState state = GameState::START;
    sf::Clock waitClock;
    sf::Clock reactionClock;
    float waitTime = 0.0f;
    float reactionTime = 0.0f;
    float bestTime = std::numeric_limits<float>::max(); // Start with infinity
    
    // Random number generator for wait time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 7.0f);
    
    while (window.isOpen())
    {
        // Event handling
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            // Mouse click handling
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (state == GameState::START) {
                    state = GameState::WAITING;
                    waitTime = dist(gen);
                    waitClock.restart();
                }
                else if (state == GameState::WAITING) {
                    state = GameState::START;
                    text.setString("Too early! Click to try again");
                }
                else if (state == GameState::READY) {
                    reactionTime = reactionClock.getElapsedTime().asSeconds();
                    
                    // Update best time
                    if (reactionTime < bestTime) {
                        bestTime = reactionTime;
                    }
                    
                    state = GameState::RESULT;
                }
                else if (state == GameState::RESULT) {
                    state = GameState::START;
                }
            }
        }
        
        // Update game state
        if (state == GameState::WAITING) {
            if (waitClock.getElapsedTime().asSeconds() >= waitTime) {
                state = GameState::READY;
                reactionClock.restart();
            }
        }
        
        // Rendering
        window.clear();
        
        // Set background color and text based on state
        if (state == GameState::START) {
            window.clear(sf::Color(50, 50, 50));
            text.setString("Click anywhere to start");
            text.setFillColor(sf::Color::White);
        }
        else if (state == GameState::WAITING) {
            window.clear(sf::Color::Red);
            text.setString("Wait...");
            text.setFillColor(sf::Color::White);
        }
        else if (state == GameState::READY) {
            window.clear(sf::Color::Green);
            text.setString("CLICK!");
            text.setFillColor(sf::Color::White);
        }
        else if (state == GameState::RESULT) {
            window.clear(sf::Color(50, 50, 50));
            
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(3);
            ss << reactionTime << " seconds";
            
            text.setString("Your reaction time:\n" + ss.str() + "\n\nClick to try again");
            text.setFillColor(sf::Color::White);
        }
        
        // Center the text - SFML 3.0 API changes
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
        text.setPosition(sf::Vector2f(400.0f, 300.0f));
        
        window.draw(text);
        
        // Draw best score if we have one
        if (bestTime != std::numeric_limits<float>::max()) {
            std::ostringstream bss;
            bss << std::fixed << std::setprecision(3);
            bss << "Best: " << bestTime << " seconds";
            bestScoreText.setString(bss.str());
            window.draw(bestScoreText);
        }
        
        window.display();
    }
    
    return 0;
}