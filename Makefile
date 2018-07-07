SICXE.out : main.o run_cmd.o shell_cmd.o history.o util.o memory.o opcode.o assemble.o loader.o
	gcc -Wall -o SICXE.out main.o run_cmd.o shell_cmd.o history.o util.o memory.o opcode.o assemble.o loader.o

main.o : main.c
	gcc -Wall -c -o main.o main.c

run_cmd.o : run_cmd.c
	gcc -Wall -c -o run_cmd.o run_cmd.c

shell_cmd.o : shell_cmd.c
	gcc -Wall -c -o shell_cmd.o shell_cmd.c

history.o : history.c
	gcc -Wall -c -o history.o history.c

util.o : util.c
	gcc -Wall -c -o util.o util.c

memory.o : memory.c
	gcc -Wall -c -o memory.o memory.c

opcode.o : opcode.c
	gcc -Wall -c -o opcode.o opcode.c

assemble.o : assemble.c
	gcc -Wall -c -o assemble.o assemble.c

loader.o : loader.c
	gcc -Wall -c -o loader.o loader.c

clean:
	rm *.o SICXE.out

cleano:
	rm *.o
