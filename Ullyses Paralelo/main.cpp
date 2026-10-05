#ifndef NOMINMAX
#define NOMINMAX   // el nivel 3 trae windows.h: por seguridad se evita que sus macros min/max choquen con std::min / std::clamp
#endif
#include <chrono>
#include <thread>
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include "Level1State.h"
#include "Level2State.h"
#include "Input.h"
#include "Narrativa.h"
#include "Textos.h"
#include "Level3State.h"   // SIEMPRE el ultimo include: Palabra.h trae "using namespace std;" y se filtra a lo que venga despues


namespace {
    // (*) Para probar un nivel sin jugar los anteriores: 1 = juego completo, 2 = empieza en el nivel 2, 3 = empieza en el nivel 3
    constexpr int NIVEL_INICIAL = 1;

    void correr(IGameState& nivel) {
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

    bool jugarNivel1() { Level1State nivel; correr(nivel); return nivel.haGanado(); }
    bool jugarNivel2() { Level2State nivel; correr(nivel); return nivel.haGanado(); }

    // Deja ver el ultimo cuadro del nivel ("llegaste a la meta") antes de la pantalla de texto
    void pausaDeVictoria() {
        vaciarEntrada();
        std::this_thread::sleep_for(std::chrono::milliseconds(1200));
        vaciarEntrada();
    }

    void finDelJuego(const char* mensaje) {
        // sin "\n" al inicio: el cuadro del nivel ocupa casi toda la consola y un salto de mas la haria hacer scroll
        std::cout << " " << mensaje << "   Presiona una tecla para salir...";
        vaciarEntrada();   // por si el jugador seguia pulsando flechas
        _getch();
    }
}

int main() {
    std::system("mode con: cols=124 lines=36");   // camino.txt mide 30 filas: que quepa sin hacer scroll

    // Cadena: [intro] -> nivel 1 -> [textos] -> nivel 2 -> [textos] -> nivel 3
    // Si el jugador pierde (o sale con X dentro de un nivel) el juego termina.
    bool continuar = true;

    if (continuar && NIVEL_INICIAL <= 1) {
        Narrativa::mostrarPantallaTexto(Textos::TITULO_NIVEL_1, Textos::INTRO_NIVEL_1);   // intro: X para empezar el nivel 1
        continuar = jugarNivel1();
        if (continuar) {
            pausaDeVictoria();
            Narrativa::mostrarPantallaTexto(Textos::TITULO_NIVEL_2, Textos::ANTES_NIVEL_2);   // X para pasar al nivel 2
        }
    }

    if (continuar && NIVEL_INICIAL <= 2) {
        continuar = jugarNivel2();
        if (continuar) {
            pausaDeVictoria();
            Narrativa::mostrarPantallaTexto(Textos::TITULO_NIVEL_3, Textos::ANTES_NIVEL_3);   // X para pasar al nivel 3
        }
    }

    if (continuar) continuar = jugarNivel3();   // el nivel 3 muestra su propio final

    finDelJuego(continuar ? "Juego completado!" : "Fin del juego.");
}