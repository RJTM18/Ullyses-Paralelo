#include "Level2State.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "Input.h"

//todas las funciones declaradas en una clase aqui deben ser definidas 

namespace {
    const int mov_x = 2; //andar en dos
    const int mov_y = 2;
    const int ANCHO_MUNDO = 120;
    const int ALTO_MUNDO = 40;

    const int NumeroVidas;
    //respaw coordenada
    const int puntoInicialX = 5;
    const int puntoInicialY = 5;

    //coord poli
    const int puntoIX = 5;
    const int puntoIY = 5;

    //static camera
    const int camX, camY = 0;
}

Level2State::Level2State() : jugador(puntoInicialX, puntoInicialY, NumeroVidas), 
                            stephen(puntoInicialX + 5, puntoInicialY - 5), 
                            policia(puntoIX, puntoIY) { };

void Level2State::init() {
    std::ifstream tramo2("#2.txt");

    if (!tramo2.is_open()) {
        std::cout << "No se pudo cargar el fondo\n";
    }

    std::string linea;

    while (std::getline(tramo2, linea)) { //if linea es vacio, retorna FALSE
        std::cout << linea << std::endl;
    }
    
    tramo2.close();
}

void Level2State::render() { ; }

void Level2State::update(float dt) {
    dt = std::min(dt, 0.1f);                                   // NUEVO
    const EntradaJugador entradaJugador = leerEntrada();
    if (entradaJugador.salir) { NivelTerminado = true; return; }

/*
    if (enPausa) {
        pausaRestante -= dt;
        if (pausaRestante > 0.f) return;
        for (auto& carril : carriles) carril.retirarChocados(jugador);
        especiales.retirarChocados(jugador);
        vaciarEntrada();
        enPausa = false;
        return;
    }

    if (hayChoque()) {
        jugador.perderVida();
        enPausa = true;
        pausaRestante = pausaGolpe;
        if (!jugador.estaVivo()) { derrota = true; terminado = true; }
        return;
    }
*/
       //ritmo de mov del jugador
    int dx = 0, dy = 0;
    if (entradaJugador.derecha && !entradaJugador.izquierda)      dx = mov_x;
    else if (entradaJugador.izquierda && !entradaJugador.derecha) dx = -mov_x;
    if (entradaJugador.arriba && !entradaJugador.abajo)           dy = -mov_y;
    else if (entradaJugador.abajo && !entradaJugador.arriba)      dy = mov_y;

    // Los límites los decide el nivel, no el jugador
    // imput = jugador.getX() + dx, min = 0, max =  ANCHO_MUNDO - Jugador::ANCHO_SPRITE)
    const int nx = std::clamp(jugador.getX() + dx, 0, ANCHO_MUNDO - Jugador::ANCHO_SPRITE);
    const int ny = std::clamp(jugador.getY() + dy, 0, ALTO_MUNDO - Jugador::ALTO_SPRITE);
    dx = nx - jugador.getX();
    dy = ny - jugador.getY();

    if (dx != 0 || dy != 0) {
        jugador.mover(dx, dy);
        stephen.mover(dx, dy);
        policia.mover(-dx, -dy); //mov inverso para el primer policia


        camara->updateCamera(jugador.getX(), jugador.getY(), camX, camY);
    }



}