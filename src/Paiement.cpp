#include "Paiement.h"
#include "Reservation.h"
#include "Passager.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Paiement::Paiement(int id, Reservation* r, long montant, ModePaiement mode)
    : id_(id), reservation_(r), montant_(montant), mode_(mode),
      date_(std::time(nullptr)), statut_(StatutPaiement::PAYE), montantRembourse_(0) {
    if (!r) throw std::invalid_argument("Un paiement exige une reservation.");
    if (montant <= 0) throw std::invalid_argument("Montant invalide.");
}

long Paiement::rembourser(int taux) {
    if (statut_ != StatutPaiement::PAYE)
        throw std::logic_error("Ce paiement a deja ete traite (remboursement).");
    if (taux < 0 || taux > 100) throw std::invalid_argument("Taux invalide.");
    montantRembourse_ = montant_ * taux / 100;
    if (taux == 100)     statut_ = StatutPaiement::REMBOURSE_TOTAL;
    else if (taux == 0)  statut_ = StatutPaiement::NON_REMBOURSABLE;
    else                 statut_ = StatutPaiement::REMBOURSE_PARTIEL;
    return montantRembourse_;
}

void Paiement::afficherEntete() {
    std::cout << "  " << std::left << std::setw(6) << "N PAY" << std::setw(6) << "RES"
              << std::setw(20) << "PASSAGER" << std::setw(16) << "MONTANT" << std::setw(16) << "MODE"
              << std::setw(26) << "STATUT" << "\n  " << std::string(88, '-') << "\n";
}
void Paiement::afficher() const {
    std::cout << "  " << std::left << std::setw(6) << id_ << std::setw(6) << reservation_->getId()
              << std::setw(20) << reservation_->getPassager()->nomComplet()
              << std::setw(16) << Utils::fcfa(montant_) << std::setw(16) << toString(mode_)
              << std::setw(26) << toString(statut_);
    if (montantRembourse_ > 0) std::cout << " (rembourse " << Utils::fcfa(montantRembourse_) << ")";
    std::cout << "\n";
}
