/**
 * Projet POO - Gestion des vols et reservations d'une agence de voyage a Cotonou.
 * Point d'entree : on cree l'agence puis on lance l'interface console.
 */
#include "Agence.h"
#include "Menu.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   // affichage correct des accents sous Windows
#endif
    Agence agence("Cotonou Voyages");
    Menu menu(agence);
    menu.lancer();
    return 0;
}
