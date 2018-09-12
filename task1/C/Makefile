.PHONY:clean
testc:testc.o
	gcc testc.o -o testc

testc.o:test2.c
	gcc -c test2.c -o testc.o

clean:
	-@rm testc testc.o
