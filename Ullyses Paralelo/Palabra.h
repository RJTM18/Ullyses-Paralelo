#pragma once
#include <string>
using namespace std;
class Palabra {
private:
    int x, y;
    string texto;
    bool enemiga;
public:
    Palabra(int xInicial, int yInicial, string textoInicial, bool esEnemiga) {
        x = xInicial; y = yInicial; texto = textoInicial; enemiga = esEnemiga;
    }
    int getX() { return x; } int getY() { return y; } string getTexto() { return texto; }
    bool esEnemiga() { return enemiga; }
    void mover() { if (enemiga) y++; else y--; }
};
