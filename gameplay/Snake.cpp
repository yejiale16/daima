#include "gameplay/Snake.h"

Snake::Snake() : dir(Direction::Up) {
}

void Snake::reset(int startX, int startY) {
    bodyData.clear();
    bodyData.push_back({startX, startY});
    bodyData.push_back({startX, startY + 1});
    bodyData.push_back({startX, startY + 2});
    bodyData.push_back({startX, startY + 3});
    dir = Direction::Up;
}

void Snake::setDirection(Direction newDirection) {
    if (!isOpposite(dir, newDirection)) {
        dir = newDirection;
    }
}

void Snake::move(bool grow) {
    Point next = bodyData.front();
    switch (dir) {
        case Direction::Up:
            --next.y;
            break;
        case Direction::Right:
            ++next.x;
            break;
        case Direction::Down:
            ++next.y;
            break;
        case Direction::Left:
            --next.x;
            break;
    }

    bodyData.push_front(next);
    if (!grow) {
        bodyData.pop_back();
    }
}

bool Snake::occupies(const Point& p) const {
    for (const Point& segment : bodyData) {
        if (segment == p) {
            return true;
        }
    }
    return false;
}

const std::deque<Point>& Snake::body() const {
    return bodyData;
}

Direction Snake::direction() const {
    return dir;
}

Point Snake::head() const {
    return bodyData.front();
}

void Snake::setBody(const std::deque<Point>& newBody) {
    bodyData = newBody;
}

void Snake::setDirectionDirect(Direction newDirection) {
    dir = newDirection;
}

bool Snake::isOpposite(Direction a, Direction b) {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}
