
proj03: proj03.student.o
	g++ -o proj03 proj03.student.o
proj03.student.o: proj03.student.c
	g++ -c -Wall proj03.student.c


