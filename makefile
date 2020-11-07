all: 
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o main.o main.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o rv32i.o rv32i.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o memory.o memory.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o registerfile.o registerfile.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o hex.o hex.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -o rv32i main.o rv32i.o memory.o registerfile.o hex.o

	./rv32i  -dirz -m100 allinsns5.bin  > allinsns5-dirz-m100.log
	./rv32i  -dz -m100 allinsns5.bin  > allinsns5-dz-m100.log
	./rv32i  -i -m100 allinsns5.bin  > allinsns5-i-m100.log
	./rv32i  -dirz -m8500 torture5.bin  > torture5-dirz-m8500.log
	./rv32i  -ir -m8500 torture5.bin  > torture5-iz-m8500.log
clean:
	rm -f *.o rv32i