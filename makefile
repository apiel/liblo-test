all:
	g++ -o client client.cpp -llo -lpthread && ./client