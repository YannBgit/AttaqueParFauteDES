// DIRECTIVES
#ifndef ATTAQUEDES_H
#define ATTAQUEDES_H

// LIBRAIRIES
#include "constantes.h"
#include "structures.h"
#include "manipulationBin.h"
#include <string.h>

// FONCTIONS
void SboxFonction(bool *resultat, bool *entree, uint8_t numSbox);
void obtenirR15L15(uint64_t uint64, Message *m);
uint64_t K15EnUint64(int TK15[][TAILLE_BLOC]);
void generationSousCles(bool sousCles[][TAILLE_SOUS_CLE], bool *cle64Bits);
void F(bool *resultat, bool *Ri, bool *Ki);
uint64_t fonctionDES(uint64_t clair, uint64_t k64);
uint64_t rechercheK15(uint64_t chiffreCorrect, uint64_t *chiffresFaux);
uint64_t rechercheK56Bits(uint64_t clair, uint64_t chiffre, uint64_t K15);
uint64_t rechercheK(uint64_t clair, uint64_t chiffre, uint64_t K15);

#endif