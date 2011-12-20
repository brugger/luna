/**********************************************************************************
 * En hægted liste, som skal indeholde historie listen
 *
 * Kodet af Kim Brugger Maj 2000 Email: bugger@diku.dk
 *********************************************************************************/
#ifndef __HLISTE
#define __HLISTE

#include "Dliste.h"
//#include "Dtree.h"
#include "Dhash.h"

struct list {
  struct list *pre, *post;
  //struct list *post;
  long start;
};

struct list* MakeList();
/******************************************************************************
 *  Opretter en ny node og undersøger om der er nok hukommelse
 *****************************************************************************/

struct list* addNode(struct list* Liste);
/******************************************************************************
 * Tilføjer en node til den angivne
 * Hvis det er den sidste node tilføjes den bare ellers indsættes den.
 *****************************************************************************/

struct list* insertNode(struct list* Liste);
/******************************************************************************
 * Indsætter en node til den angivne
 * Hvis det er den sidste node tilføjes den bare ellers indsættes den.
 *****************************************************************************/

void deleteNode(struct list* Liste);
/******************************************************************************
 * Sletter en node til den angivne
 *****************************************************************************/

void PrintListelong(struct list* Liste);
/******************************************************************************
 * Udskriver en liste
 *****************************************************************************/

void PrintListeShort(struct list* Liste);
/******************************************************************************
 * Udskriver en liste
 *****************************************************************************/
void PrintDist(struct list* Liste, const int value);
/******************************************************************************
 * Beregner af standende og udskriver dem.
 *****************************************************************************/

void HashCalcDistGar(struct Dhash* Dhash, 
		     struct list* Liste, 
		     const int value);
/******************************************************************************
 * Finder div længder og indsætter dem i længde tabellen
 *****************************************************************************/

#endif
