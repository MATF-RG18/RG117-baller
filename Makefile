PROGRAM = Baller
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib 
LDLIBS  = -lglut -lGLU -lGL

$(PROGRAM): main.o
	$(CC) $(LDFLAGS) -o $(PROGRAM) main.o $(LDLIBS) -lm

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	-rm *.o $(PROGRAM) 
