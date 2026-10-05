#pragma once
#include "IGameState.h"
#include "ICameraBehavior.h"
#include "BackgroundMap.h"
#include "Jugador.h"
#include "Stephen.h"
#include "Policia.h"
#include <memory>
#include "StaticCameraStrategy.h"

class Level2State : public IGameState {
public:
	Level2State();
	void init() override;
	void update(float deltaTime) override;
	void render() override;

	bool haTerminado() const override { return nivelTerminado; }
	bool haGanado() const { return victoria; }   
	bool haPerdido() const { return derrota; }    

private:
	BackgroundMap fondo;
	std::unique_ptr<ICameraBehavior> camara;
	int camX = 0, camY = 0;

	Jugador jugador;
	Stephen stephen;
	Policia policia;

	//arreglar camara
	bool nivelTerminado = false;
	bool victoria = false;
	bool derrota = false;
	float acumulador = 0.f;

};