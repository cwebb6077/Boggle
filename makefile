all:
	gcc -Wall -std=c11 main.c -o boggle

valgrind:
	gcc -Wall -std=c11 -ggdb3 main.c -o boggle
	valgrind --leak-check=full -v ./boggle
	
clean:
	rm boggle