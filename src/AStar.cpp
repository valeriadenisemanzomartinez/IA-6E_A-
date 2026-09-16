#include "AStar.hpp"

#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>

namespace
{
struct NodeComparator
{
    bool operator()(const std::pair<float, sf::Vector2i>& lhs,
                    const std::pair<float, sf::Vector2i>& rhs) const
    {
        return lhs.first > rhs.first;
    }
};
}

std::vector<sf::Vector2i> AStar::findPath(const Map& map,
                                         const sf::Vector2i& start,
                                         const sf::Vector2i& goal,
                                         bool carryingVictim)
{
    if (!map.inBounds(start.x, start.y) || !map.inBounds(goal.x, goal.y))
    {
        return {};
    }

    if (start == goal)
    {
        return {start};
    }

    std::priority_queue<std::pair<float, sf::Vector2i>, std::vector<std::pair<float, sf::Vector2i>>, NodeComparator> openSet;
    std::unordered_map<int, float> gScore;
    std::unordered_map<int, sf::Vector2i> cameFrom;
    std::unordered_map<int, float> fScore;

    const auto key = [](const sf::Vector2i& p) {
        return (p.y * Map::GRID_SIZE + p.x);
    };

    openSet.push({0.0f, start});
    gScore[key(start)] = 0.0f;
    fScore[key(start)] = std::abs(start.x - goal.x) + std::abs(start.y - goal.y);

    while (!openSet.empty())
    {
        auto [_, current] = openSet.top();
        openSet.pop();

        if (current == goal)
        {
            std::vector<sf::Vector2i> path;
            sf::Vector2i cursor = current;
            while (!(cursor == start))
            {
                path.push_back(cursor);
                auto it = cameFrom.find(key(cursor));
                if (it == cameFrom.end())
                {
                    break;
                }
                cursor = it->second;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int oy = -1; oy <= 1; ++oy)
        {
            for (int ox = -1; ox <= 1; ++ox)
            {
                if (ox == 0 && oy == 0)
                {
                    continue;
                }
                if (std::abs(ox) + std::abs(oy) > 1)
                {
                    continue;
                }

                sf::Vector2i next(current.x + ox, current.y + oy);
                if (map.isBlockedForPath(next.x, next.y))
                {
                    continue;
                }

                float stepCost = map.costAt(next.x, next.y, carryingVictim);
                if (!std::isfinite(stepCost))
                {
                    continue;
                }

                float tentative = gScore[key(current)] + stepCost;
                int nextKey = key(next);
                if (!gScore.count(nextKey) || tentative < gScore[nextKey])
                {
                    cameFrom[nextKey] = current;
                    gScore[nextKey] = tentative;
                    float heuristic = std::abs(next.x - goal.x) + std::abs(next.y - goal.y);
                    fScore[nextKey] = tentative + heuristic;
                    openSet.push({fScore[nextKey], next});
                }
            }
        }
    }

    return {};
}
