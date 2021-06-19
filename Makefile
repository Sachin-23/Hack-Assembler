build: asm.cpp symbol.h parser.h code.h
	g++ -Wall -ggdb -O0 asm.cpp -o asm

clean:
	rm asm
