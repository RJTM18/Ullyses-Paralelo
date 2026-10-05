// jugador.cpp
#include "Jugador.h"

namespace {
    constexpr int TOTAL_FRAMES = 3;

    const char* const SPRITES[2][TOTAL_FRAMES][Jugador::ALTO_SPRITE] = {
        { // derecha, cuando dir es cero
            { "  L", " (|)", " /  L", }, //cada fila es un frame
            { "  L", " (|)", " / L"   },
            { "  L", " (|)", "  L \\" },
        },
        //sprite 2
        { // izquierda, cuando dir es uno
            { "  L", " (|)", " J  \\" },
            { "  L", " (|)", " J /"   },
            { "  L", " (|)", "/  J"   },
        }
    };
}

void Jugador::mover(int dx, int dy) {
    posicionX += dx;
    posicionY += dy;
    if (dx > 0) mirandoDerecha = true;
    else if (dx < 0) mirandoDerecha = false;
    frameActual = (frameActual + 1) % TOTAL_FRAMES;
    //cada ves que el jugador mueve una unidad, frameActual ira alternando entre 0, 1 y 2, coincidiendo con los frames totales por sprite
}

void Jugador::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    const int dir = mirandoDerecha ? 0 : 1; // 0=true, 1=false
    for (int fila = 0; fila < ALTO_SPRITE; ++fila) {
        const int py = posicionY + fila - camY;
        if (py < 0 || py >= (int)pantalla.size()) continue;

        const char* linea = SPRITES[dir][frameActual][fila];
        for (int c = 0; linea[c] != '\0'; ++c) {
            const int px = posicionX + c - camX;
            if (px >= 0 && px < (int)pantalla[py].size())
                pantalla[py][px] = linea[c];
        }
    }
}