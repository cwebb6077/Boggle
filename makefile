all:
	gcc -Wall -std=c11 -ggdb3 main.c board.c trie.c -lncurses -o boggle

valgrind:
	grep -v "^[A-Z]" /usr/share/dict/words | grep -v "'" | grep -v "é" | grep -v "Å" | grep -v "^[b-z]"  > dict.txt
	gcc -Wall -std=c11 -ggdb3 main.c board.c trie.c -lncurses -o boggle
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --main-stacksize=6890190000 -v ./boggle
	
textfile:
	grep -v "^[A-Z]" /usr/share/dict/words | grep -v "'" | grep -v "é" | grep -v "Å" > dict.txt

clean:
	rm boggle
	rm *.txt
