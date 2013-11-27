all: p1 p2 p3 p4 p5
	gcc prob1.c -o prob1.exe
p1: prob1.c
	gcc -S -fverbose-asm prob1.c -o assem.S
p2: prob2.S
	gcc prob2.S -nostdlib -o prob2.exe
p3: prob3.S
	gcc prob3.S -nostdlib -o prob3.exe
p4: prob4.S
	gcc prob4.S -nostdlib -o prob4.exe
p5: prob5.c
	gcc prob5.c -o prob5.exe -lrt -O0
clean:
	rm *.o *.exe
