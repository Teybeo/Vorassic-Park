#ifndef H_LISTE
#define H_LISTE

#include <stdio.h>
#include <stdlib.h>

#include "header/general.h"

void affichagePile(ElemPoint *pile);
ElemPoint* empiler(ElemPoint *tetepile, Point depart);
ElemPoint* depiler(ElemPoint* tetepile);
void* liberePile(ElemPoint* tetepile);

#endif
