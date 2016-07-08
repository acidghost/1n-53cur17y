all: attack04

include ../Makefile.inc
user=level04

attack04:
	$(MAKE) attack | awk -F' ' '/Token: [[:alnum:]]*/ { print $$2 }' > token.txt
	expect expect.xp
	rm token.txt
