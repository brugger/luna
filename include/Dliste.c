/******************************************************************************
 * En hægted liste indeholdende nye hægtede lister.
 * Kodet af Kim Brugger Maj 2000 Email: bugger@diku.dk
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "Dliste.h"
#include "kerne.h"

#define dprintf if (0) printf
#define perfect 1

struct dhlist * MakeDHList() {
/******************************************************************************
 *  Opretter en ny node og undersøger om der er nok hukommelse
 *****************************************************************************/
  struct dhlist* tmpDHlist;

  tmpDHlist =  malloc(sizeof(struct dhlist));

  if (tmpDHlist == 0) {
    perror("Liste allokering");
    exit(3);
  }
  //For at sikre at værdien er nullede
  tmpDHlist->pre = 0;
  tmpDHlist->post = 0;
  tmpDHlist->start1 = tmpDHlist->stop1 = tmpDHlist->start2 = tmpDHlist->stop2 = 0;
  tmpDHlist->Ins = 0;
  tmpDHlist->fragNum = 0;
  //  dprintf("Ny   dhliste(%10p)\n", tmpDHlist);
  return(tmpDHlist);

} /* MakeDHList()*/


struct dhlist* addDHNode(struct dhlist* Liste) {
/******************************************************************************
 * Tilføjer en node til den angivne
 * Hvis det er den sidste node tilføjes den bare ellers indsættes den.
 *****************************************************************************/
  struct dhlist* nyListe;

  nyListe = MakeDHList();

  if (Liste == 0) {
    Liste = nyListe;
  }  
  else if (Liste->post == 0) {
    Liste->post = nyListe;
    nyListe->post = 0;
    nyListe->pre  = Liste;
  }
  else {
    nyListe->post = Liste;
    nyListe->pre  = Liste->pre;
    Liste->pre->post = nyListe;
    Liste->pre   = nyListe;
  }

  return(Liste);
} /* addDHNode() */


struct dhlist* DeleteDHNode(struct dhlist* Liste) {
/******************************************************************************
 * Sletter en node.
 *****************************************************************************/
  struct dhlist* ret;

  //  fprintf(stderr, "deleting node \n");
  if (Liste->pre == 0 && Liste->post == 0) {
    ret = NULL;
  }
  else if (Liste->post == 0) {
    Liste->pre->post = NULL;
    ret = NULL;
  }
  else if (Liste->pre == 0) {
    Liste->post->pre = NULL;
    ret = Liste->post;
  }
  else {
    Liste->post->pre = Liste->pre;
    Liste->pre->post = Liste->post;
    ret = Liste->post;
  }

  free(Liste);
  return(ret);

} /* DeleteDHNode*/

struct dhlist * CalcFragRepGar(struct dhlist * dhliste, 
			       const int start1, 
			       const int start2,
			       const int ins) {
/******************************************************************************
 * Løber en dhliste igennem, og undersøger om det er del af en struktur, ellers
 * oprettes der en ny node som værdierne gemmes i.
 *****************************************************************************/
  int dist;
  // Guldet i systemet ....!!!!.... .

  // If there dont exists a list one is created
  if (!dhliste) {
    // Since this is a insertion it shouldn't be created after all
    if (ins != 0)
      return NULL;

    dhliste = MakeDHList();
    dhliste->start1 =  start1;
    dhliste->start2 =  start2;
    dhliste->stop1  = start1 + klgde - 1;
    dhliste->stop2  = start2 + klgde - 1;
    dhliste->fragNum    =  0;  

    // Since this element was just created there is no reason in checking 
    // for other elements in the same distance
    return(dhliste);
  }

  dprintf("info dhliste(%10p) pre:%10p post:%10p\n", 
	 dhliste, dhliste->pre, dhliste->post);

  if ((dhliste->start1 < start1) && 
      (dhliste->stop1 + gap + klgde >= start1) && 
      (dhliste->start2 <= start2) &&
      (dhliste->stop2 + gap + klgde >= start2)) {

    if (ins)
      dhliste->Ins = 1;

    dhliste->stop1 = start1 + klgde - 1;
    dhliste->stop2 = start2 + klgde - 1;
    dhliste->fragNum = dhliste->fragNum + 1;

    dprintf("  dhliste(%10p) start1:%10ld start2:%10ld\n", 
    	    dhliste, dhliste->start1, dhliste->start2);

  }
  else if (dhliste->post){
    //    dprintf("recursiv\n");
    dhliste->post = CalcFragRepGar(dhliste->post, start1, start2, ins);
  }
  else {
    // No one matches, we create a new one
    dhliste = addDHNode(dhliste);
    dhliste->start1 =  start1;
    dhliste->start2 =  start2;
    dhliste->stop1  = start1 + klgde - 1;
    dhliste->stop2  = start2 + klgde - 1;

    //Da denn post lige er oprettet er der ingen grund til at checke
    return(dhliste);
  }

  // Skal vi slette et par poster dhlister sammen ????
  // Hvis Mem >= 2 printer vi også de poster der ikke kan gro mere....

  dist = dhliste->stop1 - dhliste->start1 + 1; 
  if (((dist < min) &&
       (dhliste->stop2 + gap + klgde <= start2)) ||
      ((dist >= min) && (dhliste->stop2 + gap + klgde <= start2))){

    //    dprintf("del  dhliste(%10p) pre:%10p post:%10p\n", 
    //	    dhliste, dhliste->pre, dhliste->post);

    if ((dist >= min) && 
	(dhliste->stop2 + gap + klgde <= start2)) {

      //      printf("Q %p <%ld, %ld> > %ld\n", 
      //	     dhliste, dhliste->start1, dhliste->stop1, dist);

      if ((!max) || (max > dist))
	printSeq(dhliste->start1, dhliste->stop1, 
		 dhliste->start2, dhliste->stop2,
		 dhliste->Ins);
    }
    return (DeleteDHNode(dhliste));
  }

  return(dhliste);
}


