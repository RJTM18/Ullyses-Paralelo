#pragma once
#include "IGameState.h"
#include "Jugador.h"
#include "BackgroundMap.h"
#include "ICameraBehavior.h"
#include <memory>
#include "VelocidadPorCarril.h"
#include <vector>
#include <string>
#include "GestorEspeciales.h"
#include <algorithm>


struct LugarEspecial {
    std::string nombre;                 
    int xInicio;                        
    int xFin;                           
    std::vector<std::string> texto;     
    bool esMeta = false;                
    bool visitado = false;              
};

class Level1State : public IGameState {
public:
    Level1State();
    void init() override;
    void update(float deltaTime) override;   // deltaTime en segundos
    void render() override;
    bool haTerminado() const override { return terminado; }
    bool  haGanado() const { return victoria; }    // true: llego al burdel y cerro su texto -> toca el nivel 2
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
    bool  victoria = false;

    // --- lugares especiales ---
    std::vector<LugarEspecial> lugares;
    int  lugarActivo = -1;                              
    std::vector<std::vector<std::string>> paginasTexto; 
    std::size_t paginaActual = 0;

    int  lugarEnPosicion() const;                       
    void abrirLugar(int indice);                       
    void avanzarTexto();                            
    std::vector<std::string> armarZonaTexto() const;  
};