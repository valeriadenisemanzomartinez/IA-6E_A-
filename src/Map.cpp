#include "Map.hpp"

#include <limits>
#include <random>

Map::Map()
{
    grid_.resize(GRID_SIZE, std::vector<Cell>(GRID_SIZE));
    resetDefaultLayout();
}

void Map::resetDefaultLayout()
{
    for (int y = 0; y < GRID_SIZE; ++y)
    {
        for (int x = 0; x < GRID_SIZE; ++x)
        {
            grid_[y][x].terrain = TerrainType::Clean;
        }
    }

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        grid_[0][i].terrain = TerrainType::Wall;
        grid_[GRID_SIZE - 1][i].terrain = TerrainType::Wall;
        grid_[i][0].terrain = TerrainType::Wall;
        grid_[i][GRID_SIZE - 1].terrain = TerrainType::Wall;
    }

    for (int y = 2; y < GRID_SIZE - 2; ++y)
    {
        if (y % 4 == 0)
        {
            for (int x = 5; x < 15; ++x)
            {
                if (x % 3 != 0)
                {
                    grid_[y][x].terrain = TerrainType::Debris;
                }
            }
        }
    }

    for (int i = 0; i < 12; ++i)
    {
        int x = 2 + (i * 2) % 14;
        int y = 2 + (i * 3) % 14;
        if (x > 0 && y > 0 && x < GRID_SIZE - 1 && y < GRID_SIZE - 1)
        {
            grid_[y][x].terrain = TerrainType::Fire;
        }
    }
}

bool Map::inBounds(int x, int y) const
{
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

TerrainType Map::terrainAt(int x, int y) const
{
    if (!inBounds(x, y))
    {
        return TerrainType::Wall;
    }
    return grid_[y][x].terrain;
}

void Map::setTerrain(int x, int y, TerrainType type)
{
    if (inBounds(x, y))
    {
        grid_[y][x].terrain = type;
    }
}

void Map::toggleTerrain(int x, int y)
{
    if (!inBounds(x, y))
    {
        return;
    }

    switch (grid_[y][x].terrain)
    {
        case TerrainType::Clean:
            grid_[y][x].terrain = TerrainType::Fire;
            break;
        case TerrainType::Fire:
            grid_[y][x].terrain = TerrainType::Wall;
            break;
        case TerrainType::Wall:
            grid_[y][x].terrain = TerrainType::Clean;
            break;
        case TerrainType::Debris:
            grid_[y][x].terrain = TerrainType::Fire;
            break;
    }
}

void Map::spreadFireRandomly()
{
    std::vector<sf::Vector2i> candidates;
    for (int y = 1; y < GRID_SIZE - 1; ++y)
    {
        for (int x = 1; x < GRID_SIZE - 1; ++x)
        {
            if (grid_[y][x].terrain == TerrainType::Fire)
            {
                for (int oy = -1; oy <= 1; ++oy)
                {
                    for (int ox = -1; ox <= 1; ++ox)
                    {
                        if (ox == 0 && oy == 0)
                        {
                            continue;
                        }
                        const int nx = x + ox;
                        const int ny = y + oy;
                        if (inBounds(nx, ny) && grid_[ny][nx].terrain != TerrainType::Wall && grid_[ny][nx].terrain != TerrainType::Fire)
                        {
                            candidates.push_back(sf::Vector2i(nx, ny));
                        }
                    }
                }
            }
        }
    }

    if (candidates.empty())
    {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(candidates.size()) - 1);
    const auto tile = candidates[dist(gen)];
    grid_[tile.y][tile.x].terrain = TerrainType::Fire;
}

float Map::costAt(int x, int y, bool carryingVictim) const
{
    if (!inBounds(x, y))
    {
        return std::numeric_limits<float>::infinity();
    }
    return Cell::movementCost(grid_[y][x].terrain, carryingVictim);
}

bool Map::isWall(int x, int y) const
{
    return inBounds(x, y) && grid_[y][x].terrain == TerrainType::Wall;
}

bool Map::isFire(int x, int y) const
{
    return inBounds(x, y) && grid_[y][x].terrain == TerrainType::Fire;
}

bool Map::isBlockedForPath(int x, int y) const
{
    return !inBounds(x, y) || grid_[y][x].terrain == TerrainType::Wall;
}

sf::Color Map::colorAt(int x, int y) const
{
    if (!inBounds(x, y))
    {
        return sf::Color::Transparent;
    }
    return Cell::colorFor(grid_[y][x].terrain);
}

const std::vector<std::vector<Cell>>& Map::grid() const
{
    return grid_;
}
