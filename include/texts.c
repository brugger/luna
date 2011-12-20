/******************************************************************************
 * Disclaimer + usage and other text stuff 
 *****************************************************************************/
#include "texts.h"

void disclaim() {
/******************************************************************************
 *
 *****************************************************************************/
  fprintf(stderr,  "\n");
  fprintf(stderr, "THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,\n");
  fprintf(stderr, "INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,\n");
  fprintf(stderr, "IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT\n");
  fprintf(stderr, "OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM\n");
  fprintf(stderr, "LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,\n");
  fprintf(stderr, "NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN\n");
  fprintf(stderr, "CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n");
  fprintf(stderr, "\nUSE THIS SOFTWARE AT OWN RISK\n");
  exit(0);
}

void Disclaim() {
/******************************************************************************
 * Eastern eeg message :)
 *****************************************************************************/
  fprintf(stderr, "\n");
  fprintf(stderr, "By executing this code you agree to the following:\n");
  fprintf(stderr, "I am NOT to be held responsible for any damage caused to your system\n");
  fprintf(stderr, "by executing this code or damage caused by strange phenomenae during\n");
  fprintf(stderr, "the run of this program. Neither can I be held responsible\n");
  fprintf(stderr, "if damage to your software or other peoples computer systems occurs,\n");
  fprintf(stderr, "if police-men or men in black kick in your front/back/room door, your\n");
  fprintf(stderr, "dog or cat or favourte pet ant dies in agonizing pain, your girl-friend\n"
);
  fprintf(stderr, "leaves you and hangs out with Spice Girls because she caught you\n");
  fprintf(stderr, "running this, the height of your phone-bill, the electric\n");
  fprintf(stderr, "lights get knocked out in your town, you get ambushed by villains with\n")
;
  fprintf(stderr, "chain-saws, global disorder occurs, armageddon, the sun turning into\n");
  fprintf(stderr, "a super nova, the Big Crunch suddenly happens, or if you\n");
  fprintf(stderr, "paper-cut your fingers opening envelopes the rest of your life.\n");
  fprintf(stderr, "I guess I'm pretty safe now...Enjoy Kim Brugger\n\n");
  exit(0);
}


void copyright() {
/******************************************************************************
 *
 *****************************************************************************/
  fprintf(stderr, "This program is written by Kim Brugger (brugger@mermaid.molbio.ku.dk)\n");
  fprintf(stderr, "All of this work is original by me and therefore I (Kim Brugger)\n");
  fprintf(stderr, "have full copyright (c) 2000-2002 to this program and its sourcecode.\n");
}


void version() {
/******************************************************************************
 * I own everything...
 *****************************************************************************/
  fprintf(stderr, "This is version %s of %s.\n", Version, ProgramName);
  fprintf(stderr,"The program is written by Kim Brugger (brugger@mermaid.molbio.ku.dk)\n");
  disclaim();
  exit(0);
}


void usage() {
/******************************************************************************
 * The name says it all.
 *****************************************************************************/

  printf("\nUsage of %s apply one or more of the following parameters: \n\n",
	 ProgramName);
  printf("-k|--ktub value      : The size of the tuble. A small tuble gives a long running time\n");
  printf("-g|--gap value       : The maximum gap size that is allowed\n");
  printf("-m|--min value       : The lower size cut-off\n");
  printf("-M|--max value       : The upper size cut-off\n");
  printf("-d|--distmin value   : The minimum distance between two sub-sequences\n");
  printf("-D|--distmax value   : The maximum distance between two sub-sequences\n");
  printf("-I|--Insert value    : The indel size\n");
  printf("--direction          : What strands to compare\n");
  printf("                         1. Foward and Reverse\n");
  printf("                         2. Foward only\n");
  printf("                         3. Reverse only\n");
  printf("-f|--flank value     : How much flank that should be added to the output\n");
  printf("--pagesidth value    : How wide is the page (default 80)\n");
  printf("--printout           : How the program should show the results :\n");
  printf("                         1. Aligned sequences\n");
  printf("                         2. Sequences only\n");
  printf("                         3. Positions and scores only\n");
  printf("-v|--verbose         : Runs verbose (repeat for more info)\n");
  printf("--disclaimer         : disclaimer\n");
  printf("-V|--Version         : Program version\n");
  printf("-h|--help            : This message\n\n");

  copyright();
  exit(0);
}

void PrintVerbose() {
/******************************************************************************
 * Prints verbose information.
 *****************************************************************************/
  if (max)
    printf("With upper-cutoff : %d\n", max);
  if (min)
    printf("With lower-cutoff : %d\n", min);
  if (DistMin)
    printf("Minimun distance between sub-sequences : %d\n", DistMin);
  if (DistMax)
    printf("Maximun distance between sub-sequences : %d\n", DistMax);
  if (flank)
    printf("Printed with flanks : %d\n", flank);
  printf("Printed pagewidth : %d\n", PageWidth);
}
