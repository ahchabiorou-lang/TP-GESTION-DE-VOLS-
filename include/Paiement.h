/**
 * @file Paiement.h
 * @brief Classe Paiement : règlement d'une réservation + suivi du remboursement.
 */
#pragma once
#include <ctime>
#include "Types.h"

class Reservation;

class Paiement {
private:
    int id_;
    Reservation* reservation_;   // association obligatoire, multiplicité 1
    long montant_;
    ModePaiement mode_;
    time_t date_;
    StatutPaiement statut_;
    long montantRembourse_;

public:
    Paiement(int id, Reservation* r, long montant, ModePaiement mode);

    int getId() const { return id_; }
    Reservation* getReservation() const { return reservation_; }
    long getMontant() const { return montant_; }
    ModePaiement getMode() const { return mode_; }
    time_t getDate() const { return date_; }
    StatutPaiement getStatut() const { return statut_; }
    long getMontantRembourse() const { return montantRembourse_; }

    /// Applique la politique de remboursement : taux en % (0, 50 ou 100).
    /// @return montant effectivement remboursé.
    long rembourser(int tauxPourcent);

    void afficher() const;
    static void afficherEntete();
};
