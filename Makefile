all: diffcat

diffcat: diffcat.c Makefile
	cc -O2 diffcat.c -o diffcat

clean:
	-rm -f diffcat
