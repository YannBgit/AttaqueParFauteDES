// LIBRAIRIES
#include "attaqueDES.h"

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
void uint64ToBin(bool *tabResult, uint64_t hexa, uint8_t nbrHexa)
{
	uint64_t tmp = hexa;
	uint64_t entier;
	uint64_t compteur = nbrHexa * 4 - 1;

	for(int i = 0; i < nbrHexa; i++)
    {
		entier = tmp & 0xF;

		for(int j = 0; j < 4; j++)
        {
			tabResult[compteur] = entier % 2;
			entier = entier / 2;
			compteur--;
		}

		tmp = tmp >> 4;
	}
}

void decimalTobBin(bool *tabResult, uint64_t decimal, uint8_t nbrBit)
{
	uint64_t entier = decimal;

	for(uint64_t i = nbrBit - 1; i >= 0; i--)
    {
		tabResult[i] = entier % 2;
		entier = entier / 2;
	}
}

uint64_t puissance(uint64_t entier, uint64_t pow)
{
	if(pow == 0)
    {
		return 1;
	}
    
    else
    {
		return entier * puissance(entier, pow - 1);
	}
}

uint64_t binToUint64_t(bool *tab, uint8_t nbrBit)
{	
	uint64_t nombre = 0;
	uint64_t i=0,j=nbrBit-1;

	while(j >= 0)
    {
		if(tab[j] != 0)
        {
			nombre += puissance(2, i);
		}

		i++;
		j--;
	}

	return nombre;
}

void permutation(bool *resultat, bool *aPermuter, bool *tablePermutation, uint8_t nbrBit)
{
	for(int i = 0; i < nbrBit; i++)
    {
		if(tablePermutation[i] != 0)
        {
			resultat[i] = aPermuter[tablePermutation[i] - 1];
		}
	}
}

void separationTab(bool *completTab, bool *leftTab, bool *rightTab, uint8_t nbrBit)
{
	for(int i = 0; i < nbrBit; i++)
    {
		leftTab[i] = completTab[i];
		rightTab[i] = completTab[i + nbrBit];
	}
}

void xor(bool *tabResult, bool *premierK, bool *deuxiemeK, uint8_t nbrBit)
{
	for(int i = 0; i < nbrBit; i++)
    {
		tabResult[i] = premierK[i] ^ deuxiemeK[i];
	}
}

uint8_t bitFaute(bool *tabJuste, bool *tabFaux)
{
	bool tabXor[33] = {0};

	xor(tabXor, tabJuste, tabFaux, TAILLE_DEMI_BLOC);

	for(int j = 0; j < 32; j++)
    {
		if(tabXor[j] == 1)
        {
			return j;
		}
	}

	return -1;
}

void Sboxfonc(bool *resultat, bool *entree, uint8_t numSbox)
{
	uint64_t row = 0;
	uint64_t column = 0;
	row = entree[0] * 2 + entree[5];
	uint8_t i = 0, j = 4;

	while(j > 0)
    {
		if(entree[j] != 0)
        {
			column += puissance(2, i);
		}

		i++;
		j--;
	}

	uint64_t resultat4bit = Sbox[numSbox][row][column];

	hexatobinary(resultat, resultat4bit, 1);
}

void obtenirR16L16(uint64_t hexa, Message *m)
{
	m->chiffreHex = hexa;
	uint64ToBinary(m->chiffreBin, hexa, 16);
	Permutation(m->chiffreBinPermute, m->chiffreBin, IP, 64);
	separationTab(m->chiffreBinPermute, m->LChiffreBin, m->RChiffreBin, 32);
	
}

void extraire6Bits(Message *m, uint64_t position)
{
	for(uint64_t i = 0; i < 6; i++)
    {
		m->Sbox6BitsBin[i] = m->RChiffreBinE[6 * position + i];
	}
}

bool egal(bool *tab1, bool *tab2, uint8_t nbrBit)
{
	for(int i = 0; i < nbrBit; i++)
    {
		if(tab1[i] != tab2[i])
        {
			return 0;
        }
	}

	return 1;
}

uint64_t k16enHexa(uint64_t tabK16[8][64])
{
	uint64_t resultat = 0;
	uint64_t tab[8] = {0};
	uint64_t tabclef[6] = {0};
	uint64_t tabresult[64] = {0};

	for(uint64_t i=0; i<8; i++)
    {
		for(uint64_t j=0;j<64;j++)
        {
			if(tabK16[i][j] == 6)
            {
                tab[i] = j;
            }
		}

		pruint64_tf("%d\n", tab[i]);
		decimaltobinary(tabclef, tab[i], 6);

		for(uint64_t j = 0; j < 6; j++)
        {
			tabresult[i * 6 + j] = tabclef[j];
		}
	}

	resultat = Tabtouint64_t(tabresult, 48);

	return resultat;
}

