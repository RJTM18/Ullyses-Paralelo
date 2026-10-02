#include "Carro.h"
#include <cmath>

namespace { //muchas definiciones bajo el mismo nombre
    const char* const SPRITE[Carro::ALTO_SPRITE] = {
        "   ______   ",
        " _/ /_/  |  ",
        "|__0____0...",
    };
}

void Carro::avanzar(float distancia) {
    xExacta -= distancia;
    posicionX = static_cast<int>(std::floor(xExacta)); //redondea al numero inferior entero mas cercano(floor)
    //luego lo transforma a int durante compilacion
}

void Carro::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
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