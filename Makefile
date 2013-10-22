

CC?=gcc

WARNINGS=-W -Wall -Wstrict-prototypes -Wmissing-prototypes -Waggregate-return \
    -Wcast-align -Wcast-qual -Wnested-externs -Wshadow -Wbad-function-cast \
    -Wwrite-strings
.SUFFIXES: .c .o

CFLAGS=-g -O1 $(WARNINGS)

ALL = real-fft
all: $(ALL)

real-fft: kiss_fft.o  kiss_fftr.o  data_processor.o data_processor_test.o
	@$(CC) $(CFLAGS) -o $@ $^ -lm

.c.o:
	@$(CC) $(CFLAGS) -o $@ -c $*.c
	
.PHONY: clean
clean:
	rm -f *.o 
	rm -f $(ALL)
	
