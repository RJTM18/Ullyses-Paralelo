#pragma once
#include <string>
#include <vector>

struct Caja { int x, y, ancho, alto; };   // rectángulo en coordenadas de MUNDO

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
    virtual Caja getHitbox() const = 0; // la "zona que duele"

    bool colisionaCon(const Entidad& otra) const {
        const Caja a = getHitbox(), b = otra.getHitbox();
        return a.x < b.x + b.ancho && b.x < a.x + a.ancho &&
            a.y < b.y + b.alto && b.y < a.y + a.alto;
    }
};