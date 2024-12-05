# Makefile for hangman.c and hangman.h
# Author: Hunter Chi
# Date: 12/1/2024
CC=gcc
CFLAGS=-g -Wall
MFLAG=-lm
targets = hangman
all: $(targets)

# $@ is the name of the target

%: %.c
	$(CC) $(CFLAGS) $@.c -o $@

.PHONY: .clean
clean:
	rm -rf $(targets)
