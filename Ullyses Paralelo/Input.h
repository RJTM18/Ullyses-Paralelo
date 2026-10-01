#pragma once
struct EntradaJugador {
    bool derecha = false, 
        izquierda = false, 
        arriba = false, 
        abajo = false, 
        salir = false;
};
EntradaJugador leerEntrada();