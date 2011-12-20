/******************************************************************************
 * En hægted liste, som skal indeholde historie listen
 *
 * Kodet af Kim Brugger Maj 2000 Email: bugger@diku.dk
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Hliste.h"
#include "Dliste.h"
//#include "Dtree.h"
#include "kerne.h"
#include "Dhash.h"
#define dprintf if (0) printf

struct list * MakeList() {
/******************************************************************************
 *  Opretter en ny node og sikre at der er nok hukommelse
 *****************************************************************************/
  struct list* tmp;

  tmp =  malloc(sizeof(struct list));

  if (tmp == 0) {
    perror("Liste allokering");
    exit(3);
  }
  //For at sikre at værdien er nullede
  tmp->pre = 0;
  tmp->post = 0;
  tmp->start = 0;
  return(tmp);
} /* MakeList()*/

struct list* addNode(struct list* Liste) {
/******************************************************************************
 * Tilføjer en node til den angivne
 * Hvis det er den sidste node tilføjes den bare ellers indsættes den.
 *****************************************************************************/
  struct list* nyListe;

  nyListe = MakeList();

  if (Liste == 0) {
    Liste = nyListe;
  }  
  else if (Liste->post == 0) {
    Liste->post = nyListe;
    nyListe->post = 0;
    nyListe->pre  = Liste;
  }
  else {
    nyListe->post = Liste->post;
    nyListe->pre  = Liste;
    Liste->post   = nyListe;
    nyListe->post->pre = nyListe;
  }
  return(Liste);
} /* addNode() */

struct list* insertNode(struct list* Liste) {
/******************************************************************************
 * Indsætter en node til den angivne
 * Hvis det er den sidste node tilføjes den bare ellers indsættes den.
 *****************************************************************************/
  struct list* nyListe;

  nyListe = MakeList();

  if (Liste == 0) {
    Liste = nyListe;
  }  
  else {
    nyListe->pre = Liste->pre;
    nyListe->post= Liste;
    Liste->pre = nyListe;
  }

  return(nyListe);
} /* insertNode() */

void deleteNode(struct list* Liste) {
/******************************************************************************
 * Sletter en node til den angivne
 *****************************************************************************/

  if (Liste->post == 0) 
    Liste->pre->post = 0;
  else if (Liste->pre == 0 ) 
    Liste->post->pre = 0;
  else {
    Liste->pre->post = Liste->post;
    Liste->post->pre = Liste->pre;      
  }
  free(Liste);

} /* deleteNode() */

void PrintListeLong(struct list* Liste) {
/******************************************************************************
 * Udskriver en liste
 *****************************************************************************/
  
  if (Liste == 0) {
    dprintf("NULL\n");
  }
  else {
    dprintf("start : %ld start pre : %-10p post : %-10p self : %-10p\n", 
	    Liste->start, Liste->pre, Liste->post, Liste);
  }

  if (Liste == 0 || Liste->post == 0) {
    return;
  }
  else
    PrintListeLong(Liste->post);

} /* PrintListe() */

void PrintListeShort(struct list* Liste) {
/******************************************************************************
 * Udskriver en liste
 *****************************************************************************/
  
  if (Liste) 
    printf(" %ld", Liste->start);
  //else
    //    printf("NULL\n");
  
  if (Liste == 0 || Liste->post == 0) {
    return;
  }
  else
    PrintListeShort(Liste->post);

} /* PrintListeShort() */

void PrintDist(struct list* Liste, const int value) {
/******************************************************************************
 * Beregner af standende og udskriver dem.
 *****************************************************************************/

  if (Liste && (value - Liste->start)) 
    printf(" %ld->%ld", Liste->start, value - Liste->start);
      
  if (Liste == 0 || Liste->post == 0) {
    return;
  }
  else 
    PrintDist(Liste->post, value);
}

void HashCalcDistGar(struct Dhash* Dhash, 
		     struct list* Liste, 
		     const int value) {
/******************************************************************************
 * Finder div længder og indsætter dem i længde tabellen 
 * Tilgangen til hvor det hele sker .. . . .. . . .. . -
 *****************************************************************************/
  struct dhlist* tmpDHlist;
  //char s[10];
  int dist;
  int i = 0;
  // denne kan være tom da dette fragment ikke eksistere 
  // i den forgående sequence
  if (!Liste)
    return;

  dist = abs(value - Liste->start);

  if (1 || (dist > 0))
    // Afstanden er null så det er det samme bogstav.
    if (((!DistMin) || (DistMin < dist)) &&
	((!DistMax) || (DistMax > dist))) {
      // This is where the delete/insert things come into the game ...
      // This is so niiiiiice _I_ could vomit
      if (Insert == 0) {
	//sprintf(s, "%ld", value - Liste->start);
	
	tmpDHlist = FetchDHash(abs(dist), Dhash);
	
	if (tmpDHlist != (struct dhlist*)-1) {

#define __NOITERATIVE
#ifdef __NOITERATIVE
	  //fprintf(stdout, "tmpDHlist[%d] == %p\n", dist, tmpDHlist);
 	  Dhash->table[dist] =
	    CalcFragRepGar(tmpDHlist, Liste->start, value, 0);
#else
	  sprintf(stderr, "Going iterative\n");
	  Dhash->table[dist] =
	    CalcFragRepGarIterativ(tmpDHlist, Liste->start, value, 0);
#endif
	  //dprintf("--tmpDHlist %p\n", tmpDHlist);
	  //SetDHashValue(value - Liste->start, Dhash, tmpDHlist);
	  //tmpDHlist = FetchDHash(value - Liste->start, Dhash);
	  //dprintf("++tmpDHlist %p\n", tmpDHlist);
	}
	else {
	  perror("Ingen hash i DHash,  BAD BAD BAD\n");
	  exit(9);
	}
      }
      else {
	for (i = -Insert; i < Insert; i++) {
	tmpDHlist = FetchDHash(abs(dist), Dhash);
	
	if (tmpDHlist != (struct dhlist*)-1) {
	  //fprintf(stdout, "tmpDHlist[%d] == %p\n", dist, tmpDHlist);
#ifdef __NOITERATIVE
 	  Dhash->table[dist] =
	    CalcFragRepGar(tmpDHlist, Liste->start, value, i);
#else
	  sprintf(stderr, "Going iterative\n");
	  Dhash->table[value - Liste->start] =
	    CalcFragRepGarIterativ(tmpDHlist, Liste->start, value, i);
#endif
	  }
	  else {
	    perror("Ingen DHash fundet BAD BAD BAD INSERT\n");
	    exit(9);
	  }
	}
      }
    }

  if (Liste->post)
    HashCalcDistGar(Dhash, Liste->post, value);
}
