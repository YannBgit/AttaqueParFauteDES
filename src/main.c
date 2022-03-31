// LIBRAIRIES
#include "constantes.h"
#include "gestionnaireDeFichiers.h"
#include "attaqueDES.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

    uint64_t clair = extractionDuClair(fic);                    // Message clair
    uint64_t chiffre = extractionDuChiffreBon(fic);             // Chiffré correct
	uint64_t *chiffresFaux = extractionDesChiffresFaux(fic);    // Tableau des chiffrés faux
    uint64_t K15;                                               // Sous-clé K15
    uint64_t K;                                                 // Clé maître K

    fclose(fic);

    // Affichage initial
    printf("Clair :\t %lx\nChiffré bon :\t %lx\n\n", clair, chiffre);

    for(int i = 0; i < NOMBRE_CHIFFRES_FAUX; i++)
    {
        printf("Chiffré faux n°%d :\t %lx\n", i, chiffresFaux[i]);
    }

    // Calculs
    K15 = rechercheK15(chiffre, chiffresFaux);
    K = rechercheK(clair, chiffre, K15);

    // Affichage final
    printf("\nValeur de K15 : %lx\n", K15);
    printf("Valeur de K : %lx\n", K);

    // Fin
    free(chiffresFaux);

    exit(0);
}