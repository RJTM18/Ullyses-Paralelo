#pragma once
#include <iostream>
#include <string>
using namespace std;
class Enemigo {
protected:
    int x, y;
    int vida;
public:
    Enemigo(int xInicial, int yInicial, int vidaInicial) { x = xInicial; y = yInicial; vida = vidaInicial; }
    virtual ~Enemigo() {}
    int getX() { return x; }
    int getY() { return y; }
    int getVida() { return vida; }
    void recibirGolpe() { if (vida > 0) vida--; }
    virtual void mover() = 0;
    virtual void dibujar() = 0;
    virtual string dialogo() = 0;
};
#pragma once
