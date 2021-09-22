SDIR=./src
IDIR =./include
ODIR=./obj

CC=gcc
CFLAGS=-std=c99 -g3 -Wall -I$(IDIR)

GTKCFLAGS=`pkg-config --cflags gtk+-3.0`
GTKLIBS=`pkg-config --libs gtk+-3.0`

LIBS=-lm

all: rCalc
	./rCalc

_DEPS = calc.h callbacks.h stack.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o callbacks.o calc.o stack.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/main.o: $(SDIR)/main.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS) $(GTKCFLAGS)

$(ODIR)/callbacks.o: $(SDIR)/callbacks.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS) $(GTKCFLAGS)
 
$(ODIR)/calc.o: $(SDIR)/calc.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ODIR)/stack.o: $(SDIR)/stack.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 


rCalc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(GTKCFLAGS) $(GTKLIBS) $(LIBS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o ./rCalc
