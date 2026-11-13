#include "Menu.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace Utils;

void Menu::titre(const string& t) {
    cout << "\n==================================================\n  " << t
         << "\n==================================================\n";
}
void Menu::pause() { lireLigne("\n  [Entree] pour continuer..."); }

void Menu::afficherMenuPrincipal() const {
    titre("AGENCE " + majuscules(agence_.getNom()) + " - GESTION DES VOLS");
    cout << "  1. Enregistrer un passager\n"
         << "  2. Ajouter un vol\n"
         << "  3. Afficher les vols disponibles\n"
         << "  4. Rechercher un vol\n"
         << "  5. Reservations\n"
         << "  6. Enregistrer un paiement\n"
         << "  7. Annuler une reservation\n"
         << "  --- Consultation & bonus ---\n"
         << "  8. Liste des passagers\n"
         << "  9. Liste des paiements\n"
         << " 10. Statistiques de l'agence\n"
         << " 11. Charger des donnees de demonstration\n"
         << "  0. Quitter\n";
}

void Menu::lancer() {
    try {
        while (true) {
            afficherMenuPrincipal();
            int choix = (int)lireEntier("  Votre choix : ", 0, 11);
            try {   // les erreurs metier (exceptions) sont affichees sans quitter le programme
                switch (choix) {
                    case 1: actionEnregistrerPassager(); break;
                    case 2: actionAjouterVol(); break;
                    case 3: actionAfficherVolsDisponibles(); break;
                    case 4: actionRechercherVol(); break;
                    case 5: menuReservations(); break;
                    case 6: actionEnregistrerPaiement(); break;
                    case 7: actionAnnulerReservation(); break;
                    case 8: actionListerPassagers(); break;
                    case 9: actionListerPaiements(); break;
                    case 10: actionStatistiques(); break;
                    case 11:
                        agence_.chargerDonneesDemo();
                        cout << "  [OK] Donnees de demonstration chargees (3 vols, 2 passagers).\n";
                        break;
                    case 0: cout << "\n  Au revoir !\n"; return;
                }
            } catch (const FinDeSaisie&) {
                throw;
            } catch (const exception& e) {
                cout << "\n  [ERREUR] " << e.what() << "\n";
            }
            if (choix != 0) pause();
        }
    } catch (const FinDeSaisie&) {
        cout << "\n  Fin de saisie detectee. Fermeture du programme.\n";
    }
}

// ---------------------------------------------------------------- 1 ---------
void Menu::actionEnregistrerPassager() {
    titre("ENREGISTRER UN PASSAGER");
    string nom, prenom, tel, email;
    do { nom = lireTexte("  Nom            : ");
         if (!nomValide(nom)) cout << "  [Erreur] Le nom ne doit pas contenir de chiffres.\n";
    } while (!nomValide(nom));
    do { prenom = lireTexte("  Prenom         : ");
         if (!nomValide(prenom)) cout << "  [Erreur] Le prenom ne doit pas contenir de chiffres.\n";
    } while (!nomValide(prenom));
    do { tel = lireTexte("  Telephone      : ");
         if (!telephoneValide(tel)) cout << "  [Erreur] 8 a 15 chiffres (ex: +22997000000).\n";
    } while (!telephoneValide(tel));
    do { email = lireTexte("  Email          : ");
         if (!emailValide(email)) cout << "  [Erreur] Format attendu : nom@domaine.xx\n";
    } while (!emailValide(email));
    string piece = lireTexte("  N piece (CNI/passeport) : ");
    Passager* p = agence_.enregistrerPassager(nom, prenom, tel, email, piece);
    cout << "\n  [OK] Passager enregistre. Identifiant : " << p->getId() << "\n";
}

