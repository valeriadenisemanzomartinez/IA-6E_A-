#include "Game.hpp"

#include "AStar.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace
{
constexpr float kWindowWidth = 640.0f;
constexpr float kWindowHeight = 640.0f;
}

Game::Game()
    : window_(sf::VideoMode(static_cast<unsigned>(kWindowWidth), static_cast<unsigned>(kWindowHeight)), "Rescue Squad"),
      victimCell_(8, 8),
      evacuationCell_(17, 17)
{
    initialize();
}

void Game::initialize()
{
    window_.setFramerateLimit(60);
    window_.setKeyRepeatEnabled(false);
    map_.resetDefaultLayout();
    rescuer_.setPositionCell(1, 1);
    targetCell_ = sf::Vector2i(1, 1);
    activePath_.clear();
    movementPoints_ = 0;
    victimRescued_ = false;
    map_.setTerrain(victimCell_.x, victimCell_.y, TerrainType::Clean);
    map_.setTerrain(evacuationCell_.x, evacuationCell_.y, TerrainType::Clean);
    computePathToTarget(sf::Vector2i(18, 18));
    updateWindowTitle();
}

void Game::run()
{
    sf::Clock clock;
    while (window_.isOpen())
    {
        const float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void Game::handleEvents()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window_.close();
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            const auto mousePos = sf::Mouse::getPosition(window_);
            const auto cell = pixelToCell(mousePos);

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (map_.inBounds(cell.x, cell.y) && !map_.isWall(cell.x, cell.y))
                {
                    computePathToTarget(cell);
                }
            }
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                map_.toggleTerrain(cell.x, cell.y);
                if (!activePath_.empty())
                {
                    computePathToTarget(targetCell_);
                }
            }
        }
    }
}

void Game::update(float dt)
{
    rescuer_.update(dt);

    if (fireClock_.getElapsedTime().asSeconds() >= 3.0f)
    {
        map_.spreadFireRandomly();
        fireClock_.restart();
        if (pathIsBlocked())
        {
            computePathToTarget(targetCell_);
        }
    }

    if (!rescuer_.hasPath() && !activePath_.empty())
    {
        auto nextCell = activePath_.front();
        if (rescuer_.currentCell() == nextCell)
        {
            activePath_.erase(activePath_.begin());
        }
    }

    if (rescuer_.currentCell() == victimCell_ && !rescuer_.carryingVictim)
    {
        rescuer_.carryingVictim = true;
        victimRescued_ = true;
    }

    if (rescuer_.carryingVictim && rescuer_.currentCell() == evacuationCell_)
    {
        victimRescued_ = true;
        movementPoints_ += 50;
        rescuer_.carryingVictim = false;
    }

    if (pathIsBlocked())
    {
        computePathToTarget(targetCell_);
    }

    updateWindowTitle();
}

void Game::render()
{
    window_.clear(sf::Color(30, 30, 30));

    for (int y = 0; y < Map::GRID_SIZE; ++y)
    {
        for (int x = 0; x < Map::GRID_SIZE; ++x)
        {
            sf::RectangleShape cellShape(sf::Vector2f(cellSize_, cellSize_));
            cellShape.setPosition(static_cast<float>(x) * cellSize_, static_cast<float>(y) * cellSize_);
            cellShape.setFillColor(map_.colorAt(x, y));
            cellShape.setOutlineColor(sf::Color(80, 80, 80));
            cellShape.setOutlineThickness(1.0f);
            window_.draw(cellShape);
        }
    }

    if (!activePath_.empty())
    {
        sf::VertexArray line(sf::LineStrip, activePath_.size());
        for (std::size_t i = 0; i < activePath_.size(); ++i)
        {
            const auto& point = activePath_[i];
            line[i].position = sf::Vector2f(static_cast<float>(point.x) * cellSize_ + cellSize_ / 2.0f,
                                            static_cast<float>(point.y) * cellSize_ + cellSize_ / 2.0f);
            line[i].color = sf::Color(40, 220, 40, 170);
        }
        window_.draw(line);
    }

    sf::CircleShape evacuationZone(6.0f);
    evacuationZone.setFillColor(sf::Color::Green);
    evacuationZone.setPosition(evacuationCell_.x * cellSize_ + 12.0f, evacuationCell_.y * cellSize_ + 12.0f);
    window_.draw(evacuationZone);

    sf::CircleShape victim(6.0f);
    victim.setFillColor(sf::Color::Yellow);
    victim.setPosition(victimCell_.x * cellSize_ + 10.0f, victimCell_.y * cellSize_ + 10.0f);
    window_.draw(victim);

    rescuer_.draw(window_);
    window_.display();
}

void Game::computePathToTarget(const sf::Vector2i& target)
{
    targetCell_ = target;
    auto path = AStar::findPath(map_, rescuer_.currentCell(), targetCell_, rescuer_.carryingVictim);
    if (!path.empty() && path.front() == rescuer_.currentCell())
    {
        path.erase(path.begin());
    }
    activePath_ = path;
    rescuer_.setPath(activePath_);
}

bool Game::pathIsBlocked() const
{
    if (activePath_.empty())
    {
        return false;
    }

    for (const auto& cell : activePath_)
    {
        if (map_.isWall(cell.x, cell.y) || map_.isFire(cell.x, cell.y))
        {
            return true;
        }
    }

    return false;
}

void Game::updateWindowTitle()
{
    std::ostringstream oss;
    oss << "Rescue Squad - Movement: " << movementPoints_ << " - "
        << (victimRescued_ ? "Victim rescued successfully" : "Victim still missing");
    window_.setTitle(oss.str());
}

sf::Vector2f Game::cellCenter(int x, int y) const
{
    return sf::Vector2f(static_cast<float>(x) * cellSize_ + cellSize_ / 2.0f,
                        static_cast<float>(y) * cellSize_ + cellSize_ / 2.0f);
}

sf::Vector2i Game::pixelToCell(const sf::Vector2i& pixel) const
{
    const int x = std::clamp(pixel.x / static_cast<int>(cellSize_), 0, Map::GRID_SIZE - 1);
    const int y = std::clamp(pixel.y / static_cast<int>(cellSize_), 0, Map::GRID_SIZE - 1);
    return sf::Vector2i(x, y);
}
