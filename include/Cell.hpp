#pragma once

#include <SFML/Graphics.hpp>
#include <limits>

enum class TerrainType
{
    Clean,
    Debris,
    Fire,
    Wall
};

struct Cell
{
    TerrainType terrain = TerrainType::Clean;

    static float movementCost(TerrainType terrainType, bool carryingVictim)
    {
        float base = 1.0f;
        switch (terrainType)
        {
            case TerrainType::Clean:
                base = 1.0f;
                break;
            case TerrainType::Debris:
                base = 3.0f;
                break;
            case TerrainType::Fire:
                base = 10.0f;
                break;
            case TerrainType::Wall:
                base = std::numeric_limits<float>::infinity();
                break;
        }

        if (carryingVictim)
        {
            base *= 2.0f;
        }

        return base;
    }

    static sf::Color colorFor(TerrainType terrainType)
    {
        switch (terrainType)
        {
            case TerrainType::Clean:
                return sf::Color(220, 220, 220);
            case TerrainType::Debris:
                return sf::Color(140, 90, 50);
            case TerrainType::Fire:
                return sf::Color(255, 40, 40);
            case TerrainType::Wall:
                return sf::Color(70, 70, 70);
        }
        return sf::Color::White;
    }
};
