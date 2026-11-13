#include "Reservation.h"
#include "Passager.h"
#include "Vol.h"
#include "Paiement.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Reservation::Reservation(int id, Passager* p, Vol* v, CanalReservation canal)
    : id_(id), passager_(p), vol_(v), canal_(canal),
      statut_(StatutReservation::EN_ATTENTE), dateCreation_(std::time(nullptr)),
      siege_(0), paiement_(nullptr) {
    // REGLE : une reservation = exactement un passager + exactement un vol
    if (!p || !v) throw std::invalid_argument("Une reservation exige un passager et un vol.");
}

void Reservation::valider(int siege) {
    if (statut_ != StatutReservation::EN_ATTENTE)
        throw std::logic_error("Seule une reservation EN ATTENTE peut etre validee.");
    siege_ = siege;
    statut_ = StatutReservation::VALIDEE;   // la place devient definitive
}

void Reservation::annuler() {
    if (statut_ == StatutReservation::ANNULEE)
        throw std::logic_error("Cette reservation est deja annulee.");
    statut_ = StatutReservation::ANNULEE;
    siege_ = 0;                             // le siege est libere
}

void Reservation::afficherEntete() {
    std::cout << "  " << std::left << std::setw(6) << "N RES" << std::setw(22) << "PASSAGER"
              << std::setw(9) << "VOL" << std::setw(12) << "STATUT" << std::setw(18) << "CANAL"
              << std::setw(7) << "SIEGE" << "\n  " << std::string(76, '-') << "\n";
}
void Reservation::afficher() const {
    std::cout << "  " << std::left << std::setw(6) << id_ << std::setw(22) << passager_->nomComplet()
              << std::setw(9) << vol_->getNumero() << std::setw(12) << toString(statut_)
              << std::setw(18) << toString(canal_)
              << std::setw(7) << (siege_ > 0 ? std::to_string(siege_) : "-") << "\n";
}

void Reservation::afficherBillet() const {
    std::cout << "\n  +------------------------------------------------+\n"
              << "  |            BILLET D'AVION - AGENCE COTONOU     |\n"
              << "  +------------------------------------------------+\n"
              << "   Reservation : #" << id_ << "   (" << toString(canal_) << ")\n"
              << "   Passager    : " << passager_->nomComplet() << " (piece " << passager_->getNumeroPiece() << ")\n"
              << "   Vol         : " << vol_->getNumero() << "  " << vol_->getVilleDepart()
              << " -> " << vol_->getDestination() << "\n"
              << "   Depart      : " << Utils::formatDateHeure(vol_->getDateDepart()) << "\n"
              << "   Siege       : " << (siege_ > 0 ? std::to_string(siege_) : "non attribue") << "\n"
              << "   Statut      : " << toString(statut_) << "\n"
              << "  +------------------------------------------------+\n";
}
