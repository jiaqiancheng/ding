.PHONY:clean
testcpp:testcpp.o
	gcc testcpp.o -lstdc++ -o testcpp

testcpp.o:test1.cpp
	gcc -c test1.cpp -o testcpp.o

clean:
	rm testcpp testcpp.o
