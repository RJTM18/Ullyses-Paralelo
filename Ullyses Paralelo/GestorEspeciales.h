#pragma once
#include "SpecialEnemy.h"
#include <algorithm>
#include <vector>

class GestorEspeciales {
public:
    GestorEspeciales(float velocidad, int filaBase, int numCarriles, int altoCarril);

    void  setVelocidad(float colPorSeg) { velocidad = std::max(0.f, colPorSeg); }   // palanca ÚNICA
    float getVelocidad() const { return velocidad; }

    void reiniciar();
    void actualizar(float dt, int camX, int anchoPantalla);
    void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const;

    const std::vector<SpecialEnemy>& getEnemigos() const { return enemigos; }   // para colisiones

private:
    float sortearEspera() const;
    int   sortearFila() const;

    float velocidad;
    int   filaBase, numCarriles, altoCarril;
    float temporizador;
    std::vector<SpecialEnemy> enemigos;
};