uint64_t rechercheK16(uint64_t LechiffrerJuste, uint64_t *LeschiffrerFaux)
{
	Message juste;
	Message faux;
	uint64_t aRetourner = 0;
	uint64_t resultat[8][64] = {0};

	obtenirR16L16(LechiffrerJuste,&juste);

	uint64_t leftPmoin1[32] = {0};
	uint64_t resultatxorLeft[32] = {0};

	for(int w = 0; w < 32; w++)
    {
		obtenirR16L16(LeschiffrerFaux[w],&faux);
		xor(resultatxorLeft, juste.LChiffreBin, faux.LChiffreBin, 32);
		Permutation(leftPmoin1, resultatxorLeft, Pinverse, 32);

		uint64_t bitfaux = bitFauter(juste.RChiffreBin, faux.RChiffreBin);

		Permutation(juste.RChiffreBinE, juste.RChiffreBin, E, 48);
		Permutation(faux.RChiffreBinE, faux.RChiffreBin, E, 48);

		uint64_t resSbox[4] = {0};
		uint64_t resLeftJuste[4] = {0};
		uint64_t cle[6] = {0};

		for(int i = 0;  i < 48; i++)
        {
			if(E[i] == (bitfaux + 1))
            {
				extraire6Bits(&juste, i/6);
				extraire6Bits(&faux, i/6);

				for(int y = 0; y < 4; y++)
                {
					resLeftJuste[y] = leftPmoin1[4*(i/6)+y];
				}

				for(int j = 0;  j < 64; j++)
                {
					decimalToBin(cle, j, 6);
					xor(juste.Sbox6BitsXoreBin, juste.Sbox6BitsBin, cle, 6);
					decimalToBin(cle, j, 6);
					xor(faux.Sbox6BitsXoreBin, faux.Sbox6BitsBin, cle, 6);
					Sboxfonc(juste.Sbox4BitsBin, juste.Sbox6BitsXoreBin, i/6);
					Sboxfonc(faux.Sbox4BitsBin, faux.Sbox6BitsXoreBin, i/6);
					xor(resSbox,juste.Sbox4BitsBin, faux.Sbox4BitsBin, 4);

					if(egal(resLeftJuste, resSbox,4))
                    {
						resultat[i/6][Tabtouint64_t(cle,6)]++;
					}
				}
			}
		}
	}

	aRetourner = k16enHexa(resultat);

	return aRetourner;
}

void initTab(bool *tab, uint8_t nbrBit)
{
	for(uint64_t i = 0; i < nbrBit; i++)
    {
		tab[i] = 0;
	}
}

void shiftGauche(uint64_t *resultat, uint64_t *tabAshifter, uint64_t nbrShift, uint64_t nbrBit)
{
	for(int i = -nbrShift; i < (nbrBit - nbrShift); i++)
    {
		if(i < 0)
        {
			resultat[i+nbrBit] = tabAshifter[i + nbrShift];
		}
        
        else
        {
			resultat[i] = tabAshifter[i + nbrShift];	
		}
	}
}

void fusionTab(uint64_t *resultat, uint64_t *leftTab, uint64_t *rightTab, uint64_t nbrBit)
{
	for(int i = 0; i < nbrBit; i++)
    {
		resultat[i] = leftTab[i];
		resultat[i+nbrBit] = rightTab[i];
	}
}

void generationSubcle(uint64_t Les16Subcle[][48], uint64_t *cle64Bit)
{
	uint64_t cle56Bin[56] = {0};
	uint64_t cle48Bin[48] = {0};
	uint64_t tabSplit[2][28] = {0};
	uint64_t tabSplitRes[2][28] = {0};

	Permutation(cle56Bin, cle64Bit, PC1, 56);

	for(int i = 0; i < 16; i++)
    {
		splitTab(cle56Bin,tabSplit[0],tabSplit[1], 28);
		shiftGauche(tabSplitRes[0], tabSplit[0], v[i], 28);
		shiftGauche(tabSplitRes[1], tabSplit[1], v[i], 28);
		fusionTab(cle56Bin, tabSplitRes[0], tabSplitRes[1], 28);
		Permutation(Les16Subcle[i], cle56Bin, PC2, 48);
	}
}

