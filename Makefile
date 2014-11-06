.PHONY: cfg

CC      = gcc
CCFLAGS = -Wall -O3 
RM      = rm

all: cfg

cfg:
	$(CC) $(CCFLAGS) -o cfg cfg.c

diff:
	git diff | colordiff | less -R


clean:
	$(RM) cfg || true
	$(RM) *~

