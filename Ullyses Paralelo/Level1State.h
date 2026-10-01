#pragma once
#include "IGameState.h"
#include "Jugador.h"

class Level1State : public IGameState {
public:
    Level1State();
    void init() override;
    void update(float deltaTime) override;   // deltaTime en segundos
    void render() override;
    bool haTerminado() const { return terminado; }

private:
    Jugador jugador;
    float acumulador = 0.f;
    bool terminado = false;
    int camX = 0, camY = 0;
};