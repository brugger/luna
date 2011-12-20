CC         = gcc
FLAGS      = -Wall -ggdb -D_GNU_SOURCE -lmcheck
LIBDIRS    =
RM         = rm -f
PROG	   = s3
OFILES	   = kerne.o  s3.o  sw.o

all	: test

$(PROG) : $(OFILES)
	gcc -o  $@  $(OFILES) $(FLAGS) $(OPTIMIZERS) $(LIBDIRS) $(LIBS)

%.o : %.c %.h
	$(CC) $(FLAGS) $(OPTIMIZERS) -c -o $@  $< 

test: $(PROG)
	./$(PROG) <  t3

clean:
	rm -f $(PROG)

veryclean :
	$(RM) *.o core $(PROG)
