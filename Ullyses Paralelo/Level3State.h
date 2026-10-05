#pragma once
#include <vector>
#include <conio.h>
#include "Molly.h"
#include "Palabra.h"
#include "Personaje.h"
#include "Pensamiento.h"

inline bool jugarNivel3()
{
    Molly molly(37, 17); Pensamiento jefe(28, 4); vector<Palabra*> palabras;
    int contador = 0, espera = 0;
    while (molly.getVidas() > 0 && jefe.getVida() > 0) {
        system("cls");
        cout << "NIVEL 3 - MOLLY: PENELOPE     MOLLY: " << molly.getVidas() << "     PENSAMIENTO: " << jefe.getVida() << "\n";
        cout << "A/D mover - ESPACIO responder - X salir\n";
        cout << "+-----------------------------------------------------------------------------+\n";
        jefe.dibujar(); molly.dibujar();
        for (int i = 0;i < palabras.size();i++)
        {
            moverCursor(palabras[i]->getX(), palabras[i]->getY()); cout << palabras[i]->getTexto();
        }

        moverCursor(0, 22); cout << "MOLLY: " << jefe.dialogo() << "                         ";

        if (_kbhit())
        {
            int tecla = _getch();
            if (tecla == 0 || tecla == 224)
            {
                tecla = _getch(); if (tecla == 75)molly.mover('A'); if (tecla == 77)molly.mover('D');
            }
            else
            {
                char t = (char)toupper(tecla); if (t == 'X') { for (auto p : palabras)delete p;return false; } if (t == 'A' || t == 'D')molly.mover(t);
                if (t == ' ' && espera == 0) {
                    string txt = jefe.getVida() == 1 ? "YES" : "RECUERDO"; palabras.push_back
                    (new Palabra(molly.getX(), 16, txt, false)); espera = 5;
                }
            }
        }
        if (espera > 0) espera--;
        if (contador % 12 == 0)

        {
            const string ataques[4] = { "DUDA","PASADO","NO","MIEDO" };

            palabras.push_back(new Palabra(20 + (contador % 35), 9, ataques[(contador / 12) % 4], true));
        }
        jefe.mover();
        for (int i = 0;i < (int)palabras.size();i++) {
            palabras[i]->mover(); bool borrar = false;
            if (palabras[i]->esEnemiga() && palabras[i]->getY() >= 17 && abs(palabras[i]->getX() - molly.getX()) < 8)

            {
                molly.perderVida(); borrar = true;
            }

            if (!palabras[i]->esEnemiga() && palabras[i]->getY() <= 8 && abs(palabras[i]->getX() - jefe.getX()) < 22)

            {
                jefe.recibirGolpe(); borrar = true;
            }
            if (palabras[i]->getY() < 3 || palabras[i]->getY() > 21) borrar = true;

            if (borrar) { delete palabras[i]; palabras.erase(palabras.begin() + i); i--; }
        }
        contador++;
        Sleep(80);

    }



    for (auto p : palabras) delete p;
    system("cls");
    if (molly.getVidas() == 0) { cout << "EL PENSAMIENTO ALCANZO A MOLLY.\n"; _getch(); return false; }
    cout << "===================== YES =====================\n\n";
    cout << "El pensamiento se desarma palabra por palabra.\n";
    cout << "Molly recuerda a Bloom y Howth. Nivel completado.\n\n";
    cout << "Presiona una tecla..."; _getch(); return true;
}