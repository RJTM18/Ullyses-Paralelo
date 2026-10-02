#pragma once
#include "Entidad.h"
class Carro : public Entidad {
public:
	static constexpr int ANCHO_SPRITE = 12;
	static constexpr int ALTO_SPRITE = 3;

	Carro(int x, int y) : Entidad(x, y), xExacta(static_cast<float>(x)) {} //converting to float

	void avanzar(float distancia);   // columnas hacia la IZQUIERDA (puede ser fraccionaria)
	void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const override;

	Caja getHitbox() const override { return { posicionX, posicionY, 10, ALTO_SPRITE }; }  // sin el escape "..."
private:
	float xExacta; // posición real; posicionX (int) es solo la versión redondeada
};