// jugador.cpp
#include "Jugador.h"

namespace {
    constexpr int TOTAL_FRAMES = 3;

    // SPRITES[direccion][frame][fila]   direccion: 0 = derecha, 1 = izquierda
    const char* const SPRITES[2][TOTAL_FRAMES][Jugador::ALTO_SPRITE] = {
        { // derecha
            { "  L", " (|)", " /  L"  },
            { "  L", " (|)", " / L"   },
            { "  L", " (|)", "  L \\" },
        },
        { // izquierda
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
}

void Jugador::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    const int dir = mirandoDerecha ? 0 : 1;
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