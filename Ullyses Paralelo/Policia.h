#pragma once
#include "Entidad.h"

class Policia : public Entidad { //muchos
public:
	static constexpr int ANCHO_SPRITE = 5;
	static constexpr int ALTO_SPRITE = 3;

	Policia(int x, int y) : Entidad(x, y) {};

	void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const override;
	void mover(int dx, int dy);

	Caja getHitbox() const override;


private:
	bool mirandoAbajo = true;
	int frameActual = 0;

};