/**
 * @file Types.h
 * @brief Énumérations métier du système + conversions en texte.
 */
#pragma once
#include <string>

/// Cycle de vie d'une réservation (voir diagramme d'états).
enum class StatutReservation { EN_ATTENTE, VALIDEE, ANNULEE };

/// Situation financière d'un paiement (statut de remboursement inclus).
enum class StatutPaiement { PAYE, REMBOURSE_TOTAL, REMBOURSE_PARTIEL, NON_REMBOURSABLE };

/// Canal par lequel le client réserve (amélioration : accueil ou site web).
enum class CanalReservation { ACCUEIL, SITE_WEB };

/// Modes de paiement acceptés par l'agence.
enum class ModePaiement { ESPECES, MOBILE_MONEY, CARTE_BANCAIRE };

inline std::string toString(StatutReservation s) {
    switch (s) {
        case StatutReservation::EN_ATTENTE: return "EN ATTENTE";
        case StatutReservation::VALIDEE:    return "VALIDEE";
        default:                            return "ANNULEE";
    }
}
inline std::string toString(StatutPaiement s) {
    switch (s) {
        case StatutPaiement::PAYE:              return "PAYE";
        case StatutPaiement::REMBOURSE_TOTAL:   return "REMBOURSE TOTALEMENT";
        case StatutPaiement::REMBOURSE_PARTIEL: return "REMBOURSE PARTIELLEMENT";
        default:                                return "NON REMBOURSABLE";
    }
}
inline std::string toString(CanalReservation c) {
    return c == CanalReservation::ACCUEIL ? "ACCUEIL (agence)" : "SITE WEB";
}
inline std::string toString(ModePaiement m) {
    switch (m) {
        case ModePaiement::ESPECES:      return "ESPECES";
        case ModePaiement::MOBILE_MONEY: return "MOBILE MONEY";
        default:                         return "CARTE BANCAIRE";
    }
}
