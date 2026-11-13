/**
 * @file Reservation.h
 * @brief Classe Reservation : associe UN passager à UN vol.
 *
 * Cycle de vie : EN_ATTENTE --valider()--> VALIDEE ; (EN_ATTENTE|VALIDEE) --annuler()--> ANNULEE
 */
#pragma once
#include <ctime>
#include "Types.h"

class Passager;
class Vol;
class Paiement;

class Reservation {
private:
    int id_;
    Passager* passager_;        // association obligatoire, multiplicité 1
    Vol* vol_;                  // association obligatoire, multiplicité 1
    CanalReservation canal_;
    StatutReservation statut_;
    time_t dateCreation_;
    int siege_;                 // 0 tant que la réservation n'est pas validée
    Paiement* paiement_;        // 0..1

public:
    /// @throws std::invalid_argument si passager ou vol est nul.
    Reservation(int id, Passager* p, Vol* v, CanalReservation canal);

    int getId() const { return id_; }
    Passager* getPassager() const { return passager_; }
    Vol* getVol() const { return vol_; }
    CanalReservation getCanal() const { return canal_; }
    StatutReservation getStatut() const { return statut_; }
    time_t getDateCreation() const { return dateCreation_; }
    int getSiege() const { return siege_; }
    Paiement* getPaiement() const { return paiement_; }

    bool estActive() const { return statut_ != StatutReservation::ANNULEE; }
    bool estValidee() const { return statut_ == StatutReservation::VALIDEE; }
    bool estAnnulee() const { return statut_ == StatutReservation::ANNULEE; }

    void setPaiement(Paiement* p) { paiement_ = p; }
    /// Valide la réservation : la place devient définitive.
    /// @throws std::logic_error si la réservation n'est pas EN_ATTENTE.
    void valider(int siege);
    /// @throws std::logic_error si déjà annulée.
    void annuler();

    void afficher() const;
    void afficherBillet() const;
    static void afficherEntete();
};
