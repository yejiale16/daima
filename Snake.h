#pragma once

#include <deque>
#include "Point.h"

enum class Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

class Snake {
public:
    Snake();

    void reset(int startX, int startY);
    void setDirection(Direction newDirection);
    void move(bool grow);

    bool occupies(const Point& p) const;
    bool isSelfCollision() const;

    const std::deque<Point>& getBody() const;
    Direction getDirection() const;
    Point getHead() const;

    void setBody(const std::deque<Point>& newBody);
    void setDirectionDirect(Direction dir);

private:
    std::deque<Point> body;
    Direction direction;

    bool isOpposite(Direction a, Direction b) const;
};
