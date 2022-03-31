// LIBRAIRIES
#include "attaqueDES.h"

// FONCTIONS
void hexEnBin(bool *resultatTab, uint64_t hex, uint8_t nombreBits)
{
	uint64_t tmp = hex;
	int entier;
	int compteur = nombreBits * 4 - 1;

	for(int i = 0; i < nombreBits; i++)
    {
		entier = tmp & 0xF;

		for(int j = 0; j < 4; j++)
        {
			resultatTab[compteur] = entier % 2;
			entier = entier / 2;
			compteur--;
		}

		tmp = tmp >> 4;
	}
}

void decimalEnBin(bool *resultatTab, uint64_t decimal, uint8_t nombreBits)
{
	uint64_t entier = decimal;

	for(int i = nombreBits - 1; i >= 0; i--)
    {
		resultatTab[i] = entier % 2;
		entier = entier / 2;
	}
}

uint64_t tabEnHex(bool *tab, uint8_t nombreBits)
{	
	uint64_t nombre = 0;

	for (int i = 0; i < nombreBits; i++)
    {
        if (tab[i])
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

void separationTab(bool *completTab, bool *LTab, bool *RTab, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		LTab[i] = completTab[i];
		RTab[i] = completTab[i + nombreBits];
	}
}

void xor(bool *resultatTab, bool *tab1, bool *tab2, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		resultatTab[i] = tab1[i] ^ tab2[i];
	}
}

uint8_t bitFaute(bool *correcteTab, bool *fauxTab)
{
	bool xorTab[33] = {0};

	xor(xorTab, correcteTab, fauxTab, TAILLE_DEMI_BLOC);

	for(int j = 0; j < TAILLE_DEMI_BLOC; j++)
    {
		if(xorTab[j] == 1)
        {
			return j;
		}
	}

	return -1;
}

void SboxFonction(bool *resultat, bool *entree, uint8_t numSbox)
{
	int row = entree[0] * 2 + entree[5];
	int column = 0;

	int i = 0, j = 4;

	while(j > 0)
    {
		if(entree[j] != 0)
        {
			column += (int)pow((double)2, (double)i);
		}

		i++;
		j--;
	}

	uint8_t resultat4bit = Sbox[numSbox][row][column];

	hexEnBin(resultat, resultat4bit, 1);
}

void obtenirR16L16(uint64_t hex, Message *m)
{
	m->chiffreHex = hex;
	hexEnBin(m->chiffreBin, hex, BASE);
	permutation(m->chiffreBinPermute, m->chiffreBin, IP, TAILLE_BLOC);
	separationTab(m->chiffreBinPermute, m->LChiffreBin, m->RChiffreBin, TAILLE_DEMI_BLOC);
	
}

void extraire6Bits(Message *m, uint8_t position)
{
	for(int i = 0; i < 6; i++)
    {
		m->Sbox6BitsBin[i] = m->RChiffreBinE[6 * position + i];
	}
}

bool tabEgaux(bool *tab1, bool *tab2, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		if(tab1[i] != tab2[i])
        {
			return 0;
        }
	}

	return 1;
}

uint64_t K16EnHex(int tabK16[8][64])
{
	int tab[8] = {0};
	bool cles[6] = {0};
	bool resultatTab[TAILLE_BLOC] = {0};

	for(int i = 0; i < 8; i++)
    {
		for(int j = 0; j < 64; j++)
        {
			if(tabK16[i][j] == 6)
            {
                tab[i] = j;
            }
		}

		printf("%d\n", tab[i]);
		decimalEnBin(cles, tab[i], 6);

		for(int j = 0; j < 6; j++)
        {
			resultatTab[i * 6 + j] = cles[j];
		}
	}

	return tabEnHex(resultatTab, TAILLE_SOUS_CLE);
}

