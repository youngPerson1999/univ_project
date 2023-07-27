#include <stdio.h>

int main(void)
{
	int a,b,c,d,e,f;
	int sum = 0;
	printf("Input : ");
	
	scanf_s("%d", &a);
	
	b = a / 10000;
	c = (a - b * 10000) / 1000;
	d = (a - b * 10000 - c * 1000) / 100;
	e = (a % 100) / 10;
	f = a % 10;
	printf("Output : \n");

	printf("%d   %d   %d   %d   %d\n", f, e, d, c, b);
	if (b % 2 == 1) sum = sum + b;
	if (c % 2 == 1) sum = sum + c;
	if (d % 2 == 1) sum = sum + d;
	if (e % 2 == 1) sum = sum + e;
	if (f % 2 == 1) sum = sum + f;
	printf("%d\n", sum);

	return 0;
}