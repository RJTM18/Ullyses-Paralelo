#pragma once
#include "Entidad.h"

class Stephen : public Entidad { //1
public:
	static constexpr int ANCHO_SPRITE = 3;
	static constexpr int ALTO_SPRITE = 5;

	Stephen(int x, int y) : Entidad(x, y) {};

	void dibujar(std::vector<std::string>& pantalla, int camX, int camY) const override;
	Caja getHitbox() const override;
	void mover(int dx, int dy);

private:
	bool mirandoArriba = false;
	int frameActual = 0; //frameActual pertecene rangoC(0,2)
};
