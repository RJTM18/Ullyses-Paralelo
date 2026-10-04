#include "Level1State.h"
#include "Input.h"
#include "ContinuousCameraStrategy.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>

//atributos de nivel, palancas
namespace {
    constexpr int ANCHO_PANTALLA = 120;
    constexpr int ALTO_PANTALLA = 23; 
    constexpr int FILA_CALLE = 11;
    constexpr int ALTO_CARRIL = 3;
    constexpr int PASO_X = 2;
    constexpr float INTERVALO_PASO = 0.08f;   // 80 ms
    constexpr int ANCHO_MUNDO = 709;  

    constexpr int NUM_CARRILES = 4;
    // ★ LA PALANCA (diseño): columnas/segundo por carril. [0] = el más cercano a la acera
    constexpr float VELOCIDAD_CARRIL[NUM_CARRILES] = { 12.f, 18.f, 26.f, 34.f }; //autos por carril
    constexpr float VELOCIDAD_ESPECIAL = 20.f; //personas
    constexpr int GAP_MIN = 60;
    constexpr int GAP_MAX = 80;
    constexpr int   VIDAS_INICIALES = 5;
    constexpr float PAUSA_GOLPE_SEG = 0.5f;   // ★ palanca de diseño
}

Level1State::Level1State()
    : jugador(5, FILA_CALLE, VIDAS_INICIALES),
    especiales(VELOCIDAD_ESPECIAL, FILA_CALLE, NUM_CARRILES, ALTO_CARRIL),
    pausaGolpe(PAUSA_GOLPE_SEG) {
}

void Level1State::init() {
    terminado = false;
    acumulador = 0.f;

    if (!fondo.loadFromFile("../Fondo de ciudad Nivel 1.txt")) {
        std::cout << "No se pudo cargar el fondo\n";
        terminado = true;
        return;
    }
    camara = std::make_unique<ContinuousCameraStrategy>(ANCHO_PANTALLA, fondo.getWidth());

    carriles.clear();
    for (int i = 0; i < NUM_CARRILES; ++i) {
        carriles.emplace_back(FILA_CALLE + i * ALTO_CARRIL, VELOCIDAD_CARRIL[i], GAP_MIN, GAP_MAX);
        carriles.back().precalentar(40, ANCHO_PANTALLA);   // sin carros cerca del punto de salida (x=5)
    }

    especiales.reiniciar();
}

bool Level1State::hayChoque() const {
    for (const auto& carril : carriles)
        for (const auto& carro : carril.getCarros())
            if (carro.colisionaCon(jugador)) return true;
    for (const auto& enemigo : especiales.getEnemigos())
        if (enemigo.colisionaCon(jugador)) return true;
    return false;
}

void Level1State::update(float dt) {
    dt = std::min(dt, 0.1f);                                   // NUEVO
    const EntradaJugador in = leerEntrada();
    if (in.salir) { terminado = true; return; }

    //cuando hay colision, el juego se detiene por un instante
    if (enPausa) {
        pausaRestante -= dt;
        if (pausaRestante > 0.f) return;
        for (auto& carril : carriles) carril.retirarChocados(jugador);
        especiales.retirarChocados(jugador);
        vaciarEntrada();
        enPausa = false;
        return;
    }

    //logica de carriles
    for (auto& carril : carriles)                              // NUEVO
        carril.actualizar(dt, camX, ANCHO_PANTALLA);

    especiales.actualizar(dt, camX, ANCHO_PANTALLA);

    //colisiones genera pausa

    if (hayChoque()) {
        jugador.perderVida();
        enPausa = true;
        pausaRestante = pausaGolpe;
        if (!jugador.estaVivo()) { derrota = true; terminado = true; }
        return;
    }

    acumulador += dt;
    if (acumulador < INTERVALO_PASO) return;


    //ritmo de mov del jugador
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
    for (int y = 0; y < ALTO_PANTALLA; ++y)
        for (int x = 0; x < ANCHO_PANTALLA; ++x)
            pantalla[y][x] = fondo.getPixel(camX + x, camY + y);

    for (const auto& carril : carriles)
        carril.dibujar(pantalla, camX, camY);

    especiales.dibujar(pantalla, camX, camY);

    jugador.dibujar(pantalla, camX, camY);

    std::string salida = " VIDAS: ";
    for (int i = 0; i < jugador.getVidas(); ++i) salida += "<3 ";
    salida += "\n";
    for (const auto& fila : pantalla) salida += "|" + fila + "|\n";
    salida += enPausa ? " >>> AUCH! PERDISTE UNA VIDA! <<<\n"
        : " ESQUIVA EL TRAFICO Y SIGUE CORRIENDO!\n";
   
    system("cls");
    std::cout << salida;
}