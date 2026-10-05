#ifndef NOMINMAX
#define NOMINMAX   // Consola.h trae windows.h, cuyas macros min/max chocarian con std::min / std::clamp
#endif
#include "Level1State.h"
#include "Input.h"
#include "Consola.h"
#include "Narrativa.h"
#include "Textos.h"
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
    constexpr int   VIDAS_INICIALES = 10;
    constexpr float PAUSA_GOLPE_SEG = 0.5f;   // ★ palanca de diseño

    // --- texto debajo del nivel (lugares especiales) ---
    constexpr std::size_t ANCHO_LINEA = ANCHO_PANTALLA + 2;     // + los dos '|' del marco
    constexpr int ALTO_ZONA_TEXTO = 9;                          // 1 titulo + 7 lineas de texto + 1 aviso "[X]"
    constexpr int LINEAS_DE_TEXTO = ALTO_ZONA_TEXTO - 2;
    constexpr int ANCHO_TEXTO = ANCHO_PANTALLA - 4;
    // Alto total del cuadro = 1 (vidas) + 23 (nivel) + 1 (pie) + 9 (texto) = 34 filas: cabe en la consola de 36 de main.cpp
}

Level1State::Level1State()
    : jugador(5, FILA_CALLE, VIDAS_INICIALES),
    especiales(VELOCIDAD_ESPECIAL, FILA_CALLE, NUM_CARRILES, ALTO_CARRIL),
    pausaGolpe(PAUSA_GOLPE_SEG) {
}

void Level1State::init() {
    terminado = false;
    victoria = false;
    acumulador = 0.f;
    lugarActivo = -1;
    paginasTexto.clear();
    paginaActual = 0;

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

    lugares = {
        { "IGLESIA",      152,    164,  Textos::IGLESIA,      false },
        { "UNIVERSIDAD",  325,    338,  Textos::UNIVERSIDAD,  false },   
        { "PUB",          507,    520,  Textos::PUB,          false },  
        { "BURDEL",       684,    698,  Textos::BURDEL,       true  },   
    };

    std::system("cls");   // render() redibuja desde la esquina (sin cls cada cuadro), asi que se limpia una sola vez
}

bool Level1State::hayChoque() const {
    for (const auto& carril : carriles)
        for (const auto& carro : carril.getCarros())
            if (carro.colisionaCon(jugador)) return true;
    for (const auto& enemigo : especiales.getEnemigos())
        if (enemigo.colisionaCon(jugador)) return true;
    return false;
}

int Level1State::lugarEnPosicion() const {
    const int centro = jugador.getX() + Jugador::ANCHO_SPRITE / 2;
    for (std::size_t i = 0; i < lugares.size(); ++i)
        if (!lugares[i].visitado && centro >= lugares[i].xInicio && centro <= lugares[i].xFin)
            return static_cast<int>(i);
    return -1;
}

void Level1State::abrirLugar(int indice) {
    lugares[indice].visitado = true;
    lugarActivo = indice;
    paginasTexto = Narrativa::paginar(lugares[indice].texto, ANCHO_TEXTO, LINEAS_DE_TEXTO, false);
    paginaActual = 0;
    vaciarEntrada();   
}

void Level1State::avanzarTexto() {
    if (paginaActual + 1 < paginasTexto.size()) { ++paginaActual; return; }

    const bool eraMeta = lugares[lugarActivo].esMeta;
    lugarActivo = -1;
    paginasTexto.clear();
    paginaActual = 0;
    vaciarEntrada();
    if (eraMeta) { victoria = true; terminado = true; }   // el burdel cierra el nivel
}

void Level1State::update(float dt) {
    dt = std::min(dt, 0.1f);                                   
    const EntradaJugador in = leerEntrada();

    // Texto de un lugar especial abierto: el nivel esta congelado (nada se mueve) y X avanza / cierra el texto.
    // Va ANTES de "X = salir" para que X no cierre el nivel mientras se lee.
    if (lugarActivo >= 0) {
        if (in.salir) avanzarTexto();
        return;
    }

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
    for (auto& carril : carriles)                              
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

        // entro a un lugar especial? -> pausa y texto debajo del nivel
        const int lugar = lugarEnPosicion();
        if (lugar >= 0) abrirLugar(lugar);
    }


}

// Lineas (ya rellenas hasta el ancho del marco) del cuadro de texto que va debajo del nivel.
// Siempre devuelve ALTO_ZONA_TEXTO filas: asi, al cerrarse el texto, se borra solo al redibujar.
std::vector<std::string> Level1State::armarZonaTexto() const {
    std::vector<std::string> zona(ALTO_ZONA_TEXTO, std::string(ANCHO_LINEA, ' '));
    if (lugarActivo < 0) return zona;

    std::string titulo = "+--[ " + lugares[lugarActivo].nombre + " ]";
    if (titulo.size() < ANCHO_LINEA) titulo.append(ANCHO_LINEA - titulo.size() - 1, '-');
    zona[0] = Narrativa::rellenar(titulo + "+", ANCHO_LINEA);

    const auto& pagina = paginasTexto[paginaActual];
    for (std::size_t i = 0; i < pagina.size() && i < static_cast<std::size_t>(LINEAS_DE_TEXTO); ++i)
        zona[1 + i] = Narrativa::rellenar("  " + pagina[i], ANCHO_LINEA);

    std::string aviso = (paginaActual + 1 < paginasTexto.size()) ? " [X] Siguiente" : " [X] Continuar";
    if (paginasTexto.size() > 1)
        aviso += "   (" + std::to_string(paginaActual + 1) + "/" + std::to_string(paginasTexto.size()) + ")";
    zona[ALTO_ZONA_TEXTO - 1] = Narrativa::rellenar(aviso, ANCHO_LINEA);
    return zona;
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

    std::string vidas = " VIDAS: ";
    for (int i = 0; i < jugador.getVidas(); ++i) vidas += "<3 ";

    std::string pie = " ESQUIVA EL TRAFICO Y SIGUE CORRIENDO!";
    if (victoria)              pie = " LLEGASTE AL FINAL DE LA CALLE!";
    else if (lugarActivo >= 0) pie = " JUEGO EN PAUSA";
    else if (enPausa)          pie = " >>> AUCH! PERDISTE UNA VIDA! <<<";

    // Todas las filas se rellenan al mismo ancho: redibujar desde la esquina no deja restos (y no parpadea como cls)
    std::string salida = Narrativa::rellenar(vidas, ANCHO_LINEA) + "\n";
    for (const auto& fila : pantalla) salida += "|" + fila + "|\n";
    salida += Narrativa::rellenar(pie, ANCHO_LINEA) + "\n";
    for (const auto& linea : armarZonaTexto()) salida += linea + "\n";

    moverCursor(0, 0);
    std::cout << salida << std::flush;
}