/**
 * @file Passager.h
 * @brief Classe Passager : identité d'un client enregistré par l'agence.
 */
#pragma once
#include <string>
#include <vector>

class Reservation;
class Vol;

class Passager {
private:                                    // ENCAPSULATION : attributs privés
    int id_;
    std::string nom_, prenom_, telephone_, email_, numeroPiece_;
    std::vector<Reservation*> reservations_;  // association 1 -> 0..* (non propriétaire)

public:
    Passager(int id, const std::string& nom, const std::string& prenom,
             const std::string& telephone, const std::string& email,
             const std::string& numeroPiece);

    // Accesseurs
    int getId() const { return id_; }
    const std::string& getNom() const { return nom_; }
    const std::string& getPrenom() const { return prenom_; }
    const std::string& getTelephone() const { return telephone_; }
    const std::string& getEmail() const { return email_; }
    const std::string& getNumeroPiece() const { return numeroPiece_; }
    const std::vector<Reservation*>& getReservations() const { return reservations_; }
    std::string nomComplet() const { return prenom_ + " " + nom_; }

    // Méthodes métier
    void ajouterReservation(Reservation* r);
    /// Règle : un passager ne réserve pas 2 fois le même vol (réservation active).
    bool aReservationActiveSur(const Vol* vol) const;
    void afficher() const;
};
