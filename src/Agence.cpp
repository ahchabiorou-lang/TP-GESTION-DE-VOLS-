#include "Agence.h"
#include "Utils.h"
#include <stdexcept>

// =============================================================== Passagers ==
Passager* Agence::enregistrerPassager(const std::string& nom, const std::string& prenom,
                                      const std::string& tel, const std::string& email,
                                      const std::string& piece) {
    if (!Utils::nomValide(nom) || !Utils::nomValide(prenom))
        throw std::invalid_argument("Nom/prenom invalides (non vides, sans chiffres).");
    if (!Utils::telephoneValide(tel)) throw std::invalid_argument("Telephone invalide (8 a 15 chiffres).");
    if (!Utils::emailValide(email))   throw std::invalid_argument("Email invalide.");
    if (piece.empty())                throw std::invalid_argument("Numero de piece obligatoire.");
    for (const auto& p : passagers_)   // unicité : une seule fiche par pièce d'identité
        if (Utils::majuscules(p->getNumeroPiece()) == Utils::majuscules(piece))
            throw std::runtime_error("Ce passager est deja enregistre (meme numero de piece).");
    passagers_.push_back(std::make_unique<Passager>(prochainIdPassager_++, nom, prenom, tel, email, piece));
    return passagers_.back().get();
}
Passager* Agence::trouverPassager(int id) const {
    for (const auto& p : passagers_) if (p->getId() == id) return p.get();
    return nullptr;
}
std::vector<Passager*> Agence::listePassagers() const {
    std::vector<Passager*> r;
    for (const auto& p : passagers_) r.push_back(p.get());
    return r;
}

// ==================================================================== Vols ==
Vol* Agence::ajouterVol(const std::string& numero, const std::string& destination,
                        time_t dateDepart, int capacite, long prix) {
    std::string num = Utils::majuscules(numero);
    if (trouverVol(num)) throw std::runtime_error("Un vol portant ce numero existe deja.");
    if (std::difftime(dateDepart, std::time(nullptr)) <= 0)
        throw std::invalid_argument("La date de depart doit etre dans le futur.");
    vols_.push_back(std::make_unique<Vol>(num, "Cotonou", destination, dateDepart, capacite, prix));
    return vols_.back().get();
}
Vol* Agence::trouverVol(const std::string& numero) const {
    std::string n = Utils::majuscules(numero);
    for (const auto& v : vols_) if (v->getNumero() == n) return v.get();
    return nullptr;
}
std::vector<Vol*> Agence::volsDisponibles() const {
    std::vector<Vol*> r;
    for (const auto& v : vols_) if (!v->estPasse() && !v->estComplet()) r.push_back(v.get());
    return r;
}
std::vector<Vol*> Agence::rechercherParDestination(const std::string& motif) const {
    std::vector<Vol*> r;
    std::string m = Utils::minuscules(motif);
    for (const auto& v : vols_)
        if (Utils::minuscules(v->getDestination()).find(m) != std::string::npos) r.push_back(v.get());
    return r;
}
std::vector<Vol*> Agence::rechercherParDate(const std::string& date, bool& formatOk) const {
    std::vector<Vol*> r;
    time_t test;
    formatOk = Utils::parseDateHeure(date, "00:00", test);   // format JJ/MM/AAAA valide ?
    if (!formatOk) return r;
    for (const auto& v : vols_) {
        bool ok;
        if (Utils::memeJour(v->getDateDepart(), date, ok)) r.push_back(v.get());
    }
    return r;
}

// ============================================================ Réservations ==
Reservation* Agence::creerReservation(int idPassager, const std::string& numVol, CanalReservation canal) {
    // REGLE 1 : le passager doit être enregistré
    Passager* p = trouverPassager(idPassager);
    if (!p) throw std::runtime_error("Passager non enregistre : enregistrez-le d'abord (option 1).");
    Vol* v = trouverVol(numVol);
    if (!v) throw std::runtime_error("Vol introuvable.");
    if (v->estPasse()) throw std::runtime_error("Ce vol est deja parti.");
    // REGLE 3 : pas deux fois le même vol
    if (p->aReservationActiveSur(v))
        throw std::runtime_error("Ce passager a deja une reservation sur ce vol.");
    // REGLE 2 : capacité
    if (v->estComplet()) throw std::runtime_error("Vol complet : plus aucune place disponible.");

    auto r = std::make_unique<Reservation>(prochainIdReservation_, p, v, canal);
    v->ajouterReservation(r.get());   // association Vol -> Reservation
    p->ajouterReservation(r.get());   // association Passager -> Reservation
    ++prochainIdReservation_;
    reservations_.push_back(std::move(r));
    return reservations_.back().get();
}
Reservation* Agence::trouverReservation(int id) const {
    for (const auto& r : reservations_) if (r->getId() == id) return r.get();
    return nullptr;
}
std::vector<Reservation*> Agence::listeReservations() const {
    std::vector<Reservation*> r;
    for (const auto& x : reservations_) r.push_back(x.get());
    return r;
}

