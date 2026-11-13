/**
 * @file Utils.h
 * @brief Fonctions utilitaires : saisies sécurisées (gestion des erreurs de saisie),
 *        validation de formats et manipulation des dates.
 */
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <stdexcept>

namespace Utils {

/// Exception lancée quand le flux d'entrée est fermé (Ctrl+D / Ctrl+Z).
struct FinDeSaisie {};

inline std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}
inline std::string majuscules(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
}
inline std::string minuscules(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

// ---------------------------------------------------------------- Saisies --
/// Lit une ligne complète (jamais de cin >> : évite les flux corrompus).
inline std::string lireLigne(const std::string& invite) {
    std::cout << invite;
    std::string l;
    if (!std::getline(std::cin, l)) throw FinDeSaisie();
    return trim(l);
}
/// Redemande tant que la saisie est vide.
inline std::string lireTexte(const std::string& invite) {
    while (true) {
        std::string s = lireLigne(invite);
        if (!s.empty()) return s;
        std::cout << "  [Erreur] Ce champ ne peut pas etre vide.\n";
    }
}
/// Lit un entier dans [min, max]; redemande en cas d'erreur.
inline long lireEntier(const std::string& invite, long min, long max) {
    while (true) {
        std::string s = lireLigne(invite);
        try {
            size_t pos = 0;
            long v = std::stol(s, &pos);
            if (pos != s.size()) throw std::invalid_argument("reste");
            if (v < min || v > max) {
                std::cout << "  [Erreur] Valeur attendue entre " << min << " et " << max << ".\n";
                continue;
            }
            return v;
        } catch (const std::exception&) {
            std::cout << "  [Erreur] Veuillez saisir un nombre entier valide.\n";
        }
    }
}
/// Question fermée Oui/Non.
inline bool confirmer(const std::string& question) {
    while (true) {
        std::string r = minuscules(lireLigne(question + " (o/n) : "));
        if (r == "o" || r == "oui") return true;
        if (r == "n" || r == "non") return false;
        std::cout << "  [Erreur] Repondez par 'o' ou 'n'.\n";
    }
}

// ------------------------------------------------------------- Validation --
inline bool nomValide(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) if (std::isdigit(c)) return false;
    return true;
}
inline bool emailValide(const std::string& s) {
    size_t at = s.find('@');
    if (at == std::string::npos || at == 0 || s.find(' ') != std::string::npos) return false;
    size_t dot = s.find('.', at);
    return dot != std::string::npos && dot > at + 1 && dot + 1 < s.size();
}
inline bool telephoneValide(const std::string& s) {
    int chiffres = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char c = s[i];
        if (std::isdigit(c)) ++chiffres;
        else if (!(c == '+' && i == 0) && c != ' ') return false;
    }
    return chiffres >= 8 && chiffres <= 15;
}

// ------------------------------------------------------------------ Dates --
/// Convertit "JJ/MM/AAAA" + "HH:MM" en time_t. Retourne false si invalide.
inline bool parseDateHeure(const std::string& date, const std::string& heure, time_t& out) {
    int j, m, a, h, mi, n1 = 0, n2 = 0;
    if (std::sscanf(date.c_str(), "%d/%d/%d%n", &j, &m, &a, &n1) != 3 || n1 != (int)date.size()) return false;
    if (std::sscanf(heure.c_str(), "%d:%d%n", &h, &mi, &n2) != 2 || n2 != (int)heure.size()) return false;
    std::tm t = {};
    t.tm_mday = j; t.tm_mon = m - 1; t.tm_year = a - 1900;
    t.tm_hour = h; t.tm_min = mi; t.tm_isdst = -1;
    std::tm copie = t;
    time_t r = std::mktime(&t);
    if (r == (time_t)-1) return false;
    // Rejette 31/02, 25:70, etc. (mktime "normalise" au lieu de refuser)
    if (t.tm_mday != copie.tm_mday || t.tm_mon != copie.tm_mon || t.tm_year != copie.tm_year ||
        t.tm_hour != copie.tm_hour || t.tm_min != copie.tm_min) return false;
    out = r;
    return true;
}
inline std::string formatDateHeure(time_t t) {
    char buf[32];
    std::tm* lt = std::localtime(&t);
    std::strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", lt);
    return buf;
}
/// Vrai si t tombe le jour "JJ/MM/AAAA" ; ok = false si le format est invalide.
inline bool memeJour(time_t t, const std::string& date, bool& ok) {
    time_t debut;
    ok = parseDateHeure(date, "00:00", debut);
    if (!ok) return false;
    return t >= debut && t < debut + 24 * 3600;
}
/// Format monétaire simple : 150000 -> "150 000 FCFA".
inline std::string fcfa(long v) {
    std::string s = std::to_string(v), r;
    int c = 0;
    for (int i = (int)s.size() - 1; i >= 0; --i) {
        r.insert(r.begin(), s[i]);
        if (++c % 3 == 0 && i > 0) r.insert(r.begin(), ' ');
    }
    return r + " FCFA";
}

} // namespace Utils
