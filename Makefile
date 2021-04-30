all:
	g++ -fsanitize=address -g main.cc
clean:
	rm a.out
