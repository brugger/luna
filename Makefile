# En Makefile som den burde v�re lavet fra starten
# Lavet til luna og venner.
# Lavet af Kim Brugger Sommeren 2000

ILIB   = ./include/
PROG   = luna luna2seq
WARN   = #-Wshadow  -Wmissing-prototypes
FLAGS  = -pipe -Wall -ggdb -I$(ILIB) -D_POSIX_SOURCE $(WARN)\
 -O6 -D__LUNA_DEBUG #-pg
CC     = gcc
LIBS   = -lm #-lefence #-ldmalloc  #-lefence #-lmcheck#-lmalloc
RM     = rm -f
MAKE   = gmake
SRCS   = $(ILIB)Dliste.c $(ILIB)Hliste.c $(ILIB)Hash.c\
$(ILIB)kerne.o $(ILIB)sw.c $(ILIB)texts.c luna.c
OBJS   = $(ILIB)Dliste.o $(ILIB)Hliste.o $(ILIB)Hash.o\
         $(ILIB)kerne.o $(ILIB)sw.o $(ILIB)texts.o $(ILIB)Dhash.o


all :  $(PROG)  

luna    :  luna.o $(OBJS) 
	$(CC) $(FLAGS)  -o  $@  $<  $(OBJS) $(LIBS)

lunainv : lunainv.o $(OBJS) 
	$(CC) $(FLAGS)  -o  $@  $< $(OBJS) $(LIBS)

luna2seq : luna2seq.o $(OBJS) 
	$(CC) $(FLAGS)  -o  $@  $< $(OBJS) $(LIBS)

lunafrag : lunafrag.o $(OBJS) 
	$(CC) $(FLAGS)  -o  $@  $< $(OBJS) $(LIBS)

%.o : %.c
	$(CC) $(FLAGS) -c  $< -o $@

doc:
	cd Doc;pwd ;latex -interaction=nonstopmode LunaAlgo.tex

cleandoc:
	cd Doc; $(RM) *.ps *.aux  *.dvi  *.log  *.toc *~

clean: 
	$(RM) $(PROG)

veryclean : cleandoc
	$(RM) $(ILIB)*.o *.o core $(PROG) *~ $(ILIB)*~ *.bak\

depend:
	makedepend -- $(FLAGS) -- $(SRCS)
# DO NOT DELETE
