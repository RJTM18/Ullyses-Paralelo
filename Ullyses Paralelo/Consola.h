#pragma once
#include <windows.h>

inline void moverCursor(int x, int y) {
    COORD posicion;
    posicion.X = static_cast<SHORT>(x);
    posicion.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicion);
}
#pragma once
