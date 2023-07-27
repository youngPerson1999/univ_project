#include <stdio.h>

int main(void)
{
	int c;
	int b;
	int sum;
	int product;
	int diff;
	int quo, rem;
	printf("Enter two numbers : ");
	scanf_s("%d", &c);
	scanf_s("%d", &b);

	sum = c + b;
	printf("The sum is %d \n", sum);


	product = c * b;
	printf("The product is %d \n", product);


	diff = c - b;
	printf("The difference is %d \n", diff);


	if (c < b)
	{
		quo = b / c;
		printf("The quotient is %d \n", quo);
		rem = b% c;
		printf("The remainder is %d \n", rem);
	}
	if(b<=c)
	{
		quo = c / b;
		printf("The quotient is %d \n", quo);
		rem = c % b;
		printf("The remainder is %d \n", rem);
	}

	return 0;
}