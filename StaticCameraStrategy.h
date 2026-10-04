#pragma once
#include "ICameraBehavior.h" 

class StaticCameraStrategy : public ICameraBehavior {
private:
	//variable
	int centroPantalla;
	//constantes
	int anchoPantalla;
	int anchoMundo;

public:
	StaticCameraStrategy(int anchoPantalla, int anchoMundo) : ICameraBehavior() { }; //no se requiere introducir todos los atributos en private; solo los necesarios
	void updateCamera(int playerX, int playerY, int& camX, int& camY) override; //variables
};