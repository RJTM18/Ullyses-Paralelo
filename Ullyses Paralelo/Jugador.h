// Jugador.h
#pragma once
#include "Entidad.h"

class Jugador : public Entidad {
public:
    static constexpr int ANCHO_SPRITE = 5;
    static constexpr int ALTO_SPRITE = 3;

    Jugador(int x, int y) : Entidad(x, y) {}

    void mover(int dx, int dy);
    void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const override;

private:
    bool mirandoDerecha = true;
    int  frameActual = 0;
};