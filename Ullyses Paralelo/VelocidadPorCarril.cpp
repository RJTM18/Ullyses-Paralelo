// Carril.cpp
#include "VelocidadPorCarril.h"
#include <random>

namespace {
    constexpr int MARGEN = Carro::ANCHO_SPRITE + 3;   // franja fuera de pantalla donde nacen y mueren

    std::mt19937& generador() {
        static std::mt19937 g{ std::random_device{}() };
        return g;
    }
}

Carril::Carril(int fila, float vel, int gMin, int gMax)
    : filaY(fila), velocidad(vel), gapMin(gMin), gapMax(gMax), gapPendiente(0) {
    gapPendiente = sortearGap();
}

int Carril::sortearGap() const {
    return std::uniform_int_distribution<int>(gapMin, gapMax)(generador());
}

void Carril::precalentar(int desdeX, int hastaX) {
    for (int x = desdeX + sortearGap(); x < hastaX; x += Carro::ANCHO_SPRITE + sortearGap())
        carros.emplace_back(x, filaY);
    gapPendiente = sortearGap();
}

void Carril::actualizar(float dt, int camX, int anchoPantalla) {
    // 1) MOVER: todos comparten la misma velocidad
    const float distancia = velocidad * dt;
    for (auto& c : carros) c.avanzar(distancia);

    // 2) RECICLAR: el más viejo (front) es el primero en salir por la izquierda
    while (!carros.empty() && carros.front().getX() + Carro::ANCHO_SPRITE < camX - MARGEN)
        carros.pop_front();

    // 3) GENERAR: nace fuera de pantalla (derecha) cuando hay espacio libre suficiente
    const int puntoAparicion = camX + anchoPantalla + MARGEN;
    const bool hayEspacio = carros.empty() ||
        puntoAparicion - (carros.back().getX() + Carro::ANCHO_SPRITE) >= gapPendiente;
    if (hayEspacio) {
        carros.emplace_back(puntoAparicion, filaY);
        gapPendiente = sortearGap();
    }
}

void Carril::dibujar(std::vector<std::string>& pantalla, int camX, int camY) const {
    for (const auto& c : carros) c.dibujar(pantalla, camX, camY);
}