void f(uint64_t *resultat, uint64_t *Ri, uint64_t *Ki)
{
	uint64_t right48b[48] = {0};
	uint64_t resultatXor[48] = {0};
	uint64_t entrerSbox[6] = {0};
	uint64_t sortiSbox[4] = {0};
	uint64_t sorti32bit[32] = {0};

	Permutation(right48b,Ri, E, 48);
	xor(resultatXor, right48b, Ki, 48);

	for(int j = 0; j < 8; j++)
    {
		for(int i = 0; i < 6; i++)
        {
			entrerSbox[i] = resultatXor[6 * j + i];
		}

		Sboxfonc(sortiSbox, entrerSbox, j);

		for(int i = 0; i < 4; i++)
        {
			sorti32bit[j * 4 + i] = sortiSbox[i];
		}
	}

	Permutation(resultat, sorti32bit, P, 32);
}

void copieTab(uint64_t *resultat, uint64_t * aCopier, uint64_t nbrBit)
{
	for(uint64_t i=0; i<nbrBit; i++)
    {
		resultat[i] = aCopier[i];
	}
}

uint64_t fonctionDES(uint64_t clair, uint64_t k64)
{
	DES d;
	uint64_t resultatF[32] = {0};
	uint64_t resultatConcat[64] = {0};

	hexatobinary(d.clairBin, clair, 16);
	hexatobinary(d.cle64Bin, k64, 16);
	Permutation(d.clairBinIP, d.clairBin, IP, 64);
	splitTab(d.clairBinIP, d.L32Bin, d.R32Bin, 32);
	generationSubcle(d.sousCles, d.cle64Bin);

	for(uint64_t i = 0; i < 16; i++)
    {
		copieTab(d.L32BinPlus1, d.R32Bin, 32);
		f(resultatF, d.R32Bin, d.sousCles[i]);
		xor(d.R32BinPlus1, d.L32Bin, resultatF, 32);
		copieTab(d.L32Bin, d.L32BinPlus1, 32);
		copieTab(d.R32Bin, d.R32BinPlus1, 32);
	}
	fusionTab(resultatConcat, d.R32Bin, d.L32Bin, 32);
	Permutation(d.chiffreBin, resultatConcat, IPinverse, 64);

	return TabToUint64_t(d.chiffreBin,64);
}

uint64_t K56bits(uint64_t clair, uint64_t chiffrer, uint64_t k16)
{
	Cle k;

	initTab(k.cle48Bin,48);
	initTab(k.cle56Bin,56);
	initTab(k.cle64Bin,64);
	hexatobinary(k.cle48Bin,k16,12);
	Permutation(k.cle56Bin, k.cle48Bin, PC2Inverse, 56);
	Permutation(k.cle64Bin, k.cle56Bin, PC1Inverse, 64);

	uint64_t position8bit[8] = {14,15,19,20,51,54,58,60};
	uint64_t i=0;

	while(i < 256)
    {
		decimaltobinary(k.cle8Bin, i, 8);

		for(int j = 0; j < 8; j++)
        {
			k.cle64Bin[position8bit[j] - 1] = k.cle8Bin[j];
		}

		uint64_t cle = TabToUint64_t(k.cle64Bin,64);

		if(chiffrer == fonctionDES(clair, cle))
        {
			return cle;
		}

		i++;
	}

	return 0;
}

uint64_t rechercheK(uint64_t clair, uint64_t chiffrer, uint64_t k16)
{
	uint64_t compteur = 0;
	uint64_t tabClefB[64] = {0};

	hexatobinary(tabClefB, getK56bit(clair, chiffrer, k16), 16);

	for(int i = 0; i < 64; i++)
    {
		printf("%d", tabClefB[i]);

		if((i+1)%8 == 0)
        {
            printf(" ");
        }
	}

	printf("\n");

	for(int i = 1; i < 65; i++)
    {
		if((i%8) == 0)
        {
			if(compteur%2 == 1)
            {
				tabClefB[i-1] = 0;
			}
            
            else
            {
				tabClefB[i-1] = 1;
			}

			compteur = 0;
		}
        
        else
        {
			compteur += tabClefB[i - 1];
		}
	}

	for(int i = 0; i < 64; i++)
    {
		printf("%d", tabClefB[i]);

		if((i+1)%8 == 0)
        {
            printf(" ");
        }
	}

	printf("\n");

	return Tabtouint64_t(tabClefB, 64);
}