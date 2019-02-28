CFLAGS = -g -O2
LIBS = -lreadline

all: gsh

gsh: gsh.o gsh_gc.o gsh_parser.o
	$(CC) -o $@ $? $(LIBS) 

%.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $<

.PHONY: clean

clean:
	-rm -f gsh
	-rm -f *.o
