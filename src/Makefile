all:
	nasm -f elf ext_ln.asm
	gcc -g -Wall log.c ext_ln.o -lm -m32

clean:
	rm *.o asmln log
