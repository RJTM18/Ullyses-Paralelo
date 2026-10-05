#include "Stephen.h"

namespace {
    constexpr int TOTAL_FRAMES = 3;

    const char* const SPRITES[2][TOTAL_FRAMES][Stephen::ALTO_SPRITE] = {
        { // derecha, cuando dir es cero
            { "  S", " (|)", " /  L"  }, //"  S"
            { "  S", " (|)", " / L"   }, //" (|)"
            { "  S", " (|)", "  L \\" }, //" /  L"
        },
        { // izquierda, cuando dir es uno
            { "  S", " (|)", " J  \\" },
            { "  S", " (|)", " J /"   },
            { "  S", " (|)", "/  J"   },
        }
    };
}

void Stephen::mover(int dx, int dy) {
    posicionX += dx;
    posicionY += dy;
    if (dx > 0) mirandoDerecha = true;
    else if (dx < 0) mirandoDerecha = false;

    frameActual = (frameActual + 1) % TOTAL_FRAMES;
}

void Stephen::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    //camX, camY = 0        

    const int dir = mirandoDerecha ? 0 : 1; // 0=derecha, 1=izquierda
    for (int fila = 0; fila < ALTO_SPRITE; ++fila) {
        const int py = posicionY + fila - camY;
        if (py < 0 || py >= (int)pantalla.size()) continue; //fila cuando esta fuera de rango es ignorado

        const char* linea = SPRITES[dir][frameActual][fila];
        for (int c = 0; linea[c] != '\0'; ++c) { //'\0' final de texto o string
            const int px = posicionX + c - camX;
            if (px >= 0 && px < (int)pantalla[py].size())
                pantalla[py][px] = linea[c]; //linea[c] es un solo caracter, impresion por caracter
        }
    }
}

Caja Stephen::getHitbox() const {
    return { posicionX + 1, posicionY, 3, ALTO_SPRITE };   // solo el torso, igual que Jugador
}