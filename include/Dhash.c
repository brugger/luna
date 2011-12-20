/******************************************************************************
 * Implemtering af en afstands hash table til søgninger.
 * Kodet af Kim Brugger Marts 2001 bugger@diku.dk
 * Version 0.00
 *****************************************************************************/
#include "Dhash.h"
#include "Dliste.h"
#include "kerne.h"

struct Dhash* MakeDHash(const int length) {
/******************************************************************************
 *  Opretter en Distance hash tabel.
 *****************************************************************************/
  struct Dhash* table;
  int i;
  table = malloc(sizeof(struct Dhash));
  table->table = malloc(length * sizeof(struct dhlist *));

  if (table->table == 0) {
    perror("MakeDHash");
    exit(3);
  }

  table->length = length;

  // Zero the bastard.
  for (i = 0; i< length;i++)
    table->table[i] = NULL;
  //dprintf("allocated %d\n", length * sizeof(struct dhlist **));

  //fprintf(stderr, "Created Dhash table\n");
  return table;
}

struct dhlist * FetchDHashNew(const int dist, struct Dhash* table) {
/******************************************************************************
 * Søger efter noden, hvis noden er tom oprettes den.
 *****************************************************************************/
  //if (0)
  int bucket = 0;
  //fprintf(stderr, "Hash størrelse : %d\n", table->length);
  bucket = dist % table->length;
  //fprintf(stderr, "dist %d --> bucket%d\n", dist, bucket);
  //  printf("fetch %d -- ", dist);
  if (( bucket>= 0) && (bucket < table->length)) {
    if (table->table[bucket] != NULL) {
      //dprintf ("returning %p\n", table->table[dist]);
      return table->table[bucket];
    }
    else {
      table->table[bucket] = MakeDHList();
      //dprintf("Created dhlist %p\n", table->table[dist]);
      return table->table[bucket];
    }
  }
  fprintf(stderr, "Distance hash error, I have to DIE\n");
  exit (9);
  return NULL;
}

struct dhlist * FetchDHash(const int dist, struct Dhash* table) {
/******************************************************************************
 * Søger efter noden, hvis noden er tom oprettes den.
 *****************************************************************************/
  if ((dist >= 0) && (dist < table->length)) {
    if (table->table[dist] != NULL) {
      //dprintf ("returning %p\n", table->table[dist]);
      return table->table[dist];
    }
    else {
      //table->table[dist] = MakeDHList();
      //dprintf("Created dhlist %p\n", table->table[dist]);
      return NULL;//table->table[dist];
    }
  }
  fprintf(stderr, "dist : %d length : %d\n", dist, table->length);
  return (struct dhlist *) -1;
}

void SetDHashValue(const int dist, 
		   struct Dhash* table, 
		   const struct dhlist * dhliste) {
/******************************************************************************
 *
 *****************************************************************************/
  if ((dist >= 0) && (dist < table->length))
      table->table[dist] = (struct dhlist *)dhliste;
}

void DHashFlush(struct Dhash* table, int pos) {
/******************************************************************************
 * Flusher alle døde fragmenter og udskriver de "levende"
 *****************************************************************************/
  int i;
  //dprintf("HashFlush *shiver*\n");
  for (i = 0; i < table->length;i++)
    if (table->table[i])
      table->table[i] = DHFlush(table->table[i], pos);
}

void DHashLastFlush(struct Dhash* table) {
/******************************************************************************
 * Flusher alle døde fragmenter og udskriver de "levende"
 *****************************************************************************/
  int i;
  //dprintf("Last HashFlush *shiver*\n");
  for (i = 0; i < table->length;i++)
    if (table->table[i])
      table->table[i] = DHLastFlush(table->table[i]);
}

void DHashPrintAll(struct Dhash* table, int min) {
/******************************************************************************
 * 
 *****************************************************************************/
  int i;
  //printf("Print All in DHash with a pointer\n");
  for (i = 0; i < table->length;i++)
    if (table->table[i])
      fprintf(stderr, "%d == %p\n", i , table->table[i]);
}
