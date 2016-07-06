CC=gcc
CFLAGS=-Wall -Wextra -lcrypto -O3

all: encrypt

encrypt: setup encrypt.c
	$(CC) $(CFLAGS) -o encrypt encrypt.c

setup:
	./setup.sh
