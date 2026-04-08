#pragma once

#include <string>
#include <vector>

#include "Point.h"

struct SavedGameData {
    std::vector<Point> snake;
    Point food;
    int direction;
    int score;
    int status;
};

class SaveSystem {
public:
    explicit SaveSystem(const std::string& filePath = "savegame.txt");

    bool save(const SavedGameData& data) const;
    bool load(SavedGameData& data) const;

private:
    std::string filePath;
};
