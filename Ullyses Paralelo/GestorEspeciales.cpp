#include "GestorEspeciales.h"
#include <cstdlib>
#include <random>

namespace {
    constexpr int    MARGEN = SpecialEnemy::ANCHO_SPRITE + 3;
    constexpr float  ESPERA_MIN = 2.0f, ESPERA_MAX = 5.0f;   // segundos entre apariciones
    constexpr size_t MAX_SIMULTANEOS = 3;
    constexpr int    SEPARACION = 10;

    std::mt19937& generador() {
        static std::mt19937 g{ std::random_device{}() };
        return g;
    }

    bool hayLugar(const std::vector<SpecialEnemy>& v, int fila, int x) {
        for (const auto& e : v)
            if (e.getY() == fila && std::abs(e.getX() - x) < SpecialEnemy::ANCHO_SPRITE + SEPARACION)
                return false;
        return true;
    }
}

GestorEspeciales::GestorEspeciales(float vel, int fila, int n, int alto)
    : velocidad(vel), filaBase(fila), numCarriles(n), altoCarril(alto), temporizador(0.f) {
    temporizador = sortearEspera();
}

float GestorEspeciales::sortearEspera() const {
    return std::uniform_real_distribution<float>(ESPERA_MIN, ESPERA_MAX)(generador());
}

int GestorEspeciales::sortearFila() const {
    const int carril = std::uniform_int_distribution<int>(0, numCarriles - 1)(generador());
    return filaBase + carril * altoCarril;
}

void GestorEspeciales::reiniciar() {
    enemigos.clear();
    temporizador = sortearEspera();
}

void GestorEspeciales::actualizar(float dt, int camX, int anchoPantalla) {
    // 1) MOVER: todos a la misma velocidad
    const float distancia = velocidad * dt;
    for (auto& e : enemigos) e.avanzar(distancia);

    // 2) ELIMINAR los que ya salieron por la izquierda
    std::erase_if(enemigos, [&](const SpecialEnemy& e) {
        return e.getX() + SpecialEnemy::ANCHO_SPRITE < camX - MARGEN;
        });

    // 3) APARECER cuando la cuenta regresiva llega a 0
    temporizador -= dt;
    if (temporizador > 0.f) return;
    temporizador = sortearEspera();
    if (enemigos.size() >= MAX_SIMULTANEOS) return;

    const int x = camX + anchoPantalla + MARGEN;
    const int y = sortearFila();
    if (hayLugar(enemigos, y, x)) enemigos.emplace_back(x, y);
}

void GestorEspeciales::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    for (const auto& e : enemigos) e.dibujar(pantalla, camX, camY);
}