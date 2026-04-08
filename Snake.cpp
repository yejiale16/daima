#include "Snake.h"

Snake::Snake() : direction(Direction::Up) {
}

void Snake::reset(int startX, int startY) {
    body.clear();
    body.push_back({startX, startY});
    body.push_back({startX, startY + 1});
    body.push_back({startX, startY + 2});
    body.push_back({startX, startY + 3});
    direction = Direction::Up;
}

void Snake::setDirection(Direction newDirection) {
    if (!isOpposite(direction, newDirection)) {
        direction = newDirection;
    }
}

void Snake::move(bool grow) {
    Point next = body.front();

    switch (direction) {
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

    body.push_front(next);
    if (!grow) {
        body.pop_back();
    }
}

bool Snake::occupies(const Point& p) const {
    for (const Point& part : body) {
        if (part == p) {
            return true;
        }
    }
    return false;
}

bool Snake::isSelfCollision() const {
    if (body.empty()) {
        return false;
    }

    const Point& head = body.front();
    for (size_t i = 1; i < body.size(); ++i) {
        if (head == body[i]) {
            return true;
        }
    }
    return false;
}

const std::deque<Point>& Snake::getBody() const {
    return body;
}

Direction Snake::getDirection() const {
    return direction;
}

Point Snake::getHead() const {
    return body.front();
}

void Snake::setBody(const std::deque<Point>& newBody) {
    body = newBody;
}

void Snake::setDirectionDirect(Direction dir) {
    direction = dir;
}

bool Snake::isOpposite(Direction a, Direction b) const {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}
