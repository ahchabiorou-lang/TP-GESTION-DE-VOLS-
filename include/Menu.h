/**
 * @file Menu.h
 * @brief Interface console : affichage des menus, saisies et messages d'erreur.
 */
#pragma once
#include "Agence.h"

class Menu {
private:
    Agence& agence_;   // association : le menu utilise l'agence

    void afficherMenuPrincipal() const;
    void actionEnregistrerPassager();
    void actionAjouterVol();
    void actionAfficherVolsDisponibles() const;
    void actionRechercherVol() const;
    void menuReservations();
    void actionNouvelleReservation();
    void actionListerReservations() const;
    void actionAfficherBillet() const;
    void actionEnregistrerPaiement();
    void actionAnnulerReservation();
    void actionListerPassagers() const;
    void actionListerPaiements() const;
    void actionStatistiques() const;

    static void titre(const std::string& t);
    static void afficherListeVols(const std::vector<Vol*>& vols);
    static void pause();

public:
    explicit Menu(Agence& a) : agence_(a) {}
    void lancer();   ///< boucle principale
};
