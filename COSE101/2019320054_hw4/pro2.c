#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int flip();

int main(void) {
	int i,j;
	int a = 0, b = 0;
	srand(time(NULL));
	for (i = 1; i <= 100; i++) {
		if (flip() == 0) a++;
		else b++;
		if(i%10 ==0)printf("\n");
	}
	printf("\nThe total number of Heads was %d", a);
	printf("\nThe total number of Tails was %d", b);
	return 0;
}
int flip() {
	int a = rand() % 2;
	if (a == 0) printf("Tails ");
	if (a == 1) printf("Heads ");
	return a;
}
