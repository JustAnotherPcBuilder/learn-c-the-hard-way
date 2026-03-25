CFLAGS=-Wall -g -std=c99
CC = gcc

PROGS := $(patsubst %.c, %,$(wildcard *.c))

all: $(PROGS)

%: %.c
	gcc $(CFLAGS) -o $@ $<