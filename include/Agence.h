/**
 * @file Agence.h
 * @brief Classe Agence : coeur métier. Elle POSSEDE les objets (composition) et
 *        applique toutes les règles de gestion. Aucune saisie/affichage ici
 *        (séparation logique métier / interface console).
 */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Passager.h"
#include "Vol.h"
#include "Reservation.h"
#include "Paiement.h"

/// Résumé statistique (fonctionnalité bonus).
struct Statistiques {
    int nbPassagers = 0, nbVols = 0;
    int resAttente = 0, resValidees = 0, resAnnulees = 0;
    long recettes = 0, rembourse = 0;
};

class Agence {
private:
    std::string nom_;
    // vector<unique_ptr> : l'agence est propriétaire, les adresses restent stables
    std::vector<std::unique_ptr<Passager>>    passagers_;
    std::vector<std::unique_ptr<Vol>>         vols_;
    std::vector<std::unique_ptr<Reservation>> reservations_;
    std::vector<std::unique_ptr<Paiement>>    paiements_;
    int prochainIdPassager_ = 1, prochainIdReservation_ = 1, prochainIdPaiement_ = 1;

public:
    explicit Agence(const std::string& nom) : nom_(nom) {}

    const std::string& getNom() const { return nom_; }

    // ---- 1. Passagers
    Passager* enregistrerPassager(const std::string& nom, const std::string& prenom,
                                  const std::string& tel, const std::string& email,
                                  const std::string& piece);
    Passager* trouverPassager(int id) const;
    std::vector<Passager*> listePassagers() const;

    // ---- 2/3/4. Vols
    Vol* ajouterVol(const std::string& numero, const std::string& destination,
                    time_t dateDepart, int capacite, long prix);
    Vol* trouverVol(const std::string& numero) const;
    std::vector<Vol*> volsDisponibles() const;
    std::vector<Vol*> rechercherParDestination(const std::string& motif) const;
    std::vector<Vol*> rechercherParDate(const std::string& date, bool& formatOk) const;

    // ---- 5. Réservations
    Reservation* creerReservation(int idPassager, const std::string& numVol, CanalReservation canal);
    Reservation* trouverReservation(int id) const;
    std::vector<Reservation*> listeReservations() const;

    // ---- 6. Paiements
    Paiement* enregistrerPaiement(int idReservation, long montant, ModePaiement mode);
    std::vector<Paiement*> listePaiements() const;

    // ---- 7. Annulation (+ remboursement)
    /// @return le paiement concerné (nullptr si la réservation n'était pas payée).
    Paiement* annulerReservation(int idReservation);
    /// Politique : >=72h -> 100 % ; 24h à 72h -> 50 % ; <24h -> 0 %.
    static int tauxRemboursement(long heuresAvantDepart);

    // ---- Bonus
    Statistiques statistiques() const;
    void chargerDonneesDemo();
};
