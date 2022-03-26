// LIBRAIRIES
#include "constantes.h"
#include "gestionnaireDeFichiers.h"
#include <stdio.h>
#include <stdlib.h>

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

    unsigned long int clair = extractionDuClair(fic);
    unsigned long int CB = extractionDuCB(fic);
	unsigned long int *TabCF = extractionDesCF(fic);

    fclose(fic);

    // Affichage initial

    printf("Clair :\t %lx\nChiffré bon :\t %lx\n\n", clair, CB);

    for(int i = 0; i < nbDeCF; i++)
    {
        printf("Chiffré faux n°%d :\t %lx\n", i, TabCF[i]);
    }

    // Calculs

    

    // Fin
    free(TabCF);

    exit(0);
}