/******************************************************************************
 * Implemtering af en afstands hash.
 * Til erstatning for det gamle D-tree.
 * Kodet af Kim Brugger Maj 2000 bugger@diku.dk
 * Version 0.00
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#ifndef __DHASH
#define __DHASH

struct Dhash {
  struct dhlist **table;
  int length;
};

struct Dhash* MakeDHash(const int length);
/******************************************************************************
 *  Opretter en Distance hash tabel.
 *****************************************************************************/

struct dhlist * FetchDHash(const int dist, struct Dhash* tabel);
/******************************************************************************
 * 
 *****************************************************************************/

void SetDHashValue(const int dist, struct Dhash* table, const struct dhlist * dhliste);
/******************************************************************************
 * 
 *****************************************************************************/

void DHashFlush(struct Dhash* tabel, int pos);
/******************************************************************************
 * Flusher alle døde fragmenter.
 *****************************************************************************/

void DHashLastFlush(struct Dhash* table);
/******************************************************************************
 * Flusher alle døde fragmenter og udskriver de "levende"
 *****************************************************************************/


void DHashPrintAll(struct Dhash* tabel, int min);
/******************************************************************************
 * Printer alle pointere i hashen.
 *****************************************************************************/

#endif
