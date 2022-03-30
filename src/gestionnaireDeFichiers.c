// LIBRAIRIES
#include "gestionnaireDeFichiers.h"

// FONCTIONS

unsigned long int extractionDuClair(FILE *fic)
{
    unsigned long int clair = 0x0a;

    char *line = NULL;
    size_t n = 0;
    int trigger = 0;

    while(trigger <= 1)
    {
        n = getline(&line, &n, fic);

        if(trigger > 0)
        {
            clair = strtoul(line, NULL, BASE);

            break;
        }

        if(strcmp(line, "Message clair :\n") == 0)
        {
            trigger++;
        }
    }

    return clair;
}

unsigned long int extractionDuChiffreBon(FILE *fic)
{
    unsigned long int chiffreBon = 0x0;

    char *line = NULL;
    size_t n = 0;
    int trigger = 0;

    while(trigger <= 1)
    {
        n = getline(&line, &n, fic);

        if(trigger > 0)
        {
            chiffreBon = strtoul(line, NULL, BASE);
            
            break;
        }

        if(strcmp(line, "Message chiffré juste :\n") == 0)
        {
            trigger++;
        }
    }

    return chiffreBon;
}

unsigned long int *extractionDesChiffresFaux(FILE *fic)
{
    unsigned long int *chiffresFaux = malloc(sizeof(unsigned long int) * NOMBRE_CHIFFRES_FAUX);

    char *line = NULL;
    size_t n = 0;
    int trigger = 0;

    while(trigger <= 1)
    {
        n = getline(&line, &n, fic);

        if(trigger > 0)
        {
            for(int i = 0; i < NOMBRE_CHIFFRES_FAUX; i++)
            {
                chiffresFaux[i] = strtoul(line, NULL, BASE);

                n = getline(&line, &n, fic);
            }

            trigger++;
        }

        if(strcmp(line, "Messages chiffrés faux :\n") == 0)
        {
            trigger++;
        }
    }

    return chiffresFaux;
}