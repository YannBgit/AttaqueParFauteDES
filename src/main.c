// LIBRAIRIES
#include "constantes.h"
#include "gestionnaireDeFichiers.h"
#include "rechercheDES.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// FONCTIONS

// MAIN
int main()
{
    // Initialisation des variables
    FILE *fic = fopen("resources/data.txt", "r");

    if(fic == NULL)
    {
        printf("Ce fichier n'existe pas\n");

        exit(0);
    }

    uint64_t clair = extractionDuClair(fic);       // Message clair
    uint64_t chiffre = extractionDuchiffre(fic);   // Chiffré correct
	uint64_t *chiffresFaux = extractionDesCF(fic); // Tableau des chiffrés faux
    uint64_t K16;                                  // Sous-clé K16
    uint64_t K;                                    // Clé maître K

    fclose(fic);

    // Affichage initial
    printf("Clair :\t %lx\nChiffré bon :\t %lx\n\n", clair, chiffre);

    for(int i = 0; i < NOMBRE_CHIFFRES_FAUX; i++)
    {
        printf("Chiffré faux n°%d :\t %lx\n", i, chiffresFaux[i]);
    }

    // Calculs
    K16 = rechercheK16(clair, chiffre, chiffresFaux);
    K = rechercheK(clair, chiffre, K16);

    // Affichage final
    printf("\nValeur de K16 : %lx\n", K16);
    printf("\nValeur de K : %lx\n", K);

    // Fin
    free(chiffresFaux);

    exit(0);
}