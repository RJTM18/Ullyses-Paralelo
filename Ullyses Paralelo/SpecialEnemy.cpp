#include "SpecialEnemy.h"
#include <cmath>

namespace { //con animacion
    constexpr int FRAMES = 2;
    constexpr int COLUMNAS_POR_FRAME = 3;   // cambia de pose cada 3 columnas recorridas

    const char* const SPRITES[FRAMES][SpecialEnemy::ALTO_SPRITE] = {
        { "   O ", "==/|\\", "  / \\" },   // piernas separadas
        { "   O ", "==/|\\", "   | " },   // piernas juntas
    };
}

void SpecialEnemy::avanzar(float distancia) {
    xExacta -= distancia;
    posicionX = static_cast<int>(std::floor(xExacta));
}

void SpecialEnemy::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    const int frame = (posicionX / COLUMNAS_POR_FRAME) & 1;
    for (int fila = 0; fila < ALTO_SPRITE; ++fila) {
        const int py = posicionY + fila - camY;
        if (py < 0 || py >= (int)pantalla.size()) continue;
        for (int c = 0; c < ANCHO_SPRITE; ++c) {
            const int px = posicionX + c - camX;
            if (px >= 0 && px < (int)pantalla[py].size())
                pantalla[py][px] = SPRITES[frame][fila][c];
        }
    }
}