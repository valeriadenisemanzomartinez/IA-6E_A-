#include "Rescuer.hpp"

#include <algorithm>
#include <cmath>

Rescuer::Rescuer()
    : body_(8.0f),
      position_(0.5f, 0.5f),
      currentCell_(1, 1),
      speed_(7.0f)
{
    body_.setFillColor(sf::Color::Blue);
    body_.setOrigin(8.0f, 8.0f);
}

void Rescuer::setPositionCell(int x, int y)
{
    currentCell_ = sf::Vector2i(x, y);
    position_ = sf::Vector2f(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
}

sf::Vector2i Rescuer::currentCell() const
{
    return currentCell_;
}

sf::Vector2f Rescuer::position() const
{
    return position_;
}

void Rescuer::setPath(const std::vector<sf::Vector2i>& path)
{
    path_.clear();
    for (const auto& cell : path)
    {
        if (cell == currentCell_)
        {
            continue;
        }
        path_.push_back(cell);
    }
}

void Rescuer::clearPath()
{
    path_.clear();
}

bool Rescuer::hasPath() const
{
    return !path_.empty();
}

void Rescuer::update(float dt)
{
    if (path_.empty())
    {
        return;
    }

    const sf::Vector2f target(static_cast<float>(path_.front().x) + 0.5f,
                             static_cast<float>(path_.front().y) + 0.5f);
    const sf::Vector2f delta = target - position_;
    const float distance = std::hypot(delta.x, delta.y);

    if (distance <= 0.05f)
    {
        position_ = target;
        currentCell_ = path_.front();
        path_.pop_front();
        return;
    }

    const float step = std::min(speed_ * dt, distance);
    if (distance > 0.0f)
    {
        position_ += (delta / distance) * step;
    }
    currentCell_ = sf::Vector2i(static_cast<int>(std::floor(position_.x)),
                               static_cast<int>(std::floor(position_.y)));
}

void Rescuer::draw(sf::RenderTarget& target)
{
    body_.setPosition(position_.x * 32.0f, position_.y * 32.0f);
    target.draw(body_);
}
