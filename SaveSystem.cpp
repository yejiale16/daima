#include "SaveSystem.h"

#include <fstream>
#include <sstream>

SaveSystem::SaveSystem(const std::string& filePath) : filePath(filePath) {
}

bool SaveSystem::save(const SavedGameData& data) const {
    std::ofstream out(filePath, std::ios::trunc);
    if (!out.is_open()) {
        return false;
    }

    out << "score=" << data.score << '\n';
    out << "direction=" << data.direction << '\n';
    out << "status=" << data.status << '\n';
    out << "food=" << data.food.x << "," << data.food.y << '\n';
    out << "snake=";

    for (size_t i = 0; i < data.snake.size(); ++i) {
        out << data.snake[i].x << "," << data.snake[i].y;
        if (i + 1 < data.snake.size()) {
            out << ";";
        }
    }
    out << '\n';

    return true;
}

bool SaveSystem::load(SavedGameData& data) const {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        return false;
    }

    SavedGameData temp;
    temp.score = 0;
    temp.direction = 0;
    temp.status = 0;
    temp.food = {1, 1};

    std::string line;
    while (std::getline(in, line)) {
        const size_t pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }

        const std::string key = line.substr(0, pos);
        const std::string value = line.substr(pos + 1);

        if (key == "score") {
            temp.score = std::stoi(value);
        } else if (key == "direction") {
            temp.direction = std::stoi(value);
        } else if (key == "status") {
            temp.status = std::stoi(value);
        } else if (key == "food") {
            std::stringstream ss(value);
            std::string xStr;
            std::string yStr;
            if (std::getline(ss, xStr, ',') && std::getline(ss, yStr, ',')) {
                temp.food.x = std::stoi(xStr);
                temp.food.y = std::stoi(yStr);
            }
        } else if (key == "snake") {
            std::stringstream ss(value);
            std::string segment;
            while (std::getline(ss, segment, ';')) {
                std::stringstream coord(segment);
                std::string xStr;
                std::string yStr;
                if (std::getline(coord, xStr, ',') && std::getline(coord, yStr, ',')) {
                    temp.snake.push_back({std::stoi(xStr), std::stoi(yStr)});
                }
            }
        }
    }

    if (temp.snake.empty()) {
        return false;
    }

    data = temp;
    return true;
}
