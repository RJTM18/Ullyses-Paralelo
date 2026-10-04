#include "StaticCameraStrategy.h"

//declaracion de variables internas
StaticCameraStrategy::StaticCameraStrategy(int anchoP, int anchoM) : centroPantalla(anchoP / 2), anchoPantalla(anchoP), anchoMundo(anchoM) {}

void StaticCameraStrategy::updateCamera(int playerX, int playerY, int& camX, int& camY) {
    camX = 0; //no desplazamiento horizontal
    camY = 0; //no desplazamiento vertical
}