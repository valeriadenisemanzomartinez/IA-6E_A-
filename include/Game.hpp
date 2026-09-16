#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Map.hpp"
#include "Rescuer.hpp"

class Game
{
public:
    Game();
    void run();

private:
    void initialize();
    void handleEvents();
    void update(float dt);
    void render();
    void computePathToTarget(const sf::Vector2i& target);
    bool pathIsBlocked() const;
    void updateWindowTitle();
    sf::Vector2f cellCenter(int x, int y) const;
    sf::Vector2i pixelToCell(const sf::Vector2i& pixel) const;

    Map map_;
    Rescuer rescuer_;
    sf::RenderWindow window_;
    sf::Clock fireClock_;
    sf::Clock updateClock_;
    std::vector<sf::Vector2i> activePath_;
    sf::Vector2i targetCell_;
    sf::Vector2i victimCell_;
    sf::Vector2i evacuationCell_;
    bool victimRescued_ = false;
    int movementPoints_ = 0;
    float cellSize_ = 32.0f;
    float lastDisplayedCost_ = 0.0f;
};
