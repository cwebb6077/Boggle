all:
	gcc -Wall -std=c11 -ggdb3 main.c board.c trie.c -o boggle

valgrind:
	gcc -Wall -std=c11 -ggdb3 main.c board.c trie.c -o boggle
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --main-stacksize=6890190000 -v ./boggle
	
textfile:
	grep -v "^[A-Z]" /usr/share/dict/words | grep -v "'" | grep -v "é" | grep -v "Å"  > dict.txt

clean:
	rm boggle
	rm *.txt
