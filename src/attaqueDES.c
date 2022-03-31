// LIBRAIRIES
#include "attaqueDES.h"

// FONCTIONS
void SboxFonction(bool *resultat, bool *entree, uint8_t numSbox)
{
	int row = entree[0] * 2 + entree[TAILLE_ENTREE_SBOX - 1];
	int column = 0;
	int i = 0;

	for(int j = TAILLE_SORTIE_SBOX; j > 0; j--)
    {
		if(entree[j] != 0)
        {
			column += (int)pow((double)2, (double)i);
		}

		i++;
	}

	uint8_t resultat4bit = Sbox[numSbox][row][column];

	uint64EnBin(resultat, resultat4bit, 1);
}

void obtenirR15L15(uint64_t uint64, Message *m)
{
	m->chiffreUint64 = uint64;
	uint64EnBin(m->chiffreBin, uint64, BASE);
	permutation(m->chiffreBinPermute, m->chiffreBin, IP, TAILLE_BLOC);
	separationT(m->chiffreBinPermute, m->LChiffreBin, m->RChiffreBin, TAILLE_DEMI_BLOC);
	
}

uint64_t K15EnUint64(int TK15[][TAILLE_BLOC])
{
	int T[NOMBRE_SBOXES] = {0};
	bool cles[TAILLE_ENTREE_SBOX] = {0};
	bool resultatT[TAILLE_BLOC] = {0};

	for(int i = 0; i < NOMBRE_SBOXES; i++)
    {
		for(int j = 0; j < TAILLE_BLOC; j++)
        {
			if(TK15[i][j] == TAILLE_ENTREE_SBOX)
            {
                T[i] = j;
            }
		}

		decimalEnBin(cles, T[i], TAILLE_ENTREE_SBOX);

		for(int j = 0; j < TAILLE_ENTREE_SBOX; j++)
        {
			resultatT[i * TAILLE_ENTREE_SBOX + j] = cles[j];
		}
	}

	return TEnUint64(resultatT, TAILLE_SOUS_CLE);
}

void generationSousCles(bool sousCles[][TAILLE_SOUS_CLE], bool *cle64Bits)
{
	bool cle56Bin[TAILLE_CLE_MAITRE_SANS_PARITE] = {0};
	bool TSepare[2][TAILLE_DEMIE_CLE_MAITRE_SANS_PARITE] = {0};
	bool TSepareRes[2][TAILLE_DEMIE_CLE_MAITRE_SANS_PARITE] = {0};

	permutation(cle56Bin, cle64Bits, PC1, TAILLE_CLE_MAITRE_SANS_PARITE);

	for(int i = 0; i < NOMBRE_SOUS_CLES; i++)
    {
		separationT(cle56Bin,TSepare[0],TSepare[1], TAILLE_DEMIE_CLE_MAITRE_SANS_PARITE);
		decalageGauche(TSepareRes[0], TSepare[0], V[i], TAILLE_DEMIE_CLE_MAITRE_SANS_PARITE);
		decalageGauche(TSepareRes[1], TSepare[1], V[i], TAILLE_DEMIE_CLE_MAITRE_SANS_PARITE);
		fusionT(cle56Bin, TSepareRes[0], TSepareRes[1], TAILLE_DEMIE_CLE_MAITRE_SANS_PARITE);
		permutation(sousCles[i], cle56Bin, PC2, TAILLE_CLE_MAITRE_SANS_PARITE);
	}
}

void F(bool *resultat, bool *Ri, bool *Ki)
{
	bool R48B[TAILLE_SOUS_CLE] = {0};
	bool resultatXor[TAILLE_SOUS_CLE] = {0};
	bool entrerSbox[TAILLE_ENTREE_SBOX] = {0};
	bool sortieSbox[TAILLE_SORTIE_SBOX] = {0};
	bool sortie32Bin[TAILLE_DEMI_BLOC] = {0};

	permutation(R48B, Ri, E, TAILLE_SOUS_CLE);
	xor(resultatXor, R48B, Ki, TAILLE_SOUS_CLE);

	for(int i = 0; i < NOMBRE_SBOXES; i++)
    {
		for(int j = 0; j < TAILLE_ENTREE_SBOX; j++)
        {
			entrerSbox[j] = resultatXor[TAILLE_ENTREE_SBOX * i + j];
		}

		SboxFonction(sortieSbox, entrerSbox, i);

		for(int j = 0; j < TAILLE_SORTIE_SBOX; j++)
        {
			sortie32Bin[i * TAILLE_SORTIE_SBOX + j] = sortieSbox[j];
		}
	}

	permutation(resultat, sortie32Bin, P, TAILLE_DEMI_BLOC);
}

uint64_t fonctionDES(uint64_t clair, uint64_t k64)
{
	DES d;
	bool resultatF[TAILLE_DEMI_BLOC] = {0};
	bool resultatConcat[TAILLE_BLOC] = {0};

	uint64EnBin(d.clairBin, clair, BASE);
	uint64EnBin(d.cle64Bin, k64, BASE);
	permutation(d.clairBinIP, d.clairBin, IP, TAILLE_BLOC);
	separationT(d.clairBinIP, d.L32Bin, d.R32Bin, TAILLE_DEMI_BLOC);
	generationSousCles(d.sousCles, d.cle64Bin);

	for(int i = 0; i < NOMBRE_SOUS_CLES; i++)
    {
		memcpy(d.L32BinPlus1, d.R32Bin, TAILLE_DEMI_BLOC);
		F(resultatF, d.R32Bin, d.sousCles[i]);
		xor(d.R32BinPlus1, d.L32Bin, resultatF, TAILLE_DEMI_BLOC);
		memcpy(d.L32Bin, d.L32BinPlus1, TAILLE_DEMI_BLOC);
		memcpy(d.R32Bin, d.R32BinPlus1, TAILLE_DEMI_BLOC);
	}

	fusionT(resultatConcat, d.R32Bin, d.L32Bin, TAILLE_DEMI_BLOC);
	permutation(d.chiffreBin, resultatConcat, IPinverse, TAILLE_BLOC);

	return TEnUint64(d.chiffreBin, TAILLE_BLOC);
}

