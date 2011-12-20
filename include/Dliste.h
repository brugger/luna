/******************************************************************************
 * En h�gted liste indeholdende nye h�gtede lister.
 * Kodet af Kim Bruger Maj 2000 Email: bugger@diku.dk
 *****************************************************************************/

#ifndef __DLISTE
#define __DLISTE
#include <stdlib.h>

#include "kerne.h"

struct dhlist {
  struct dhlist *pre, *post;
  long fragNum;
  long start1, stop1, start2, stop2;
  char Ins;
};



struct dhlist * MakeDHList();
/******************************************************************************
 *  Opretter en ny node og unders�ger om der er nok hukommelse
 *****************************************************************************/

struct dhlist* addDHNode(struct dhlist* Liste);
/******************************************************************************
 * Tilf�jer en node til den angivne
 * Hvis det er den sidste node tilf�jes den bare ellers inds�ttes den.
 *****************************************************************************/

struct dhlist* DeleteDHNode(struct dhlist* Liste);
/******************************************************************************
 * Sletter en node.
 *****************************************************************************/

struct dlist* findDliste(struct dlist *Liste, int value);
/******************************************************************************
 * Finder en Dliste med den �nskede v�rdi eller opretter en
 *****************************************************************************/

struct dhlist *CalcFragRepGar(struct dhlist * dhliste, 
			      const int start1, 
			      const int start2,
			      const int ins);
/******************************************************************************
 * Finder en Dliste med den �nskede v�rdi eller opretter en
 *****************************************************************************/

struct dhlist * CalcFragRepGarIterativ(struct dhlist * dhliste, 
				       const int start1, 
				       const int start2,
				       const int ins);
/******************************************************************************
 * L�ber en dhliste igennem, og unders�ger om det er del af en struktur, ellers
 * oprettes der en ny node som v�rdierne gemmes i.
 *****************************************************************************/

struct dhlist * DHFlush(struct dhlist * DHliste, const int pos);
/******************************************************************************
 * Grand Flusher alt i denne liste.
 *****************************************************************************/

struct dhlist * DHLastFlush(struct dhlist * DHliste);
/*****************************************************************************
 * Flusher alt i denne liste.
 *****************************************************************************/

#endif
