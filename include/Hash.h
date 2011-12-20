/******************************************************************************
 * En hash liste, til opslag af DNA fragmenter
 *
 * Kodet af Kim Brugger Juli 2000 Email: bugger@diku.dk
 *****************************************************************************/
#ifndef __HASH
#define __HASH

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Hliste.h" 

typedef struct list **DNAhash;

DNAhash MakeDNAhash(const int length);
/******************************************************************************
 *
 *****************************************************************************/

inline int ATGChash(const char * s);
/******************************************************************************
 * Calculates a  unique interger from a DNA seq. 
 * The method was developed by Peter Redder.
 *****************************************************************************/

int FetchDNA(const char * s);
/******************************************************************************
 * Fetch the wanted instance from the array.
 *****************************************************************************/

void printDNAhash(const int length, const DNAhash hash);
/******************************************************************************
 *
 *****************************************************************************/

#endif
