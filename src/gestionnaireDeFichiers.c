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
            clair = strtoul(line, NULL, base);

            break;
        }

        if(strcmp(line, "Message clair :\n") == 0)
        {
            trigger++;
        }
    }

    return clair;
}

unsigned long int extractionDuCB(FILE *fic)
{
    unsigned long int CB = 0x0;

    char *line = NULL;
    size_t n = 0;
    int trigger = 0;

    while(trigger <= 1)
    {
        n = getline(&line, &n, fic);

        if(trigger > 0)
        {
            CB = strtoul(line, NULL, base);
            
            break;
        }

        if(strcmp(line, "Message chiffré juste :\n") == 0)
        {
            trigger++;
        }
    }

    return CB;
}

unsigned long int *extractionDesCF(FILE *fic)
{
    unsigned long int *TabCF = malloc(sizeof(unsigned long int) * nbDeCF);

    char *line = NULL;
    size_t n = 0;
    int trigger = 0;

    while(trigger <= 1)
    {
        n = getline(&line, &n, fic);

        if(trigger > 0)
        {
            for(int i = 0; i < nbDeCF; i++)
            {
                TabCF[i] = strtoul(line, NULL, base);

                n = getline(&line, &n, fic);
            }

            trigger++;
        }

        if(strcmp(line, "Messages chiffrés faux :\n") == 0)
        {
            trigger++;
        }
    }

    return TabCF;
}