CC=gcc
CFLAGS=-Wall -Wextra -lcrypto -O3

all: encrypt guessword

setup:
	./setup.sh

%: %.c setup
	$(CC) $(CFLAGS) -o $@ $<
