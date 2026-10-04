#include "IGameState.h"
#include "StaticCameraStrategy.h"
#include "ICameraBehavior.h"
// #include "BackgroundMap.h"
#include "math.h"
#include <vector>
#include <fstream> 
#include "Jugador.h"
#include "Stephen.h"
#include "Policia.h"

class Level2State : public IGameState {
public: 
		Level2State();
		void init() override;
		void update(float deltaTime) override;
		void render() override;

		void spawn();
private:
	// BackgroundMap fondo;
	bool NivelTerminado = false;
	Jugador jugador;
	Stephen stephen;
	Policia policia;

	//arreglar camara
	StaticCameraStrategy* camara;
	const int camX = 0, camY = 0;

};