#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include "Consola.h"

class Personaje {
protected:
    int x, y;
    int vidas;
public:
    Personaje(int xInicial, int yInicial, int vidasIniciales)     {
        x = xInicial; y = yInicial; vidas = vidasIniciales;
    }
    virtual ~Personaje() {}
    
    int getX() { return x; }
    int getY() { return y; }
    int getVidas() { return vidas; }
    void perderVida() { if (vidas > 0) vidas--; }
    
    virtual void mover(char tecla) {
        if (tecla == 'A' && x > 3) x -= 2;
        if (tecla == 'D' && x < 72) x += 2;
        if (tecla == 'W' && y > 5) y--;
        if (tecla == 'S' && y < 19) y++;
    }

    virtual void dibujar() = 0;
};
