// LIBRAIRIES
#include "manipulationBin.h"

// FONCTIONS
void uint64EnBin(bool *resultatT, uint64_t uint64, uint8_t nombreBits)
{
	uint64_t tmp = uint64;
	int entier;
	int compteur = nombreBits * 4 - 1;

	for(int i = 0; i < nombreBits; i++)
    {
		entier = tmp & 0xF;

		for(int j = 0; j < TAILLE_SORTIE_SBOX; j++)
        {
			resultatT[compteur] = entier % 2;
			entier = entier / 2;
			compteur--;
		}

		tmp = tmp >> TAILLE_SORTIE_SBOX;
	}
}

void decimalEnBin(bool *resultatT, int decimal, uint8_t nombreBits)
{
	uint64_t entier = decimal;

	for(int i = nombreBits - 1; i >= 0; i--)
    {
		resultatT[i] = entier % 2;
		entier = entier / 2;
	}
}

uint64_t TEnUint64(bool *T, uint8_t nombreBits)
{	
	uint64_t nombre = 0;

	for (int i = 0; i < nombreBits; i++)
    {
        if(T[nombreBits - i - 1] != 0)
		{
			nombre += (uint64_t)pow((double)2, (double)i);
		}
    }

	return nombre;
}

void permutation(bool *resultat, bool *aPermute, const uint8_t *matriceP, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		if(matriceP[i] != 0)
        {
			resultat[i] = aPermute[matriceP[i] - 1];
		}
	}
}

void separationT(bool *completT, bool *LT, bool *RT, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		LT[i] = completT[i];
		RT[i] = completT[i + nombreBits];
	}
}

void xor(bool *resultatT, bool *T1, bool *T2, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		resultatT[i] = T1[i] ^ T2[i];
	}
}

void extraire6Bits(Message *m, uint8_t position)
{
	for(int i = 0; i < TAILLE_ENTREE_SBOX; i++)
    {
		m->Sbox6BitsBin[i] = m->RChiffreBinE[TAILLE_ENTREE_SBOX * position + i];
	}
}

bool TEgaux(bool *T1, bool *T2, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		if(T1[i] != T2[i])
        {
			return 0;
        }
	}

	return 1;
}

void initT(bool *T, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		T[i] = 0;
	}
}

void decalageGauche(bool *resultat, bool *TaShift, uint8_t nombreShifts, uint8_t nombreBits)
{
	for(int i = -nombreShifts; i < (nombreBits - nombreShifts); i++)
    {
		if(i < 0)
        {
			resultat[i + nombreBits] = TaShift[i + nombreShifts];
		}
        
        else
        {
			resultat[i] = TaShift[i + nombreShifts];	
		}
	}
}

uint64_t bitFaute(bool *correcteT, bool *fauxT)
{
	bool xorT[TAILLE_DEMI_BLOC + 1] = {0};

	xor(xorT, correcteT, fauxT, TAILLE_DEMI_BLOC);

	for(int j = 0; j < TAILLE_DEMI_BLOC; j++)
    {
		if(xorT[j] == 1)
        {
			return j;
		}
	}

	return -1;
}

void fusionT(bool *resultat, bool *LT, bool *RT, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		resultat[i] = LT[i];
		resultat[i + nombreBits] = RT[i];
	}
}