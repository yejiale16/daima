#include "Food.h"

#include <cstdlib>

Food::Food() : position{1, 1} {
}

void Food::generate(int width, int height, const std::deque<Point>& snakeBody) {
    if (width <= 2 || height <= 2) {
        position = {1, 1};
        return;
    }

    while (true) {
        Point candidate;
        candidate.x = 1 + (std::rand() % (width - 2));
        candidate.y = 1 + (std::rand() % (height - 2));

        bool overlap = false;
        for (const Point& part : snakeBody) {
            if (part == candidate) {
                overlap = true;
                break;
            }
        }

        if (!overlap) {
            position = candidate;
            return;
        }
    }
}

Point Food::getPosition() const {
    return position;
}

void Food::setPosition(const Point& p) {
    position = p;
}
