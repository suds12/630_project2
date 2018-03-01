
all: main output

main: main.cpp
	g++ main.cpp -o output.out -std=c++11

output: output.out
	./output.out 