struct dhlist * CalcFragRepGarIterativ(struct dhlist * dhliste, 
				       const int start1, 
				       const int start2,
				       const int ins) {
/******************************************************************************
 * Løber en dhliste igennem, og undersøger om det er del af en struktur, ellers
 * oprettes der en ny node som værdierne gemmes i.
 * En iterativ form som forventer kun eet hit pr liste og sætter hittet i front
 *****************************************************************************/
  //  int dist;
  struct dhlist * tmpdhl, * tmpdhl2;
  // Guldet i systemet ....!!!!.... .

  //Hvis dhlisten ikke eksistere opretter vi en...
  if (!dhliste) {
    if (ins != 0)
      return NULL;
    dhliste = MakeDHList();
    dhliste->start1 =  start1;
    dhliste->start2 =  start2;
    dhliste->stop1  = start1 + klgde - 1;
    dhliste->stop2  = start2 + klgde - 1;
    //Da denn post lige er oprettet er der ingen grund til at checke
    dprintf("\nFIRSTtmpdhl : %p (post : %p)\n",dhliste, dhliste->post);
    return(dhliste);
  }

  tmpdhl = dhliste;
  dprintf("\nPre LOOP tmpdhl : %p (post : %p)\n",dhliste, dhliste->post);
  
  while (tmpdhl) {
    // We have a hit, lets take it out and put it in front ;)
    if ((tmpdhl->start1 < start1) && 
	(tmpdhl->stop1 + gap + klgde >= start1) && 
	(tmpdhl->start2 <= start2) &&
	(tmpdhl->stop2 + gap + klgde >= start2)) {
      
      if (ins)
	tmpdhl->Ins = 1;

      tmpdhl->stop1 = start1 + klgde - 1;
      tmpdhl->stop2 = start2 + klgde - 1;
      break;
      // It is already the first element, there is no need to move it then
      if (tmpdhl->pre != 0)
	break;

      if (tmpdhl->post != 0)
	tmpdhl->post->pre = tmpdhl->pre;

      if (tmpdhl->pre != 0)
	tmpdhl->pre->post = tmpdhl->post;

      dhliste->pre = tmpdhl;
      dhliste->post = tmpdhl;
      break;
    }
    else if (tmpdhl->post){
      tmpdhl2  = tmpdhl->post;
      tmpdhl = tmpdhl2;      
      dprintf("tmpdhl : %p (post : %p)\n",tmpdhl,tmpdhl->post);
    }
    else {
      // ikke flere poster, så vi opretter en.
      tmpdhl = addDHNode(tmpdhl);
      tmpdhl->post->start1 =  start1;
      tmpdhl->post->start2 =  start2;
      tmpdhl->post->stop1  = start1 + klgde - 1;
      tmpdhl->post->stop2  = start2 + klgde - 1;
      dprintf("\nnew : %p\n",tmpdhl);
      //    dhliste->antFrag    =  0;  
      //Da denn post lige er oprettet er der ingen grund til at checke
      break;
      //return(tmpdhl);
    }
  }
  // Skal vi slette et par poster dhlister sammen ????
  dprintf("break : %p (post : %p)\n",tmpdhl,tmpdhl->post);

  return(tmpdhl);
}

struct dhlist * DHFlush(struct dhlist * DHliste, const int pos) {
/*****************************************************************************
 * Grand Flusher alt i denne liste.
 *****************************************************************************/
  long dis;

  if (DHliste->post)
    DHliste->post = DHFlush(DHliste->post, pos);

  dis = DHliste->stop1 - DHliste->start1 + 1; 
  if (DHliste->stop2 + gap + klgde < pos) {
    /*
      dprintf("Flush  dhliste(%10p) pre:%10p post:%10p\n", 
      DHliste, DHliste->pre, DHliste->post);
	    
      dprintf("Q %d <%ld, %ld> > %d\n", 
      pos, DHliste->start2, DHliste->stop2, DHliste->Ins);
    */
    if (dis > min) {
      //      dprintf("F %p <%ld, %ld> > %ld\n", 
      //      DHliste, DHliste->start2, DHliste->stop2, dis);
      if ((!max) || (max > dis))
	printSeq(DHliste->start1, DHliste->stop1, 
		 DHliste->start2, DHliste->stop2,
		 DHliste->Ins);
    }
    return (DeleteDHNode(DHliste));    
  }
  //  dprintf("S %d <%ld, %ld> > %ld\n", pos, DHliste->start2, DHliste->stop2, dis);
  return (DHliste);    
}

struct dhlist * DHLastFlush(struct dhlist * DHliste) {
/*****************************************************************************
 * Flusher alt i denne liste.
 *****************************************************************************/
  long dis;

  if (DHliste->post)
    DHliste->post = DHLastFlush(DHliste->post);

  dis = DHliste->stop1 - DHliste->start1 + 1; 

  if (dis > min) {
    if ((!max) || (max > dis))
      printSeq(DHliste->start1, DHliste->stop1, 
	       DHliste->start2, DHliste->stop2,
	       DHliste->Ins);
  }
  return (DeleteDHNode(DHliste));    
}