uint64_t rechercheK15(uint64_t chiffreCorrect, uint64_t *chiffresFaux)
{
	Message juste;
	Message faux;
	int resultat[NOMBRE_SBOXES][TAILLE_BLOC] = {0};
	bool LPinverse[TAILLE_DEMI_BLOC] = {0};
	bool resultatXorL[TAILLE_DEMI_BLOC] = {0};

	obtenirR15L15(chiffreCorrect, &juste);

	for(int w = 0; w < TAILLE_DEMI_BLOC; w++)
    {
		obtenirR15L15(chiffresFaux[w], &faux);
		xor(resultatXorL, juste.LChiffreBin, faux.LChiffreBin, TAILLE_DEMI_BLOC);
		permutation(LPinverse, resultatXorL, Pinverse, TAILLE_DEMI_BLOC);

		uint64_t bitFaux = bitFaute(juste.RChiffreBin, faux.RChiffreBin);
		permutation(juste.RChiffreBinE, juste.RChiffreBin, E, TAILLE_SOUS_CLE);
		permutation(faux.RChiffreBinE, faux.RChiffreBin, E, TAILLE_SOUS_CLE);

		bool resSbox[TAILLE_SORTIE_SBOX] = {0};
		bool resLeftJuste[TAILLE_SORTIE_SBOX] = {0};
		bool cle[TAILLE_ENTREE_SBOX] = {0};

		for(int i = 0;  i < TAILLE_SOUS_CLE; i++)
        {
			if(E[i] == (bitFaux + 1))
            {
				extraire6Bits(&juste, i / TAILLE_ENTREE_SBOX);
				extraire6Bits(&faux, i / TAILLE_ENTREE_SBOX);

				for(int y = 0; y < TAILLE_SORTIE_SBOX; y++)
                {
					resLeftJuste[y] = LPinverse[TAILLE_SORTIE_SBOX * (i / TAILLE_ENTREE_SBOX) + y];
				}

				for(int j = 0;  j < TAILLE_BLOC; j++)
                {
					decimalEnBin(cle, j, TAILLE_ENTREE_SBOX);
					xor(juste.Sbox6BitsXoreBin, juste.Sbox6BitsBin, cle, TAILLE_ENTREE_SBOX);
					decimalEnBin(cle, j, TAILLE_ENTREE_SBOX);
					xor(faux.Sbox6BitsXoreBin, faux.Sbox6BitsBin, cle, TAILLE_ENTREE_SBOX);
					SboxFonction(juste.Sbox4BitsBin, juste.Sbox6BitsXoreBin, i / TAILLE_ENTREE_SBOX);
					SboxFonction(faux.Sbox4BitsBin, faux.Sbox6BitsXoreBin, i / TAILLE_ENTREE_SBOX);
					xor(resSbox,juste.Sbox4BitsBin, faux.Sbox4BitsBin, TAILLE_SORTIE_SBOX);

					if(TEgaux(resLeftJuste, resSbox, TAILLE_SORTIE_SBOX))
                    {
						resultat[i / TAILLE_ENTREE_SBOX][TEnUint64(cle, TAILLE_ENTREE_SBOX)]++;
					}
				}
			}
		}
	}

	return K15EnUint64(resultat);
}

uint64_t rechercheK56Bits(uint64_t clair, uint64_t chiffre, uint64_t K15)
{
	Cle k;

	initT(k.cle48Bin, TAILLE_SOUS_CLE);
	initT(k.cle56Bin, TAILLE_CLE_MAITRE_SANS_PARITE);
	initT(k.cle64Bin, TAILLE_BLOC);
	uint64EnBin(k.cle48Bin, K15,12);
	permutation(k.cle56Bin, k.cle48Bin, PC2Inverse, 56);
	permutation(k.cle64Bin, k.cle56Bin, PC1Inverse, 64);

	uint8_t position8bit[NOMBRE_SBOXES] = {14, 15, 19, 20, 51, 54, 58, 60};

	for(int i = 0; i < (int)pow((double)2, (double)NOMBRE_SBOXES); i++)
    {
		decimalEnBin(k.cle8Bin, i, NOMBRE_SBOXES);

		for(int j = 0; j < NOMBRE_SBOXES; j++)
        {
			k.cle64Bin[position8bit[j] - 1] = k.cle8Bin[j];
		}

		uint64_t cle = TEnUint64(k.cle64Bin, TAILLE_BLOC);

		if(chiffre == fonctionDES(clair, cle))
        {
			return cle;
		}
	}

	return 0;
}

uint64_t rechercheK(uint64_t clair, uint64_t chiffre, uint64_t K15)
{
	int compteur = 0;
	bool clesB[TAILLE_BLOC] = {0};

	uint64EnBin(clesB, rechercheK56Bits(clair, chiffre, K15), BASE);

	for(int i = 1; i < (TAILLE_BLOC + 1); i++)
    {
		if((i % NOMBRE_SBOXES) == 0)
        {
			if(compteur % 2 == 1)
            {
				clesB[i - 1] = 0;
			}
            
            else
            {
				clesB[i - 1] = 1;
			}

			compteur = 0;
		}
        
        else
        {
			compteur += clesB[i - 1];
		}
	}

	return TEnUint64(clesB, TAILLE_BLOC);
}