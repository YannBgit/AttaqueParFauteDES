// DIRECTIVES
#ifndef GESTIONNAIREDEFICHIERS_H
#define GESTIONNAIREDEFICHIERS_H

// LIBRAIRIES
#include "constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FONCTIONS
unsigned long int extractionDuClair(FILE *fic);
unsigned long int extractionDuCB(FILE *fic);
unsigned long int *extractionDesCF(FILE *fic);

#endif