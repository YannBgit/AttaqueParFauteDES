// DIRECTIVES
#ifndef STRUCTURES_H
#define STRUCTURES_H

// LIBRAIRIES
#include "constantes.h"
#include <stdbool.h>

// STRUCTURES
// Structure représentant les étapes de chiffrement d'un message
typedef struct message
{
	uint64_t chiffreUint64;
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
	bool sousCles[NOMBRE_SOUS_CLES][TAILLE_SOUS_CLE];
	bool chiffreBin[TAILLE_BLOC];
} DES;

#endif