// ---------------------------------------------------------------- 2 ---------
void Menu::actionAjouterVol() {
    titre("AJOUTER UN VOL (depart de Cotonou)");
    string numero = lireTexte("  Numero du vol (ex: BJ101) : ");
    string dest   = lireTexte("  Destination               : ");
    time_t dt;
    while (true) {
        string d = lireTexte("  Date de depart (JJ/MM/AAAA): ");
        string h = lireTexte("  Heure de depart (HH:MM)    : ");
        if (!parseDateHeure(d, h, dt)) {
            cout << "  [Erreur] Date ou heure invalide (format JJ/MM/AAAA et HH:MM).\n";
        } else if (difftime(dt, time(nullptr)) <= 0) {
            cout << "  [Erreur] La date de depart doit etre dans le futur.\n";
        } else break;
    }
    int cap = (int)lireEntier("  Capacite (nombre de places): ", 1, 1000);
    long prix = lireEntier("  Prix du billet (FCFA)      : ", 1, 100000000);
    Vol* v = agence_.ajouterVol(numero, dest, dt, cap, prix);
    cout << "\n  [OK] Vol " << v->getNumero() << " ajoute : Cotonou -> " << v->getDestination() << "\n";
}

// ------------------------------------------------------------ 3 et 4 --------
void Menu::afficherListeVols(const vector<Vol*>& vols) {
    if (vols.empty()) { cout << "\n  Aucun vol trouve.\n"; return; }
    cout << "\n";
    Vol::afficherEntete();
    for (const Vol* v : vols) v->afficher();
    cout << "\n  " << vols.size() << " vol(s).\n";
}
void Menu::actionAfficherVolsDisponibles() const {
    titre("VOLS DISPONIBLES");
    afficherListeVols(agence_.volsDisponibles());
}
void Menu::actionRechercherVol() const {
    titre("RECHERCHER UN VOL");
    cout << "  1. Par destination\n  2. Par date\n  3. Par numero de vol\n";
    int c = (int)lireEntier("  Critere : ", 1, 3);
    if (c == 1) {
        afficherListeVols(agence_.rechercherParDestination(lireTexte("  Destination (ou debut) : ")));
    } else if (c == 2) {
        bool ok;
        auto r = agence_.rechercherParDate(lireTexte("  Date (JJ/MM/AAAA) : "), ok);
        if (!ok) cout << "\n  [Erreur] Date invalide.\n"; else afficherListeVols(r);
    } else {
        Vol* v = agence_.trouverVol(lireTexte("  Numero de vol : "));
        afficherListeVols(v ? vector<Vol*>{v} : vector<Vol*>{});
    }
}

// ---------------------------------------------------------------- 5 ---------
void Menu::menuReservations() {
    while (true) {
        titre("RESERVATIONS");
        cout << "  1. Nouvelle reservation\n  2. Lister les reservations\n"
             << "  3. Afficher le billet d'une reservation\n  0. Retour\n";
        int c = (int)lireEntier("  Choix : ", 0, 3);
        try {
            if (c == 0) return;
            if (c == 1) actionNouvelleReservation();
            if (c == 2) actionListerReservations();
            if (c == 3) actionAfficherBillet();
        } catch (const FinDeSaisie&) { throw;
        } catch (const exception& e) { cout << "\n  [ERREUR] " << e.what() << "\n"; }
    }
}
void Menu::actionNouvelleReservation() {
    titre("NOUVELLE RESERVATION");
    int idP = (int)lireEntier("  Identifiant du passager : ", 1, 1000000);
    if (!agence_.trouverPassager(idP)) {
        cout << "\n  [ERREUR] Passager non enregistre. Utilisez d'abord l'option 1 du menu principal.\n";
        return;
    }
    cout << "\n  Vols disponibles :\n";
    afficherListeVols(agence_.volsDisponibles());
    string num = lireTexte("\n  Numero du vol : ");
    cout << "  Canal : 1. Accueil (agence)   2. Site web\n";
    CanalReservation canal = lireEntier("  Canal : ", 1, 2) == 1 ? CanalReservation::ACCUEIL
                                                                : CanalReservation::SITE_WEB;
    Reservation* r = agence_.creerReservation(idP, num, canal);
    cout << "\n  [OK] Reservation #" << r->getId() << " creee (EN ATTENTE).\n"
         << "  La place sera DEFINITIVE apres paiement/validation (option 6).\n"
         << "  Prix a regler : " << fcfa(r->getVol()->getPrix()) << "\n";
}
void Menu::actionListerReservations() const {
    titre("LISTE DES RESERVATIONS");
    auto l = agence_.listeReservations();
    if (l.empty()) { cout << "\n  Aucune reservation.\n"; return; }
    cout << "\n"; Reservation::afficherEntete();
    for (const Reservation* r : l) r->afficher();
}
void Menu::actionAfficherBillet() const {
    int id = (int)lireEntier("  N de reservation : ", 1, 1000000);
    Reservation* r = agence_.trouverReservation(id);
    if (!r) throw runtime_error("Reservation introuvable.");
    r->afficherBillet();
}