// ================================================================ Paiements ==
Paiement* Agence::enregistrerPaiement(int idReservation, long montant, ModePaiement mode) {
    Reservation* r = trouverReservation(idReservation);
    if (!r) throw std::runtime_error("Reservation introuvable.");
    // REGLE 5 : une réservation annulée ne peut plus être payée
    if (r->estAnnulee()) throw std::runtime_error("Reservation annulee : paiement impossible.");
    if (r->getPaiement()) throw std::runtime_error("Cette reservation est deja payee.");
    if (r->getVol()->estPasse()) throw std::runtime_error("Le vol est deja parti.");
    if (montant != r->getVol()->getPrix())
        throw std::invalid_argument("Montant incorrect : il faut regler exactement " +
                                    Utils::fcfa(r->getVol()->getPrix()) + ".");
    auto p = std::make_unique<Paiement>(prochainIdPaiement_++, r, montant, mode);
    r->setPaiement(p.get());
    // REGLE 6 : la place est définitive uniquement après VALIDATION (ici : par le paiement)
    r->valider(r->getVol()->prochainSiegeLibre());
    paiements_.push_back(std::move(p));
    return paiements_.back().get();
}
std::vector<Paiement*> Agence::listePaiements() const {
    std::vector<Paiement*> r;
    for (const auto& x : paiements_) r.push_back(x.get());
    return r;
}

// =============================================================== Annulation ==
int Agence::tauxRemboursement(long h) {
    if (h >= 72) return 100;
    if (h >= 24) return 50;
    return 0;
}
Paiement* Agence::annulerReservation(int idReservation) {
    Reservation* r = trouverReservation(idReservation);
    if (!r) throw std::runtime_error("Reservation introuvable.");
    if (r->estAnnulee()) throw std::runtime_error("Cette reservation est deja annulee.");
    if (r->getVol()->estPasse()) throw std::runtime_error("Le vol est deja parti : annulation impossible.");
    Paiement* p = r->getPaiement();
    if (p) p->rembourser(tauxRemboursement(r->getVol()->heuresAvantDepart())); // statut de remboursement
    r->annuler();   // libère la place automatiquement (placesOccupees ne compte que les actives)
    return p;
}

// ==================================================================== Bonus ==
Statistiques Agence::statistiques() const {
    Statistiques s;
    s.nbPassagers = (int)passagers_.size();
    s.nbVols = (int)vols_.size();
    for (const auto& r : reservations_) {
        switch (r->getStatut()) {
            case StatutReservation::EN_ATTENTE: ++s.resAttente; break;
            case StatutReservation::VALIDEE:    ++s.resValidees; break;
            default:                            ++s.resAnnulees; break;
        }
    }
    for (const auto& p : paiements_) { s.recettes += p->getMontant(); s.rembourse += p->getMontantRembourse(); }
    return s;
}

void Agence::chargerDonneesDemo() {
    time_t now = std::time(nullptr);
    const time_t jour = 24 * 3600;
    ajouterVol("BJ101", "Paris",   now + 10 * jour, 3,  450000);
    ajouterVol("BJ202", "Abidjan", now + 5 * jour,  50, 150000);
    ajouterVol("BJ303", "Lagos",   now + 12 * 3600, 20, 80000);   // départ dans 12 h
    enregistrerPassager("Adjovi", "Koffi", "+22997000001", "koffi@mail.com", "BJ-PASS-001");
    enregistrerPassager("Dossou", "Aicha", "+22996000002", "aicha@mail.com", "BJ-PASS-002");
}
