// DIRECTIVES
#ifndef ATTAQUEDES_H
#define ATTAQUEDES_H

// LIBRAIRIES
#include "constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// STRUCTURES
// Structure représentant les étapes de chiffrement d'un message
typedef struct message
{
	uint64_t chiffreuint64;
	bool chiffreBin[TAILLE_BLOC];
	bool chiffreBinPermute[TAILLE_BLOC];
	bool LChiffreBin[TAILLE_DEMI_BLOC];
	bool RChiffreBin[TAILLE_DEMI_BLOC];
	bool RChiffreBinE[TAILLE_SOUS_CLE];
	bool Sbox6BitsBin[TAILLE_ENTREE_SBOX];
	bool Sbox6BitsXoreBin[TAILLE_ENTREE_SBOX];
	bool Sbox4BitsBin[TAILLE_SORTIE_SBOX];
} Message;

// Structure de stockage des clés
typedef struct cle
{
	bool cle48Bin[TAILLE_SOUS_CLE];
	bool cle56Bin[TAILLE_CLE_MAITRE_SANS_PARITE];
	bool cle64Bin[TAILLE_BLOC];
	bool cle8Bin[NOMBRE_SBOXES];
} Cle;

// Structure de stockage des variables nécessaires au déroulement d'un DES
typedef struct des
{
	bool clairBin[TAILLE_BLOC];
	bool cle64Bin[TAILLE_BLOC];
	bool clairBinIP[TAILLE_BLOC];
	bool R32Bin[TAILLE_DEMI_BLOC];
	bool L32Bin[TAILLE_DEMI_BLOC];
	bool R32BinPlus1[TAILLE_DEMI_BLOC];
	bool L32BinPlus1[TAILLE_DEMI_BLOC];
	bool R48Bin[TAILLE_SOUS_CLE];
	bool sousCles[NOMBRE_SOUS_CLES][TAILLE_SOUS_CLE];
	bool chiffreBin[TAILLE_BLOC];
} DES;

// FONCTIONS
void uint64EnBin(bool *resultatTab, uint64_t uint64, uint8_t nombreBits);
void decimalEnBin(bool *resultatTab, int decimal, uint8_t nombreBits);
uint64_t tabEnUint64(bool *tab, uint8_t nombreBits);
void permutation(bool *resultat, bool *aPermute, const uint8_t *matriceP, uint8_t nombreBits);
void separationTab(bool *completTab, bool *LTab, bool *RTab, uint8_t nombreBits);
void xor(bool *resultatTab, bool *tab1, bool *tab2, uint8_t nombreBits);
uint64_t bitFaute(bool *correcteTab, bool *fauxTab);
void SboxFonction(bool *resultat, bool *entree, uint8_t numSbox);
void obtenirR16L16(uint64_t uint64, Message *m);
void extraire6Bits(Message *m, uint8_t position);
bool tabEgaux(bool *tab1, bool *tab2, uint8_t nombreBits);
uint64_t K16Enuint64(int tabK16[8][64]);
uint64_t rechercheK16(uint64_t chiffreCorrect, uint64_t *chiffresFaux);
void initTab(bool *tab, uint8_t nombreBits);
void decalageGauche(bool *resultat, bool *tabAshifter, uint8_t nombreShifts, uint8_t nombreBits);
void fusionTab(bool *resultat, bool *LTab, bool *RTab, uint8_t nombreBits);
void generationSousCles(bool sousCles[][TAILLE_SOUS_CLE], bool *cle64Bits);
void F(bool *resultat, bool *Ri, bool *Ki);
void copieTab(bool *resultat, bool *aCopier, uint8_t nombreBits);
uint64_t fonctionDES(uint64_t clair, uint64_t k64);
uint64_t rechercheK56Bits(uint64_t clair, uint64_t chiffre, uint64_t k16);
uint64_t rechercheK(uint64_t clair, uint64_t chiffre, uint64_t k16);

#endif