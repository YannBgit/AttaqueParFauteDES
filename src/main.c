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
    FILE *fic = fopen("ressources/data.txt", "r");

    if(fic == NULL)
    {
        printf("Ce fichier n'existe pas\n");

        exit(0);
    }

    uint64_t clair = extractionDuClair(fic);                    // Message clair
    uint64_t chiffre = extractionDuChiffreBon(fic);             // Chiffré correct
	uint64_t *chiffresFaux = extractionDesChiffresFaux(fic);    // Tableau des chiffrés faux
    uint64_t K16;                                               // Sous-clé K16
    uint64_t K56;                                               // Sous-clé K56
    uint64_t K64;                                               // Clé maître K64

    fclose(fic);

    // Affichage initial
    printf("Clair :\t %lx\nChiffré bon :\t %lx\n\n", clair, chiffre);

    for(int i = 0; i < NOMBRE_CHIFFRES_FAUX; i++)
    {
        printf("Chiffré faux n°%d :\t %lx\n", i, chiffresFaux[i]);
    }

    // Calculs
    K16 = rechercheK16(chiffre, chiffresFaux);
    K56 = rechercheK56(clair, chiffre, K16);
    K64 = rechercheK64(clair, chiffre, K56);

    // Affichage final
    printf("\nValeur de K16 : %lx\n", K16);
    printf("Valeur de K56 : %lx\n", K56);
    printf("Valeur de K64 : %lx\n", K64);

    // Fin
    free(chiffresFaux);

    exit(0);
}