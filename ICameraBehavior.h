#pragma once
class ICameraBehavior {
public:
    virtual void updateCamera(int playerX, int playerY, int& camX, int& camY) = 0;
    virtual ~ICameraBehavior() = default;
    //sea camX la coordenada x del borde izquierdo de la pantalla
};