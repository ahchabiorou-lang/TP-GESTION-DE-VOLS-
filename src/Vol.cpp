#include "Vol.h"
#include "Reservation.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

Vol::Vol(const std::string& numero, const std::string& villeDepart,
         const std::string& destination, time_t dateDepart, int capacite, long prix)
    : numero_(numero), villeDepart_(villeDepart), destination_(destination),
      dateDepart_(dateDepart), capacite_(capacite), prix_(prix) {
    if (capacite <= 0) throw std::invalid_argument("La capacite doit etre positive.");
    if (prix <= 0)     throw std::invalid_argument("Le prix doit etre positif.");
    if (numero.empty() || destination.empty()) throw std::invalid_argument("Numero/destination vides.");
}

int Vol::placesOccupees() const {
    int n = 0;
    for (const Reservation* r : reservations_) if (r->estActive()) ++n;
    return n;
}
int Vol::placesValidees() const {
    int n = 0;
    for (const Reservation* r : reservations_) if (r->estValidee()) ++n;
    return n;
}
bool Vol::estPasse() const { return std::difftime(dateDepart_, std::time(nullptr)) <= 0; }

long Vol::heuresAvantDepart() const {
    return (long)(std::difftime(dateDepart_, std::time(nullptr)) / 3600.0);
}

int Vol::prochainSiegeLibre() const {
    std::vector<int> pris;
    for (const Reservation* r : reservations_)
        if (r->estValidee()) pris.push_back(r->getSiege());
    for (int s = 1; s <= capacite_; ++s)
        if (std::find(pris.begin(), pris.end(), s) == pris.end()) return s;
    return -1;
}

void Vol::ajouterReservation(Reservation* r) {
    if (!r) throw std::invalid_argument("Reservation nulle.");
    // REGLE : jamais plus de reservations actives que la capacite
    if (estComplet()) throw std::runtime_error("Vol complet : capacite maximale atteinte.");
    reservations_.push_back(r);
}

void Vol::afficherEntete() {
    std::cout << "  " << std::left << std::setw(9) << "N VOL" << std::setw(11) << "DEPART"
              << std::setw(18) << "DESTINATION" << std::setw(18) << "DATE/HEURE"
              << std::setw(10) << "PLACES" << std::setw(14) << "PRIX" << "\n"
              << "  " << std::string(80, '-') << "\n";
}
void Vol::afficher() const {
    std::string places = std::to_string(placesDisponibles()) + "/" + std::to_string(capacite_);
    std::cout << "  " << std::left << std::setw(9) << numero_ << std::setw(11) << villeDepart_
              << std::setw(18) << destination_ << std::setw(18) << Utils::formatDateHeure(dateDepart_)
              << std::setw(10) << places << std::setw(14) << Utils::fcfa(prix_)
              << (estComplet() ? " COMPLET" : "") << "\n";
}
