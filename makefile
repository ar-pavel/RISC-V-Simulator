all: 
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o main.o main.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o rv32i.o rv32i.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o memory.o memory.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o registerfile.o registerfile.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o hex.o hex.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -o rv32i main.o rv32i.o memory.o registerfile.o hex.o
	
clean:
	rm -f *.o rv32i