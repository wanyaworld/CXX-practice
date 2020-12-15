a.out: main.o myClass.o
	g++ -o a.out main.o myClass.o

main.o: myClass.h main.cc
	g++ -c -o main.o main.cc

myClass.o: myClass.h myClass.cc
	g++ -c -o myClass.o myClass.cc

clean:
	rm *.o a.out
