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
    if (dy < 0) mirandoArriba = true; //si va arriba, y se reduce
    else if (dy > 0) mirandoArriba = false;
}

void Stephen::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    //nivel 2 camara estatica: camX, camY = 0        
        
    const int dir = mirandoArriba ? 0 : 1; // 0=true, 1=false
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
};

Caja Stephen::getHitbox() const {
    
};