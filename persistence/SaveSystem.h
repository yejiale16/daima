#pragma once

#include <string>
#include <vector>

#include "Point.h"

struct SaveData {
    std::vector<Point> snake;
    Point food;
    int direction;
    int score;
    int gameState;
};

class SaveSystem {
public:
    explicit SaveSystem(const std::string& file = "savegame.txt");

    bool save(const SaveData& data) const;
    bool load(SaveData& data) const;

private:
    std::string filePath;
};
