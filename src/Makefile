PROGRAM = Baller
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib 
LDLIBS  = -lglut -lGLU -lGL

$(PROGRAM): main.o object.o pomocne.o image.o
	$(CC) $(LDFLAGS) -o $(PROGRAM) $^ $(LDLIBS) -lm

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $<

object.o : object.c object.h
	$(CC) $(CFLAGS) -c -o $@ $<

pomocne.o : pomocne.c pomocne.h
	$(CC) $(CFLAGS) -c -o $@ $<

image.o : image.c image.h
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	-rm *.o $(PROGRAM) 
