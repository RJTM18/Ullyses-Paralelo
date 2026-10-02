#pragma once
#include "Carro.h"
#include <algorithm>
#include <deque>

class Carril {
public:
    Carril(int fila, float vel, int gMin, int gMax);

    void  setVelocidad(float colPorSeg) { velocidad = std::max(0.f, colPorSeg); }  // <-- LA PALANCA
    float getVelocidad() const { return velocidad; }
    int   getFila() const { return filaY; }

    void precalentar(int desdeX, int hastaX);
    void actualizar(float dt, int camX, int anchoPantalla);
    void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const;

    const std::deque<Carro>& getCarros() const { return carros; }   // para colisiones, después
    //en una lista, agregar de manera rapida elementos al inicio y final

    void retirarChocados(const Entidad& objetivo) {
        std::erase_if(carros, [&](const Carro& c) { return c.colisionaCon(objetivo); });
    }

private:
    int sortearGap() const;

    int   filaY;
    float velocidad;       // columnas por segundo, hacia la izquierda
    int   gapMin, gapMax;  // espacio libre entre un carro y el siguiente
    int   gapPendiente;    // espacio que exigirá el PRÓXIMO carro
    std::deque<Carro> carros;   // front = el más viejo (el más a la izquierda)
};