all: Sem

Sem: source.cpp
	g++ --std=c++20 -o $@ -lpthread $<
