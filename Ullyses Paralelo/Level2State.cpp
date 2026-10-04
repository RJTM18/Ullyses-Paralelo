#include "Level2State.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "Input.h"

//todas las funciones declaradas en una clase aqui deben ser definidas 

namespace {
    struct Punto { int x, y; };

    const char* const RUTA_FONDO = "camino.txt";

    // La calle dibujada en camino.txt: columnas 30..88 y filas 1..28 (59 x 28).
    // Es el unico lugar donde "vive" el conocimiento de donde estan las paredes.
    constexpr Caja ZONA_CALLE = { 30, 1, 59, 28 };   // x, y, ancho, alto
    constexpr int  FILA_META = ZONA_CALLE.y;         // Fase 3: Leopold llega aqui -> siguiente tramo

    // Puntos de salida (esquina superior izquierda de cada sprite)
    constexpr Punto INICIO_LEOPOLD = { 55, 26 };
    constexpr Punto INICIO_STEPHEN = { 63, 26 };
    constexpr Punto INICIO_POLICIA = { 59,  1 };

    constexpr int   VIDAS_INICIALES = 1;      // un solo toque y te capturan (Fase 3)
    constexpr int   PASO_X = 2;
    constexpr int   PASO_Y = 1;               // un caracter es ~2 veces mas alto que ancho: 1 fila ≈ 2 columnas
    constexpr float INTERVALO_PASO = 0.08f;   // 80 ms, igual que el nivel 1
}

Level2State::Level2State()
    : jugador(INICIO_LEOPOLD.x, INICIO_LEOPOLD.y, VIDAS_INICIALES),
    stephen(INICIO_STEPHEN.x, INICIO_STEPHEN.y),
    policia(INICIO_POLICIA.x, INICIO_POLICIA.y) {
}

void Level2State::init() {
    nivelTerminado = false;
    acumulador = 0.f;

    if (!fondo.loadFromFile(RUTA_FONDO)) {
        std::cout << "No se pudo cargar el fondo: " << RUTA_FONDO << "\n";
        nivelTerminado = true;
        return;
    }

    // Misma estrategia de camara que usa el nivel 1, pero estatica: render() no sabe cual es.
    camara = std::make_unique<StaticCameraStrategy>(fondo.getWidth(), fondo.getWidth());
    camara->updateCamera(jugador.getX(), jugador.getY(), camX, camY);
}

void Level2State::update(float dt) {
    dt = std::min(dt, 0.1f);
    const EntradaJugador in = leerEntrada();
    if (in.salir) { nivelTerminado = true; return; }

    acumulador += dt;
    if (acumulador < INTERVALO_PASO) return;

    // Lo que Leopold QUIERE hacer
    int dx = 0, dy = 0;
    if (in.derecha && !in.izquierda)      dx = PASO_X;
    else if (in.izquierda && !in.derecha) dx = -PASO_X;
    if (in.arriba && !in.abajo)           dy = -PASO_Y;
    else if (in.abajo && !in.arriba)      dy = PASO_Y;

    // Lo que el NIVEL le permite: Leopold no sabe donde estan las paredes, el nivel si.
    const int nx = std::clamp(jugador.getX() + dx,
        ZONA_CALLE.x, ZONA_CALLE.x + ZONA_CALLE.ancho - Jugador::ANCHO_SPRITE);
    const int ny = std::clamp(jugador.getY() + dy,
        ZONA_CALLE.y, ZONA_CALLE.y + ZONA_CALLE.alto - Jugador::ALTO_SPRITE);
    dx = nx - jugador.getX();
    dy = ny - jugador.getY();

    if (dx != 0 || dy != 0) {
        jugador.mover(dx, dy);
        acumulador = 0.f;
        // Fase 2: aqui Stephen y los policias tambien reaccionan a la ENTRADA (no a dx, dy)
    }
}

void Level2State::render() {
    const int ancho = fondo.getWidth();
    const int alto = fondo.getHeight();

    // 1) fondo
    std::vector<std::string> pantalla(alto, std::string(ancho, ' '));
    for (int y = 0; y < alto; ++y)
        for (int x = 0; x < ancho; ++x)
            pantalla[y][x] = fondo.getPixel(camX + x, camY + y);

    // 2) actores (el ultimo que se dibuja queda por encima)
    policia.dibujar(pantalla, camX, camY);
    stephen.dibujar(pantalla, camX, camY);
    jugador.dibujar(pantalla, camX, camY);

    // 3) a texto, todo de una vez
    std::string salida = " NIVEL 2 - Llega a la parte superior de la calle!   (X = salir)\n";
    for (const auto& fila : pantalla) salida += "|" + fila + "|\n";

    system("cls");
    std::cout << salida;
}