compare: main.o libalt.so
	g++ -o compare main.o -L. -lalt -Wl,-rpath,.

main.o: main.cpp
	g++ -c main.cpp

libalt.so: compare.o
	g++ -shared -Wall -o libalt.so compare.o

compare.o: compare.cpp
	g++ -g -c -fPIC -Wall compare.cpp

clean:
	rm -f *.o *.so
