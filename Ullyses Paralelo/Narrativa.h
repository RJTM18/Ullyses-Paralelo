// Narrativa.h  -  utilidades para mostrar texto al jugador (solo cabecera: no hay que agregar ningun .cpp al proyecto)
#pragma once
#include <algorithm>
#include <cctype>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "Input.h"

namespace Narrativa {

    using Pagina = std::vector<std::string>;   // lineas ya ajustadas que caben en una "pantalla"

    // Recorta o rellena con espacios hasta 'ancho'. Sirve para redibujar sobre lo anterior sin system("cls").
    inline std::string rellenar(std::string texto, std::size_t ancho) {
        if (texto.size() > ancho) texto.resize(ancho);
        else texto.append(ancho - texto.size(), ' ');
        return texto;
    }

    // Parte UN parrafo en lineas de maximo 'ancho' caracteres sin cortar palabras ('\n' fuerza salto de linea)
    inline std::vector<std::string> ajustarTexto(const std::string& parrafo, std::size_t ancho) {
        std::vector<std::string> lineas;
        std::size_t inicio = 0;
        while (inicio <= parrafo.size()) {
            std::size_t fin = parrafo.find('\n', inicio);
            if (fin == std::string::npos) fin = parrafo.size();
            const std::string segmento = parrafo.substr(inicio, fin - inicio);
            inicio = fin + 1;

            std::string actual, palabra;
            const auto agregarPalabra = [&]() {
                if (palabra.empty()) return;
                while (palabra.size() > ancho) {                       // palabra mas larga que la linea
                    if (!actual.empty()) { lineas.push_back(actual); actual.clear(); }
                    lineas.push_back(palabra.substr(0, ancho));
                    palabra.erase(0, ancho);
                }
                if (actual.empty())                              actual = palabra;
                else if (actual.size() + 1 + palabra.size() <= ancho) actual += ' ' + palabra;
                else { lineas.push_back(actual); actual = palabra; }
                palabra.clear();
                };
            for (const char c : segmento) {
                if (c == ' ') agregarPalabra();
                else palabra += c;
            }
            agregarPalabra();
            lineas.push_back(actual);                                  // (vacia si el segmento no tenia texto)
        }
        return lineas;
    }

    // Convierte la serie de strings en paginas de 'lineasPorPagina' lineas. Si el texto no cabe en una, el jugador pulsa X por pagina.
    inline std::vector<Pagina> paginar(const std::vector<std::string>& parrafos, std::size_t ancho,
        std::size_t lineasPorPagina, bool lineaEnBlancoEntreParrafos) {
        std::vector<std::string> lineas;
        for (std::size_t i = 0; i < parrafos.size(); ++i) {
            if (i > 0 && lineaEnBlancoEntreParrafos) lineas.push_back("");
            for (const auto& l : ajustarTexto(parrafos[i], ancho)) lineas.push_back(l);
        }

        std::vector<Pagina> paginas;
        std::size_t i = 0;
        while (i < lineas.size()) {
            if (!paginas.empty())                                      // sin lineas en blanco al inicio de una pagina nueva
                while (i < lineas.size() && lineas[i].empty()) ++i;
            if (i >= lineas.size()) break;
            const std::size_t fin = std::min(lineas.size(), i + lineasPorPagina);
            paginas.emplace_back(lineas.begin() + i, lineas.begin() + fin);
            i = fin;
        }
        if (paginas.empty()) paginas.emplace_back();                   // sin texto: igual hay una pagina (vacia)
        return paginas;
    }

    // Bloquea hasta que el jugador presione X (ignora cualquier otra tecla, incluidas las flechas)
    inline void esperarX() {
        vaciarEntrada();                                               // descarta teclas que se hayan quedado en el buffer
        for (;;) {
            const int t = _getch();
            if (t == 0 || t == 224) { _getch(); continue; }            // tecla especial: ocupa dos codigos
            if (std::toupper(t) == 'X') return;
        }
    }

    // Pantalla completa de texto: titulo + serie de strings. Pulsar X pasa de pagina y, en la ultima, termina.
    // Pensada para las transiciones entre niveles (consola de 124 x 36).
    inline void mostrarPantallaTexto(const std::string& titulo, const std::vector<std::string>& parrafos) {
        constexpr std::size_t ANCHO_CONSOLA = 124;
        constexpr std::size_t MARGEN = 12;
        constexpr std::size_t ANCHO_TEXTO = ANCHO_CONSOLA - 2 * MARGEN;
        constexpr std::size_t LINEAS_POR_PAGINA = 22;

        const auto centrar = [&](const std::string& s) {
            const std::size_t sangria = s.size() < ANCHO_CONSOLA ? (ANCHO_CONSOLA - s.size()) / 2 : 0;
            return std::string(sangria, ' ') + s + "\n";
            };

        const auto paginas = paginar(parrafos, ANCHO_TEXTO, LINEAS_POR_PAGINA, true);
        for (std::size_t p = 0; p < paginas.size(); ++p) {
            std::string salida = "\n\n";
            if (!titulo.empty()) salida += centrar("=== " + titulo + " ===") + "\n";
            for (const auto& linea : paginas[p]) salida += std::string(MARGEN, ' ') + linea + "\n";

            std::string aviso = "[ Presiona X para continuar ]";
            if (paginas.size() > 1) aviso += "   (" + std::to_string(p + 1) + "/" + std::to_string(paginas.size()) + ")";
            salida += "\n\n" + centrar(aviso);

            std::system("cls");
            std::cout << salida << std::flush;
            esperarX();
        }
    }
}