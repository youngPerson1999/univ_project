#include<stdio.h>

int main(void)
{
	int base, expo;
	int copy, rst;
	rst = 1;
	do {
		printf("Enter the base number: ");
		scanf_s("%d", &base);
	} while (base < 0);
	do {
		printf("Enter the exponent number: ");
		scanf_s("%d", &expo);
	} while (expo < 0);
	copy = expo;
	while (expo > 0)
	{
		rst *= base;
		expo--;
	}
	if (copy == 0) rst = 1;
	printf("%d^%d is %d", base, copy, rst);
	return 0;
}
