#include <chrono>
#include <thread>
#include "Level2State.h"
#include "Input.h"
// #include "Level1State.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>


namespace {
    void correr(IGameState& nivel) {
        nivel.init();
        auto anterior = std::chrono::steady_clock::now();

        while (!nivel.haTerminado()) {
            const auto ahora = std::chrono::steady_clock::now();
            const float dt = std::chrono::duration<float>(ahora - anterior).count();
            anterior = ahora;

            nivel.update(dt);
            nivel.render();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}

int main() {
    std::system("mode con: cols=124 lines=36");   // camino.txt mide 30 filas: que quepa sin hacer scroll

    Level2State nivel;
    correr(nivel);

    // Sin esto la ventana se cierra en cuanto termina el nivel y no se alcanza a leer el resultado
    std::cout << "\n Presiona una tecla para salir...";
    vaciarEntrada();   // por si el jugador seguia pulsando flechas
    _getch();

    // Para probar el nivel 1 con el MISMO bucle:
    //   Level1State nivel1;
    //   correr(nivel1);
}