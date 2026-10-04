#pragma once
#include "IGameState.h"
#include "Jugador.h"
#include "BackgroundMap.h"
#include "ICameraBehavior.h"
#include <memory>
#include "VelocidadPorCarril.h"
#include <vector>
#include "GestorEspeciales.h"
#include <algorithm>      

class Level1State : public IGameState {
public:
    Level1State();
    void init() override;
    void update(float deltaTime) override;   // deltaTime en segundos
    void render() override;
    bool haTerminado() const override { return terminado; }
    bool  haPerdido() const { return derrota; }
    void  setPausaGolpe(float segundos) { pausaGolpe = std::max(0.f, segundos); }   // ★ LA PALANCA
    float getPausaGolpe() const { return pausaGolpe; }

private:
    Jugador jugador;
    float acumulador = 0.f;
    bool terminado = false;
    int camX = 0, camY = 0;
    BackgroundMap fondo;
    std::unique_ptr<ICameraBehavior> camara;
    std::vector<Carril> carriles;
    GestorEspeciales especiales;
    bool hayChoque() const;
    bool  enPausa = false;
    float pausaRestante = 0.f;
    float pausaGolpe;
    bool  derrota = false;
};