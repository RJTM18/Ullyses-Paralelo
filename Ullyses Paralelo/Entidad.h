#pragma once
#include <string>
#include <vector>

class Entidad {
protected:
    int posicionX;
    int posicionY;
public:
    Entidad(int x, int y) : posicionX(x), posicionY(y) {}
    virtual ~Entidad() = default;

    int getX() const { return posicionX; }
    int getY() const { return posicionY; }

    virtual void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const = 0; //buffer
};