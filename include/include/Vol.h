/**
 * @file Vol.h
 * @brief Classe Vol : un vol au départ de Cotonou avec une capacité limitée.
 */
#pragma once
#include <string>
#include <vector>
#include <ctime>

class Reservation;

class Vol {
private:
    std::string numero_, villeDepart_, destination_;
    time_t dateDepart_;
    int capacite_;
    long prix_;                               // en FCFA
    std::vector<Reservation*> reservations_;  // association 1 -> 0..capacite

public:
    /// @throws std::invalid_argument si capacité/prix invalides.
    Vol(const std::string& numero, const std::string& villeDepart,
        const std::string& destination, time_t dateDepart, int capacite, long prix);

    const std::string& getNumero() const { return numero_; }
    const std::string& getVilleDepart() const { return villeDepart_; }
    const std::string& getDestination() const { return destination_; }
    time_t getDateDepart() const { return dateDepart_; }
    int getCapacite() const { return capacite_; }
    long getPrix() const { return prix_; }
    const std::vector<Reservation*>& getReservations() const { return reservations_; }

    int placesOccupees() const;       ///< réservations actives (en attente + validées)
    int placesValidees() const;       ///< places définitivement réservées
    int placesDisponibles() const { return capacite_ - placesOccupees(); }
    bool estComplet() const { return placesDisponibles() <= 0; }
    bool estPasse() const;            ///< vrai si le vol est déjà parti
    int prochainSiegeLibre() const;   ///< plus petit n° de siège non attribué
    long heuresAvantDepart() const;

    void ajouterReservation(Reservation* r);
    void afficher() const;
    static void afficherEntete();
};
