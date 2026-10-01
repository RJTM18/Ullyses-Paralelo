#include <chrono>
#include <thread>
#include "Level1State.h"

int main() {
    Level1State nivel;
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