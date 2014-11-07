.PHONY: cfg parray.o tokenize

CC      = gcc
CCFLAGS = -Wall -O3 
RM      = rm

all: cfg

cfg:
	$(CC) $(CCFLAGS) -o cfg cfg.c

parray.o:
	$(CC) $(CCFLAGS) -c parray.c

tokenize: parray.o
	$(CC) $(CCFLAGS) parray.o -o tokenize tokenize.c

diff:
	git diff | colordiff | less -R


clean:
	$(RM) cfg || true
	$(RM) parray.o || true
	$(RM) *~

