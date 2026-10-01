// Level1State.cpp
#include "Level1State.h"
#include "Input.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace {
    constexpr int ANCHO_PANTALLA = 90;
    constexpr int ALTO_PANTALLA = 18;   // 6 filas de fondo + 12 de calle (como antes)
    constexpr int FILA_CALLE = 6;
    constexpr int ALTO_CARRIL = 3;
    constexpr int PASO_X = 2;
    constexpr float INTERVALO_PASO = 0.08f;   // 80 ms
    constexpr int ANCHO_MUNDO = ANCHO_PANTALLA;   // provisional: se reemplaza en el paso 7
}

Level1State::Level1State() : jugador(5, FILA_CALLE) {}

void Level1State::init() {
    terminado = false;
    acumulador = 0.f;
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
        acumulador = 0.f;
    }
}

void Level1State::render() {
    std::vector<std::string> pantalla(ALTO_PANTALLA, std::string(ANCHO_PANTALLA, ' '));
    // aquí irá el fondo (paso 7)
    jugador.dibujar(pantalla, camX, camY);

    std::string salida;
    for (const auto& fila : pantalla) salida += "|" + fila + "|\n";
    system("cls");
    std::cout << salida;
}