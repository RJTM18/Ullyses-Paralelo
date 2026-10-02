#include "Input.h"
#include <conio.h>
#include <cctype>

//uso de booleanos en controles

EntradaJugador leerEntrada() {
    EntradaJugador e;
    if (!_kbhit()) return e;

    int t = _getch();
    if (t == 0 || t == 224) {            // teclas especiales (flechas)
        t = _getch();
        e.derecha = (t == 77);
        e.izquierda = (t == 75);
        e.arriba = (t == 72);
        e.abajo = (t == 80);
    }
    else {
        t = std::toupper(t);
        e.derecha = (t == 'D');
        e.izquierda = (t == 'A');
        e.arriba = (t == 'W');
        e.abajo = (t == 'S');
        e.salir = (t == 'X');
    }
    return e;
}

void vaciarEntrada() {
    while (_kbhit()) _getch();
} //absorbe los inputs