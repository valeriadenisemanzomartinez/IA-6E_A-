#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "Map.hpp"

namespace AStar
{
std::vector<sf::Vector2i> findPath(const Map& map,
                                  const sf::Vector2i& start,
                                  const sf::Vector2i& goal,
                                  bool carryingVictim);
}
