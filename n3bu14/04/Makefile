all: attack-this

include ../Makefile.inc
user=level04
flag=flag04

attack-this:
	$(MAKE) attack | awk -F' ' '/Token: [[:alnum:]]*/ { print $$2 }' > token.txt
	expect ../expect.xp ${flag} ${ip}
	rm token.txt
