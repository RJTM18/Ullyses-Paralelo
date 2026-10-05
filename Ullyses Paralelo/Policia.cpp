#include "Policia.h"

namespace {
    constexpr int TOTAL_FRAMES = 3;

    const char* const SPRITES[2][TOTAL_FRAMES][Policia::ALTO_SPRITE] = {
        { // derecha, cuando dir es cero
            { "  P", " /|)" , " / L"  },  //"  P"   "  P"   "  P"
            { "  P", " /|)" , "  /L"  },  //" /|)"  " /|)"  " (|\\"
            { "  P", " (|\\", "   L\\" }, //" / L"  "  /L"  "   L\\"
        },
        { // izquierda, cuando dir es uno
            { "  P", " (|\\", " L \\"  },
            { "  P", " (|\\", " L/ "  },
            { "  P", " /|)" , "L/  "   },
        }
    };
}

void Policia::mover(int dx, int dy) {
    posicionX += dx;
    posicionY += dy;
    if (dx > 0) mirandoDerecha = true;   // los sprites son de perfil: solo el movimiento horizontal los gira
    else if (dx < 0) mirandoDerecha = false;   // (si solo hay dy, conserva la ultima orientacion)

    frameActual = (frameActual + 1) % TOTAL_FRAMES; //solo valores de 0, 1 y 2
}

void Policia::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    const int dir = mirandoDerecha ? 0 : 1; // 0=derecha, 1=izquierda
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

Caja Policia::getHitbox() const {
    return { posicionX + 1, posicionY, 3, ALTO_SPRITE };   // solo el torso, igual que Jugador
}