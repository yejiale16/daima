#include "persistence/SaveSystem.h"

#include <fstream>
#include <sstream>

SaveSystem::SaveSystem(const std::string& file) : filePath(file) {
}

bool SaveSystem::save(const SaveData& data) const {
    std::ofstream out(filePath, std::ios::trunc);
    if (!out.is_open()) {
        return false;
    }

    out << "score=" << data.score << '\n';
    out << "direction=" << data.direction << '\n';
    out << "state=" << data.gameState << '\n';
    out << "food=" << data.food.x << "," << data.food.y << '\n';
    out << "snake=";

    for (size_t i = 0; i < data.snake.size(); ++i) {
        out << data.snake[i].x << "," << data.snake[i].y;
        if (i + 1 < data.snake.size()) {
            out << ';';
        }
    }
    out << '\n';

    return true;
}

bool SaveSystem::load(SaveData& data) const {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        return false;
    }

    SaveData temp;
    temp.score = 0;
    temp.direction = 0;
    temp.gameState = 0;
    temp.food = {1, 1};

    bool hasSnake = false;
    bool hasFood = false;

    std::string line;
    while (std::getline(in, line)) {
        const size_t sep = line.find('=');
        if (sep == std::string::npos) {
            continue;
        }

        const std::string key = line.substr(0, sep);
        const std::string value = line.substr(sep + 1);

        try {
            if (key == "score") {
                temp.score = std::stoi(value);
            } else if (key == "direction") {
                temp.direction = std::stoi(value);
            } else if (key == "state") {
                temp.gameState = std::stoi(value);
            } else if (key == "food") {
                std::stringstream ss(value);
                std::string x;
                std::string y;
                if (std::getline(ss, x, ',') && std::getline(ss, y, ',')) {
                    temp.food = {std::stoi(x), std::stoi(y)};
                    hasFood = true;
                }
            } else if (key == "snake") {
                std::stringstream ss(value);
                std::string segment;
                while (std::getline(ss, segment, ';')) {
                    std::stringstream pointStream(segment);
                    std::string x;
                    std::string y;
                    if (std::getline(pointStream, x, ',') && std::getline(pointStream, y, ',')) {
                        temp.snake.push_back({std::stoi(x), std::stoi(y)});
                    }
                }
                hasSnake = !temp.snake.empty();
            }
        } catch (...) {
            return false;
        }
    }

    if (!hasSnake || !hasFood) {
        return false;
    }

    if (temp.direction < 0 || temp.direction > 3) {
        return false;
    }

    data = temp;
    return true;
}
