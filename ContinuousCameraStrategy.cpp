#include "ContinuousCameraStrategy.h"

//declaracion de variables internas
ContinuousCameraStrategy::ContinuousCameraStrategy(int anchoP, int anchoM) : centroPantalla(anchoP / 2), anchoPantalla(anchoP), anchoMundo(anchoM) { }

void ContinuousCameraStrategy::updateCamera(int playerX, int playerY, int& camX, int& camY) {
    camX = playerX - centroPantalla;
    if (camX < 0) camX = 0;
    if (camX > anchoMundo - anchoPantalla) camX = anchoMundo - anchoPantalla;
    camY = 0; //no desplazamiento vertical
}