#pragma once

#include <deque>

#include "Point.h"

class Food {
public:
    Food();

    void generate(int width, int height, const std::deque<Point>& snakeBody);
    Point position() const;
    void setPosition(const Point& p);

private:
    Point pos;
};
