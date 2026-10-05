#define NOMINMAX   // windows.h define macros min/max que chocarian con std::min / std::clamp
#include <windows.h>
#include "Level2State.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "Input.h"

//todas las funciones declaradas en una clase aqui deben ser definidas 

namespace {
    struct Punto { int x, y; };

    const char* const RUTA_FONDO = "camino.txt";


    constexpr Caja ZONA_CALLE = { 30, 1, 59, 28 };   // x, y, ancho, alto
    constexpr int  FILA_META = ZONA_CALLE.y;         

    // Puntos de salida (respawn)
    constexpr Punto INICIO_LEOPOLD = { 55, 22 };
    constexpr Punto INICIO_STEPHEN = { 55, 25 };
    constexpr Punto INICIO_POLICIA = { 59,  1 };

    constexpr int   VIDAS_INICIALES = 1;      
    constexpr int   PASO_X = 2;
    constexpr int   PASO_Y = 1;               // un caracter es ~2 veces mas alto que ancho
    constexpr float INTERVALO_PASO = 0.08f;   

    //movimientos inversos del policia
    constexpr int ESPEJO_POLICIA_X = -1;
    constexpr int ESPEJO_POLICIA_Y = -1;

    int desplazamientoPermitido(int delta, int pos, int minimo, int maximo) {
        return std::clamp(pos + delta, minimo, maximo) - pos;
    }

    constexpr int maxX(int anchoSprite) { return ZONA_CALLE.x + ZONA_CALLE.ancho - anchoSprite; }
    constexpr int maxY(int altoSprite) { return ZONA_CALLE.y + ZONA_CALLE.alto - altoSprite; }

    std::string rellenar(std::string texto, std::size_t ancho) {
        if (texto.size() < ancho) texto.append(ancho - texto.size(), ' ');
        return texto;
    }

    void cursorAlInicio() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, 0 });
    }
}

Level2State::Level2State()
    : jugador(INICIO_LEOPOLD.x, INICIO_LEOPOLD.y, VIDAS_INICIALES),
    stephen(INICIO_STEPHEN.x, INICIO_STEPHEN.y),
    policia(INICIO_POLICIA.x, INICIO_POLICIA.y) {
}

void Level2State::init() {
    nivelTerminado = false;
    victoria = false;
    derrota = false;
    acumulador = 0.f;

    if (!fondo.loadFromFile(RUTA_FONDO)) {
        std::cout << "No se pudo cargar el fondo: " << RUTA_FONDO << "\n";
        nivelTerminado = true;
        return;
    }

    camara = std::make_unique<StaticCameraStrategy>(fondo.getWidth(), fondo.getWidth());
    camara->updateCamera(jugador.getX(), jugador.getY(), camX, camY);

    std::system("cls"); 
}

void Level2State::update(float dt) {
    dt = std::min(dt, 0.1f);
    const EntradaJugador entradaJugador = leerEntrada();
    if (entradaJugador.salir) { nivelTerminado = true; return; }

    acumulador += dt;
    if (acumulador < INTERVALO_PASO) return;

    //imput
    int dx = 0, dy = 0;
    if (entradaJugador.derecha && !entradaJugador.izquierda)      dx = PASO_X;
    else if (entradaJugador.izquierda && !entradaJugador.derecha) dx = -PASO_X;
    if (entradaJugador.arriba && !entradaJugador.abajo)           dy = -PASO_Y;
    else if (entradaJugador.abajo && !entradaJugador.arriba)      dy = PASO_Y;

    if (dx == 0 && dy == 0) return;   
    acumulador = 0.f;                

    // Los dos reciben el mismo dx, dy.
    const int dxL = desplazamientoPermitido(dx, jugador.getX(), ZONA_CALLE.x, maxX(Jugador::ANCHO_SPRITE));
    const int dxS = desplazamientoPermitido(dx, stephen.getX(), ZONA_CALLE.x, maxX(Stephen::ANCHO_SPRITE));
    const int dyL = desplazamientoPermitido(dy, jugador.getY(), ZONA_CALLE.y, maxY(Jugador::ALTO_SPRITE));
    const int dyS = desplazamientoPermitido(dy, stephen.getY(), ZONA_CALLE.y, maxY(Stephen::ALTO_SPRITE));
    const int dxGrupo = (std::abs(dxL) < std::abs(dxS)) ? dxL : dxS;   // el mas restrictivo (mismo signo)
    const int dyGrupo = (std::abs(dyL) < std::abs(dyS)) ? dyL : dyS;

    // El policia reacciona a la MISMA entrada (no a dxGrupo, dyGrupo) y tiene sus propias paredes:
    // si Leopold esta bloqueado pero el policia no, el policia igual avanza (y viceversa).
    const int dxPoli = desplazamientoPermitido(ESPEJO_POLICIA_X * dx, policia.getX(), ZONA_CALLE.x, maxX(Policia::ANCHO_SPRITE));
    const int dyPoli = desplazamientoPermitido(ESPEJO_POLICIA_Y * dy, policia.getY(), ZONA_CALLE.y, maxY(Policia::ALTO_SPRITE));

    // Todos se mueven en el MISMO tick
    if (dxGrupo != 0 || dyGrupo != 0) {
        jugador.mover(dxGrupo, dyGrupo);
        stephen.mover(dxGrupo, dyGrupo);
    }
    if (dxPoli != 0 || dyPoli != 0) policia.mover(dxPoli, dyPoli);

    if (policia.colisionaCon(jugador) || policia.colisionaCon(stephen)) {
        jugador.perderVida();
        derrota = true;
        nivelTerminado = true;
        return;
    }

    // meta
    if (jugador.getY() <= FILA_META) {
        victoria = true;
        nivelTerminado = true;
    }
}

void Level2State::render() {
    const int ancho = fondo.getWidth();
    const int alto = fondo.getHeight();

    //fondo
    std::vector<std::string> pantalla(alto, std::string(ancho, ' '));
    for (int y = 0; y < alto; ++y)
        for (int x = 0; x < ancho; ++x)
            pantalla[y][x] = fondo.getPixel(camX + x, camY + y);

    //actores 
    policia.dibujar(pantalla, camX, camY);
    stephen.dibujar(pantalla, camX, camY);
    jugador.dibujar(pantalla, camX, camY);

    //texto
    const std::size_t anchoLinea = static_cast<std::size_t>(ancho) + 2;   // + los dos '|'
    std::string pie = " Esquiva al policia: se mueve en espejo a tus pasos!";
    if (victoria)     pie = " LLEGASTE A LA META! Leopold y Stephen escapan de la policia.";
    else if (derrota) pie = " LA POLICIA TE ATRAPO!";

    std::string salida = rellenar(" NIVEL 2 - Llega a la parte superior de la calle!   (X = salir)", anchoLinea) + "\n";
    for (const auto& fila : pantalla) salida += "|" + fila + "|\n";
    salida += rellenar(pie, anchoLinea) + "\n";

    cursorAlInicio();
    std::cout << salida << std::flush;
}