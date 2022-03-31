// DIRECTIVES
#ifndef GESTIONNAIREDEFICHIERS_H
#define GESTIONNAIREDEFICHIERS_H

// LIBRAIRIES
#include "constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// FONCTIONS
uint64_t extractionDuClair(FILE *fic);
uint64_t extractionDuChiffreBon(FILE *fic);
uint64_t *extractionDesChiffresFaux(FILE *fic);

#endif