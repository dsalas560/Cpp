#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <random>
#include <vector>
#include <sstream>
#include <iomanip>

struct Dot {
    sf::CircleShape shape;
    sf::Vector2f position;
    
    Dot(float x, float y, float radius, sf::Color color) {
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(sf::Vector2f(radius, radius));
        position = sf::Vector2f(x, y);
        shape.setPosition(position);
    }
    
    bool contains(sf::Vector2f point) {
        float dx = point.x - position.x;
        float dy = point.y - position.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        return distance <= shape.getRadius();
    }
};

class FPSTrainer {
private:
    std::vector<Dot> dots;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> distX;
    std::uniform_real_distribution<float> distY;
    
    int score;
    int minDots;
    int maxDots;
    float dotRadius;
    sf::Vector2f windowSize;
    
public:
    FPSTrainer(sf::Vector2f winSize, int minDots = 3, int maxDots = 6, float radius = 20.0f) 
        : gen(rd()), 
          windowSize(winSize),
          minDots(minDots), 
          maxDots(maxDots),
          dotRadius(radius),
          score(0) {
        
        // Set distribution ranges with padding so dots don't spawn at edges
        distX = std::uniform_real_distribution<float>(radius + 10, winSize.x - radius - 10);
        distY = std::uniform_real_distribution<float>(radius + 10, winSize.y - radius - 10);
        
        // Spawn initial dots - START WITH MAXIMUM
        spawnDots(maxDots);
    }
    
    void spawnDots(int count) {
        for (int i = 0; i < count; ++i) {
            float x = distX(gen);
            float y = distY(gen);
            dots.emplace_back(x, y, dotRadius, sf::Color::Red);
        }
    }
    
    void handleClick(sf::Vector2f mousePos) {
        // Check if any dot was clicked
        for (auto it = dots.begin(); it != dots.end();) {
            if (it->contains(mousePos)) {
                it = dots.erase(it);
                score++;
                
                // FIXED: Check AFTER removal if we need to spawn more
                // When we go from 6->5->4->3, spawn 3 more to get back to 6
                if (static_cast<int>(dots.size()) == minDots) {
                    spawnDots(3);
                }
                
                return; // Only remove one dot per click
            } else {
                ++it;
            }
        }
    }
    
    void draw(sf::RenderWindow& window) {
        for (auto& dot : dots) {
            window.draw(dot.shape);
        }
    }
    
    int getScore() const { return score; }
    int getDotCount() const { return dots.size(); }
};

int main()
{
    // Window setup
    const unsigned int WIDTH = 1280;
    const unsigned int HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "FPS Trainer");
    window.setFramerateLimit(60);
    
    // Font setup
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        return -1;
    }
    
    // Score text
    sf::Text scoreText(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(sf::Vector2f(10.0f, 10.0f));
    
    // Dot count text
    sf::Text dotCountText(font);
    dotCountText.setCharacterSize(24);
    dotCountText.setFillColor(sf::Color::Yellow);
    dotCountText.setPosition(sf::Vector2f(10.0f, 50.0f));
    
    // Instructions
    sf::Text instructionText(font);
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color(200, 200, 200));
    instructionText.setPosition(sf::Vector2f(10.0f, HEIGHT - 30.0f));
    instructionText.setString("Click the red dots! 3 new dots spawn when you reach 3 remaining");
    
    // Create game
    FPSTrainer trainer(sf::Vector2f(WIDTH, HEIGHT), 3, 6, 20.0f);
    
    while (window.isOpen())
    {
        // Event handling
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            // Mouse click handling
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x), 
                                         static_cast<float>(mouseEvent->position.y));
                    trainer.handleClick(mousePos);
                }
            }
            
            // ESC to close
            if (event->is<sf::Event::KeyPressed>()) {
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                }
            }
        }
        
        // Update UI text
        std::ostringstream ss;
        ss << "Score: " << trainer.getScore();
        scoreText.setString(ss.str());
        
        std::ostringstream dss;
        dss << "Dots: " << trainer.getDotCount();
        dotCountText.setString(dss.str());
        
        // Rendering
        window.clear(sf::Color(30, 30, 40));
        
        trainer.draw(window);
        window.draw(scoreText);
        window.draw(dotCountText);
        window.draw(instructionText);
        
        window.display();
    }
    
    return 0;
}