// ---------------------------------------------------------------- 6 ---------
void Menu::actionEnregistrerPaiement() {
    titre("ENREGISTRER UN PAIEMENT");
    int id = (int)lireEntier("  N de reservation : ", 1, 1000000);
    Reservation* r = agence_.trouverReservation(id);
    if (!r) throw runtime_error("Reservation introuvable.");
    if (r->estAnnulee()) throw runtime_error("Reservation annulee : paiement impossible.");
    cout << "  Passager : " << r->getPassager()->nomComplet() << " | Vol " << r->getVol()->getNumero()
         << " | Montant du : " << fcfa(r->getVol()->getPrix()) << "\n";
    cout << "  Mode : 1. Especes  2. Mobile Money  3. Carte bancaire\n";
    int m = (int)lireEntier("  Mode de paiement : ", 1, 3);
    long montant = lireEntier("  Montant verse (FCFA) : ", 1, 100000000);
    Paiement* p = agence_.enregistrerPaiement(id, montant, static_cast<ModePaiement>(m - 1));
    cout << "\n  [OK] Paiement #" << p->getId() << " enregistre. Reservation VALIDEE.\n";
    r->afficherBillet();
}

// ---------------------------------------------------------------- 7 ---------
void Menu::actionAnnulerReservation() {
    titre("ANNULER UNE RESERVATION");
    int id = (int)lireEntier("  N de reservation : ", 1, 1000000);
    Reservation* r = agence_.trouverReservation(id);
    if (!r) throw runtime_error("Reservation introuvable.");
    cout << "  Politique de remboursement : >=72h avant depart: 100% | 24h-72h: 50% | <24h: 0%\n";
    if (!confirmer("  Confirmer l'annulation de la reservation #" + to_string(id) + " ?")) {
        cout << "  Annulation abandonnee.\n"; return;
    }
    Paiement* p = agence_.annulerReservation(id);
    cout << "\n  [OK] Reservation #" << id << " annulee. La place est liberee.\n";
    if (!p) cout << "  Aucun paiement associe : rien a rembourser.\n";
    else cout << "  Statut du paiement : " << toString(p->getStatut())
              << " | Montant rembourse : " << fcfa(p->getMontantRembourse()) << "\n";
}

// --------------------------------------------------------------- bonus ------
void Menu::actionListerPassagers() const {
    titre("LISTE DES PASSAGERS");
    auto l = agence_.listePassagers();
    if (l.empty()) { cout << "\n  Aucun passager.\n"; return; }
    for (const Passager* p : l) p->afficher();
}
void Menu::actionListerPaiements() const {
    titre("LISTE DES PAIEMENTS");
    auto l = agence_.listePaiements();
    if (l.empty()) { cout << "\n  Aucun paiement.\n"; return; }
    cout << "\n"; Paiement::afficherEntete();
    for (const Paiement* p : l) p->afficher();
}
void Menu::actionStatistiques() const {
    titre("STATISTIQUES");
    Statistiques s = agence_.statistiques();
    cout << "  Passagers enregistres : " << s.nbPassagers << "\n"
         << "  Vols programmes       : " << s.nbVols << "\n"
         << "  Reservations          : " << s.resAttente << " en attente, " << s.resValidees
         << " validees, " << s.resAnnulees << " annulees\n"
         << "  Encaissements         : " << fcfa(s.recettes) << "\n"
         << "  Rembourse             : " << fcfa(s.rembourse) << "\n"
         << "  Recette nette         : " << fcfa(s.recettes - s.rembourse) << "\n";
}
