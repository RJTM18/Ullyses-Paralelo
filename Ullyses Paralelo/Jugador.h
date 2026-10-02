// Jugador.h
#pragma once
#include "Entidad.h"

class Jugador : public Entidad {
public:
    static constexpr int ANCHO_SPRITE = 5;
    static constexpr int ALTO_SPRITE = 3;

    Jugador(int x, int y, int vidasIniciales) : Entidad(x, y), vidas(vidasIniciales) {}

    int  getVidas() const { return vidas; }
    void perderVida() { if (vidas > 0) --vidas; }
    bool estaVivo() const { return vidas > 0; } 

    void mover(int dx, int dy);
    void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const override;

    Caja getHitbox() const override { return { posicionX + 1, posicionY, 3, ALTO_SPRITE }; }  // solo el torso "(|)"

private:
    bool mirandoDerecha = true;
    int  frameActual = 0;
    int vidas;
};