#include "Level1State.h"
#include "Input.h"
#include "ContinuousCameraStrategy.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>

namespace {
    constexpr int ANCHO_PANTALLA = 120;
    constexpr int ALTO_PANTALLA = 23; 
    constexpr int FILA_CALLE = 11;
    constexpr int ALTO_CARRIL = 3;
    constexpr int PASO_X = 2;
    constexpr float INTERVALO_PASO = 0.08f;   // 80 ms
    constexpr int ANCHO_MUNDO = 709;  
}

Level1State::Level1State() : jugador(5, FILA_CALLE) {}

void Level1State::init() {
    terminado = false;
    acumulador = 0.f;

    if (!fondo.loadFromFile("../Fondo de ciudad Nivel 1.txt")) {
        std::cout << "No se pudo cargar el fondo\n";
        terminado = true;
        return;
    }
    camara = std::make_unique<ContinuousCameraStrategy>(ANCHO_PANTALLA, fondo.getWidth());
}


void Level1State::update(float dt) {
    const EntradaJugador in = leerEntrada();
    if (in.salir) { terminado = true; return; }

    acumulador += dt;
    if (acumulador < INTERVALO_PASO) return;

    int dx = 0, dy = 0;
    if (in.derecha && !in.izquierda)      dx = PASO_X;
    else if (in.izquierda && !in.derecha) dx = -PASO_X;
    if (in.arriba && !in.abajo)           dy = -ALTO_CARRIL;
    else if (in.abajo && !in.arriba)      dy = ALTO_CARRIL;

    // Los límites los decide el nivel, no el jugador
    const int nx = std::clamp(jugador.getX() + dx, 0, ANCHO_MUNDO - Jugador::ANCHO_SPRITE);
    const int ny = jugador.getY() + dy;
    if (ny < FILA_CALLE || ny + Jugador::ALTO_SPRITE > ALTO_PANTALLA) dy = 0;
    dx = nx - jugador.getX();

    if (dx != 0 || dy != 0) {
        jugador.mover(dx, dy);
        camara->updateCamera(jugador.getX(), jugador.getY(), camX, camY);
        acumulador = 0.f;
    }
}

void Level1State::render() {
    std::vector<std::string> pantalla(ALTO_PANTALLA, std::string(ANCHO_PANTALLA, ' '));
    // aquí irá el fondo (paso 7)
    for (int y = 0; y < ALTO_PANTALLA; ++y)
        for (int x = 0; x < ANCHO_PANTALLA; ++x)
            pantalla[y][x] = fondo.getPixel(camX + x, camY + y);

    jugador.dibujar(pantalla, camX, camY);

    std::string salida;
    for (const auto& fila : pantalla) salida += "|" + fila + "|\n";
    system("cls");
    std::cout << salida;
}