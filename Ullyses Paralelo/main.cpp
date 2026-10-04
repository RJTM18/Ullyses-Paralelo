#include <chrono>
#include <thread>
#include "Level2State.h"
#include <iostream>
#include <conio.h>

int main() {
    /*
    Level1State nivel;
    nivel.init();
    auto anterior = std::chrono::steady_clock::now();

    while (!nivel.haTerminado()) {
        if (nivel.haPerdido()) {
            std::cout << "\n  GAME OVER - te quedaste sin vidas.\n  Presiona una tecla para salir...";
            _getch();
        }
        const auto ahora = std::chrono::steady_clock::now();
        const float dt = std::chrono::duration<float>(ahora - anterior).count();
        anterior = ahora;

        nivel.update(dt);
        nivel.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    */
    Level2State nivel;

    nivel.init();

}