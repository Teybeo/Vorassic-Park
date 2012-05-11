#ifndef H_LISTE
#define H_LISTE

#include <stdlib.h>

#include "header/general.h"

ElemPoint* empiler(ElemPoint *tetepile, Point depart);
ElemPoint* depiler(ElemPoint* tetepile);
void* liberePile(ElemPoint* tetepile);

#endif
