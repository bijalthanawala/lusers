CPPFLAGS=-std=c++11 -Wall -pedantic

CXX=clang++
GPP=g++

lusers: lusers.cpp
	$(GPP) $(CPPFLAGS) -o lusers lusers.cpp

clean:
	rm lusers

