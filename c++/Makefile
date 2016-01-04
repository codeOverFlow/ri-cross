.PHONY: clean
.SUFFIXES:

all: clean
	g++ -std=c++11 -O2 appax.cpp -o appax
	g++ -std=c++11 -O2 -pthread ranking.cpp -o ranking
	g++ -std=c++11 -O2 indexinverse.cpp -o indexinverse

clean:
	rm -fr appax
	rm -fr ranking
	rm -fr indexinverse
