#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>

class Rescuer
{
public:
    Rescuer();

    void setPositionCell(int x, int y);
    sf::Vector2i currentCell() const;
    sf::Vector2f position() const;
    void setPath(const std::vector<sf::Vector2i>& path);
    void clearPath();
    bool hasPath() const;
    void update(float dt);
    void draw(sf::RenderTarget& target);

    bool carryingVictim = false;

private:
    sf::CircleShape body_;
    sf::Vector2f position_;
    sf::Vector2i currentCell_;
    std::deque<sf::Vector2i> path_;
    float speed_;
};
