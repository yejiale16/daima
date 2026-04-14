#include "gameplay/Food.h"

#include <cstdlib>

Food::Food() : pos{1, 1} {
}

void Food::generate(int width, int height, const std::deque<Point>& snakeBody) {
    // Keep food at least one cell away from walls to avoid edge-visual ambiguity.
    if (width <= 6 || height <= 6) {
        pos = {1, 1};
        return;
    }

    const int minX = 2;
    const int maxX = width - 3;
    const int minY = 2;
    const int maxY = height - 3;

    while (true) {
        Point candidate{minX + (std::rand() % (maxX - minX + 1)), minY + (std::rand() % (maxY - minY + 1))};
        bool blocked = false;
        for (const Point& segment : snakeBody) {
            if (segment == candidate) {
                blocked = true;
                break;
            }
        }

        if (!blocked) {
            pos = candidate;
            return;
        }
    }
}

Point Food::position() const {
    return pos;
}

void Food::setPosition(const Point& p) {
    pos = p;
}
