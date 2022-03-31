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
void obtenirR16L16(uint64_t uint64, Message *m);
uint64_t K16EnUint64(int TK16[][TAILLE_BLOC]);
void generationSousCles(bool sousCles[][TAILLE_SOUS_CLE], bool *cle64Bits);
void F(bool *resultat, bool *Ri, bool *Ki);
uint64_t fonctionDES(uint64_t clair, uint64_t K64Bin);
uint64_t rechercheK16(uint64_t chiffreCorrect, uint64_t *chiffresFaux);
uint64_t rechercheK56(uint64_t clair, uint64_t chiffre, uint64_t K16);
uint64_t rechercheK64(uint64_t clair, uint64_t chiffre, uint64_t K56);

#endif