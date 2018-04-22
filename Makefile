debug: diffcat.c Makefile
	cc -O2 diffcat.c -o diffcat -DDEBUG

diffcat: diffcat.c Makefile
	cc -O2 diffcat.c -o diffcat

clean:
	-rm -f diffcat
