// Carro.cpp
#include "SpecialEnemy.h"
#include <cmath>

namespace {
    const char* const SPRITE[SpecialEnemy::ALTO_SPRITE] = {
        "  \\_X_/  ",
        "   [###]  ",
        "   _/ \\_ ",
    };
}

void SpecialEnemy::avanzar(float distancia) {
    xExacta -= distancia;
    posicionX = static_cast<int>(std::floor(xExacta));
}

void SpecialEnemy::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    for (int fila = 0; fila < ALTO_SPRITE; ++fila) {
        const int py = posicionY + fila - camY;
        if (py < 0 || py >= (int)pantalla.size()) continue;
        for (int c = 0; c < ANCHO_SPRITE; ++c) {
            const int px = posicionX + c - camX;
            if (px >= 0 && px < (int)pantalla[py].size())
                pantalla[py][px] = SPRITE[fila][c];
        }
    }
}