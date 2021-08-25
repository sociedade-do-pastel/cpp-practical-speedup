all:
	g++ Main.cpp -o Primos -pthread
debug:
	g++ -g Main.cpp -o Primos -pthread
