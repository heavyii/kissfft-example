

CC?=gcc
CFLAGS=-g -O1 -Wall

.SUFFIXES: .c .o

ALL = real-fft
all: $(ALL)

real-fft: kiss_fft.o  kiss_fftr.o  test_real.o
	$(CC) $(CFLAGS) -o $@ $^ -lm


		
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $*.c
	
.PHONY: clean
clean:
	rm -f *.o 
	rm -f $(ALL)
	
