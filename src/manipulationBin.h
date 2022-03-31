// DIRECTIVES
#ifndef MANIPULATIONBIN_H
#define MANIPULATIONBIN_H

// LIBRAIRIES
#include "constantes.h"
#include "structures.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// FONCTIONS
void uint64EnBin(bool *resultatT, uint64_t uint64, uint8_t nombreBits);
void decimalEnBin(bool *resultatT, int decimal, uint8_t nombreBits);
uint64_t TEnUint64(bool *T, uint8_t nombreBits);
void permutation(bool *resultat, bool *aPermute, const uint8_t *matriceP, uint8_t nombreBits);
void separationT(bool *completT, bool *LT, bool *RT, uint8_t nombreBits);
void xor(bool *resultatT, bool *T1, bool *T2, uint8_t nombreBits);
void extraire6Bits(Message *m, uint8_t position);
bool TEgaux(bool *T1, bool *T2, uint8_t nombreBits);
void initT(bool *T, uint8_t nombreBits);
void decalageGauche(bool *resultat, bool *TaShift, uint8_t nombreShifts, uint8_t nombreBits);
uint64_t bitFaute(bool *correcteT, bool *fauxT);
void fusionT(bool *resultat, bool *LT, bool *RT, uint8_t nombreBits);
void copieT(bool *resultat, bool *aCopier, uint8_t nombreBits);

#endif