#pragma once
class Entidad {
protected:
	int posicionX;
	int posicionY;
public:
	virtual void printSprite() = 0;
};