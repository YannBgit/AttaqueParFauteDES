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
	uint64_t chiffreHex;
	bool chiffreBin[64];
	bool chiffreBinPermute[64];
	bool LChiffreBin[32];
	bool RChiffreBin[32];
	bool RChiffreBinE[48];
	bool Sbox6BitsBin[6];
	bool Sbox6BitsXoreBin[6];
	bool Sbox4BitsBin[4];
} Message;

// Structure de stockage des clés
typedef struct cle
{
	bool cle48Bin[48];
	bool cle56Bin[56];
	bool cle64Bin[64];
	bool cle8Bin[8];
} Cle;

// Structure de stockage des variables nécessaires au déroulement d'un DES
typedef struct des
{
	bool clairBin[64];
	bool cle64Bin[64];
	bool clairBinIP[64];
	bool R32Bin[32];
	bool L32Bin[32];
	bool R32BinPlus1[32];
	bool L32BinPlus1[32];
	bool R48Bin[48];
	bool sousCles[16][48];
	bool chiffreBin[64];
} DES;

// FONCTIONS
void uint64EnBin(bool *resultatTab, uint64_t hex);
void decimalEnBin(bool *resultatTab, uint64_t decimal, uint8_t nombreBits);
uint64_t binEnUint64(bool *tab, uint8_t nombreBits);
void permutation(bool *resultat, bool *aPermute, const uint8_t *matriceP, uint8_t nombreBits);
void separationTab(bool *completTab, bool *LTab, bool *RTab, uint8_t nombreBits);
void xor(bool *resultatTab, bool *premierK, bool *deuxiemeK, uint8_t nombreBits);
uint8_t bitFaute(bool *correcteTab, bool *fauxTab);
void SboxFonction(bool *resultat, bool *entree, uint8_t numSbox);
void obtenirR16L16(uint64_t hex, Message *m);
void extraire6Bits(Message *m, uint8_t position);
uint64_t K16EnHex(int tabK16[8][64]);
uint64_t rechercheK16(uint64_t chiffreCorrect, uint64_t *chiffresFaux);
void initTab(bool *tab, uint8_t nombreBits);
void decalageGauche(bool *resultat, bool *tabAshifter, uint8_t nbrShift, uint8_t nombreBits);
void fusionTab(bool *resultat, bool *LTab, bool *RTab, uint8_t nombreBits);
void generationSousCles(bool sousCles[][48], bool *cle64Bits);
void F(bool *resultat, bool *Ri, bool *Ki);
void copieTab(bool *resultat, bool *aCopier, uint8_t nombreBits);
uint64_t fonctionDES(uint64_t clair, uint64_t k64);
uint64_t rechercheK56Bits(uint64_t clair, uint64_t chiffre, uint64_t k16);
uint64_t rechercheK(uint64_t clair, uint64_t chiffre, uint64_t k16);

#endif