uint64_t rechercheK16(uint64_t chiffreCorrect, uint64_t *chiffresFaux)
{
	Message juste;
	Message faux;
	uint64_t aRetourne = 0;
	int resultat[8][TAILLE_BLOC] = {0};

	obtenirR16L16(chiffreCorrect, &juste);

	bool LPinverse[TAILLE_DEMI_BLOC] = {0};
	bool resultatXorL[TAILLE_DEMI_BLOC] = {0};

	for(int w = 0; w < TAILLE_DEMI_BLOC; w++)
    {
		obtenirR16L16(chiffresFaux[w], &faux);
		xor(resultatXorL, juste.LChiffreBin, faux.LChiffreBin, TAILLE_DEMI_BLOC);
		permutation(LPinverse, resultatXorL, Pinverse, TAILLE_DEMI_BLOC);

		uint8_t bitFaux = bitFaute(juste.RChiffreBin, faux.RChiffreBin);

		permutation(juste.RChiffreBinE, juste.RChiffreBin, E, TAILLE_SOUS_CLE);
		permutation(faux.RChiffreBinE, faux.RChiffreBin, E, TAILLE_SOUS_CLE);

		bool resSbox[4] = {0};
		bool resLeftJuste[4] = {0};
		bool cle[6] = {0};

		for(int i = 0;  i < TAILLE_SOUS_CLE; i++)
        {
			if(E[i] == (bitFaux + 1))
            {
				extraire6Bits(&juste, i / 6);
				extraire6Bits(&faux, i / 6);

				for(int y = 0; y < 4; y++)
                {
					resLeftJuste[y] = LPinverse[4 * (i / 6) + y];
				}

				for(int j = 0;  j < TAILLE_BLOC; j++)
                {
					decimalEnBin(cle, j, 6);
					xor(juste.Sbox6BitsXoreBin, juste.Sbox6BitsBin, cle, 6);
					decimalEnBin(cle, j, 6);
					xor(faux.Sbox6BitsXoreBin, faux.Sbox6BitsBin, cle, 6);
					SboxFonction(juste.Sbox4BitsBin, juste.Sbox6BitsXoreBin, i/6);
					SboxFonction(faux.Sbox4BitsBin, faux.Sbox6BitsXoreBin, i/6);
					xor(resSbox,juste.Sbox4BitsBin, faux.Sbox4BitsBin, 4);

					if(tabEgaux(resLeftJuste, resSbox, 4))
                    {
						resultat[i / 6][tabEnHex(cle, 6)]++;
					}
				}
			}
		}
	}

	aRetourne = K16EnHex(resultat);

	return aRetourne;
}

void initTab(bool *tab, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		tab[i] = 0;
	}
}

void decalageGauche(bool *resultat, bool *tabAshifter, uint8_t nombreShifts, uint8_t nombreBits)
{
	for(int i = -nombreShifts; i < (nombreBits - nombreShifts); i++)
    {
		if(i < 0)
        {
			resultat[i+nombreBits] = tabAshifter[i + nombreShifts];
		}
        
        else
        {
			resultat[i] = tabAshifter[i + nombreShifts];	
		}
	}
}

void fusionTab(bool *resultat, bool *LTab, bool *RTab, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		resultat[i] = LTab[i];
		resultat[i + nombreBits] = RTab[i];
	}
}

void generationSousCles(bool sousCles[][TAILLE_SOUS_CLE], bool *cle64Bits)
{
	bool cle56Bin[TAILLE_CLE_MAITRE_SANS_PARITE] = {0};
	bool tabSepare[2][28] = {0};
	bool tabSepareRes[2][28] = {0};

	permutation(cle56Bin, cle64Bits, PC1, TAILLE_CLE_MAITRE_SANS_PARITE);

	for(int i = 0; i < 16; i++)
    {
		separationTab(cle56Bin,tabSepare[0],tabSepare[1], 28);
		decalageGauche(tabSepareRes[0], tabSepare[0], V[i], 28);
		decalageGauche(tabSepareRes[1], tabSepare[1], V[i], 28);
		fusionTab(cle56Bin, tabSepareRes[0], tabSepareRes[1], 28);
		permutation(sousCles[i], cle56Bin, PC2, TAILLE_CLE_MAITRE_SANS_PARITE);
	}
}

