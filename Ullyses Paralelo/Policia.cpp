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
    if (dy > 0) mirandoAbajo = true; //si va abajo, y aumenta, 
    //mientras el jugador avance arriba, el policia ira en el mov opuesto.
    else if (dy < 0) mirandoAbajo = false; //porque mira hacia arriba

    frameActual = (frameActual + 1) % TOTAL_FRAMES; //solo valores de 0, 1 y 2
}

void Policia::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    const int dir = mirandoAbajo ? 0 : 1; // 0=true, 1=false
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
};

Caja Policia::getHitbox() const {

};