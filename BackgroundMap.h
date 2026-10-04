#pragma once
#include <vector>
#include <string>
#include <fstream> //para leer .txt
#include <algorithm>

class BackgroundMap {
private:
    std::vector<std::string> grid;
    int width = 0, height = 0;
public:
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    bool loadFromFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) return false;

        grid.clear();
        std::string line;
        std::size_t ancho = 0;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            ancho = std::max(ancho, line.size());
            grid.push_back(line);
        }
        if (grid.empty()) return false;

        for (auto& fila : grid) fila.resize(ancho, ' ');
        height = static_cast<int>(grid.size());
        width = static_cast<int>(ancho);
        return true;
    }

    char getPixel(int worldX, int worldY) const {
        if (worldY < 0 || worldY >= height || worldX < 0 || worldX >= width) {
            return ' '; // Out of bounds fallback
        }
        return grid[worldY][worldX];
    }
};