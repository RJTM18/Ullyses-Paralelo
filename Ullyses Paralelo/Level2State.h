#include "IGameState.h"
#include "math.h"
#include <vector>

class Level2State : public IGameState {
public: 
		Level2State();
		void init() override;
		void update(float deltaTime) override;   // deltaTime en segundos
		void render() override;
		bool haTerminado() const { return terminado; }
		bool  haPerdido() const { return derrota; }
		void  setPausaGolpe(float segundos) { pausaGolpe = std::max(0.f, segundos); }   // ★ LA PALANCA
		float getPausaGolpe() const { return pausaGolpe; }
private:
	bool terminado = false;
	bool  derrota = false;
	float pausaGolpe;
};