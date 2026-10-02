#pragma once
#include "Entidad.h"

class SpecialEnemy : public Entidad {
public:
    static constexpr int ANCHO_SPRITE = 5;
    static constexpr int ALTO_SPRITE = 3;

    SpecialEnemy(int x, int y) : Entidad(x, y), xExacta(static_cast<float>(x)) {}

    void avanzar(float distancia);   // columnas hacia la IZQUIERDA (puede ser fraccionaria)
    void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const override;

private:
    float xExacta;   // posición real; posicionX (int) es solo la versión redondeada
};