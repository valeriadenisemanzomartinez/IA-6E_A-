#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Cell.hpp"

class Map
{
public:
    static constexpr int GRID_SIZE = 20;

    Map();

    void resetDefaultLayout();
    bool inBounds(int x, int y) const;
    TerrainType terrainAt(int x, int y) const;
    void setTerrain(int x, int y, TerrainType type);
    void toggleTerrain(int x, int y);
    void spreadFireRandomly();
    float costAt(int x, int y, bool carryingVictim) const;
    bool isWall(int x, int y) const;
    bool isFire(int x, int y) const;
    bool isBlockedForPath(int x, int y) const;
    sf::Color colorAt(int x, int y) const;

    const std::vector<std::vector<Cell>>& grid() const;

private:
    std::vector<std::vector<Cell>> grid_;
};