void F(bool *resultat, bool *Ri, bool *Ki)
{
	bool R48B[48] = {0};
	bool resultatXor[TAILLE_SOUS_CLE] = {0};
	bool entrerSbox[6] = {0};
	bool sortieSbox[4] = {0};
	bool sortie32Bin[TAILLE_DEMI_BLOC] = {0};

	permutation(R48B, Ri, E, TAILLE_SOUS_CLE);
	xor(resultatXor, R48B, Ki, TAILLE_SOUS_CLE);

	for(int i = 0; i < 8; i++)
    {
		for(int j = 0; j < 6; j++)
        {
			entrerSbox[j] = resultatXor[6 * i + j];
		}

		SboxFonction(sortieSbox, entrerSbox, i);

		for(int j = 0; j < 4; j++)
        {
			sortie32Bin[i * 4 + j] = sortieSbox[j];
		}
	}

	permutation(resultat, sortie32Bin, P, TAILLE_DEMI_BLOC);
}

void copieTab(bool *resultat, bool *aCopier, uint8_t nombreBits)
{
	for(int i = 0; i < nombreBits; i++)
    {
		resultat[i] = aCopier[i];
	}
}

uint64_t fonctionDES(uint64_t clair, uint64_t k64)
{
	DES d;
	bool resultatF[TAILLE_DEMI_BLOC] = {0};
	bool resultatConcat[TAILLE_BLOC] = {0};

	hexEnBin(d.clairBin, clair, BASE);
	hexEnBin(d.cle64Bin, k64, BASE);
	permutation(d.clairBinIP, d.clairBin, IP, TAILLE_BLOC);
	separationTab(d.clairBinIP, d.L32Bin, d.R32Bin, TAILLE_DEMI_BLOC);
	generationSousCles(d.sousCles, d.cle64Bin);

	for(int i = 0; i < 16; i++)
    {
		copieTab(d.L32BinPlus1, d.R32Bin, TAILLE_DEMI_BLOC);
		F(resultatF, d.R32Bin, d.sousCles[i]);
		xor(d.R32BinPlus1, d.L32Bin, resultatF, TAILLE_DEMI_BLOC);
		copieTab(d.L32Bin, d.L32BinPlus1, TAILLE_DEMI_BLOC);
		copieTab(d.R32Bin, d.R32BinPlus1, TAILLE_DEMI_BLOC);
	}

	fusionTab(resultatConcat, d.R32Bin, d.L32Bin, TAILLE_DEMI_BLOC);
	permutation(d.chiffreBin, resultatConcat, IPinverse, TAILLE_BLOC);

	return tabEnHex(d.chiffreBin, TAILLE_BLOC);
}

uint64_t rechercheK56Bits(uint64_t clair, uint64_t chiffre, uint64_t k16)
{
	Cle k;

	initTab(k.cle48Bin,48);
	initTab(k.cle56Bin,56);
	initTab(k.cle64Bin,64);
	hexEnBin(k.cle48Bin,k16,12);
	permutation(k.cle56Bin, k.cle48Bin, PC2Inverse, 56);
	permutation(k.cle64Bin, k.cle56Bin, PC1Inverse, 64);

	uint8_t position8bit[8] = {14, 15, 19, 20, 51, 54, 58, 60};

	for(int i = 0; i < 256; i++)
    {
		decimalEnBin(k.cle8Bin, i, 8);

		for(int j = 0; j < 8; j++)
        {
			k.cle64Bin[position8bit[j] - 1] = k.cle8Bin[j];
		}

		uint64_t cle = tabEnHex(k.cle64Bin,64);

		if(chiffre == fonctionDES(clair, cle))
        {
			return cle;
		}
	}

	return 0;
}

uint64_t rechercheK(uint64_t clair, uint64_t chiffre, uint64_t k16)
{
	int compteur = 0;
	bool clesB[64] = {0};

	hexEnBin(clesB, rechercheK56Bits(clair, chiffre, k16), BASE);

	for(int i = 0; i < TAILLE_BLOC; i++)
    {
		printf("%d", clesB[i]);

		if((i + 1) % 8 == 0)
        {
            printf(" ");
        }
	}

	printf("\n");

	for(int i = 1; i < 65; i++)
    {
		if((i % 8) == 0)
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

	for(int i = 0; i < TAILLE_BLOC; i++)
    {
		printf("%d", clesB[i]);

		if((i + 1) % 8 == 0)
        {
            printf(" ");
        }
	}

	printf("\n");

	return tabEnHex(clesB, TAILLE_BLOC);
}