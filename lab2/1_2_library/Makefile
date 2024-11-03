Cal.out: testCal.o calculate.o
	gcc -o Cal.out testCal.o calculate.o

testCal.o: exLib/calculate.h testCal.c
	gcc -c -o testCal.o testCal.c -I./exLib

calculate.o: exLib/calculate.h exLib/calculate.c
	gcc -c -o calculate.o exLib/calculate.c -I./exLib