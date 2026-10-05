#pragma once
#include "Consola.h"
#include "Enemigo.h"
#include <windows.h>
class Pensamiento : public Enemigo {
private:
    int direccion;
public:
    Pensamiento(int xInicial, int yInicial) : Enemigo(xInicial, yInicial, 9) { direccion = 1; }
    void mover() override { x += direccion; if (x > 44 || x < 20) direccion *= -1; }
    void dibujar() override {
        moverCursor(x, y);   cout << "       DUDA       ";
        moverCursor(x, y + 1); cout << " PASADO     BOYLAN ";
        moverCursor(x, y + 2); cout << "BLOOM  RECUERDO  MILLY";
        moverCursor(x, y + 3); cout << " GIBRALTAR   HOWTH ";
    }
    string dialogo() override { return "Los recuerdos de Molly se mezclan en su pensamiento."; }
};
#pragma once
