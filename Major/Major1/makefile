major1: major1.o palindrome.o power.o reverse.o replace.o
	gcc -Wall -o major1 major1.o palindrome.o power.o reverse.o replace.o

major1.o: major1.c
	gcc -c major1.c

palindrome.o: palindrome.c
	gcc -c palindrome.c

power.o: power.c
	gcc -c power.c

reverse.o: reverse.c
	gcc -c reverse.c
	
replace.o: replace.c
	gcc -c replace.c
	
run:
	./major1

clean:
	rm *.o
	rm major1
