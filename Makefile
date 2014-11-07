.PHONY: cfg 
#parray.o tokenize tokenize.o

CC      = gcc
#CCFLAGS = -Wall -O3 
CCFLAGS = -Wall -g -O0 
RM      = rm

all: cfg tokenize

debug:

cfg: parray.o tokenize.o
	$(CC) $(CCFLAGS) parray.o tokenize.o -o cfg cfg.c

parray.o:
	$(CC) $(CCFLAGS) -c parray.c

tokenize.o: parray.o
	$(CC) $(CCFLAGS) -DTOKENIZE_TESTCASE=0 -c tokenize.c

tokenize: parray.o
	$(CC) $(CCFLAGS) -DTOKENIZE_TESTCASE=1 parray.o -o tokenize tokenize.c

diff:
	git diff | colordiff | less -R


clean:
	$(RM) cfg || true
	$(RM) parray.o || true
	$(RM) tokenize.o || true
	$(RM) tokenize || true
	$(RM) *~

