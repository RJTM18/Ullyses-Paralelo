#pragma once
#include "Personaje.h"
#include <windows.h>
class Molly : public Personaje {
private:
    int frame;
public:
    Molly(int xInicial, int yInicial) : Personaje(xInicial, yInicial, 5) { frame = 0; }
    void mover(char tecla) override { Personaje::mover(tecla); frame = (frame + 1) % 3; }
    void dibujar() override {
        const char* pies[3] = { " / \\", " /|  ", "  |\\" };
        moverCursor(x, y); std::cout << "/////";
        moverCursor(x, y + 1); std::cout << " | | ";
        moverCursor(x, y + 2); std::cout << pies[frame];
    }
};
