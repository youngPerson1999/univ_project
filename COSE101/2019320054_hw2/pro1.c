#include <stdio.h>

int main(void)
{
	int a, b, sum;
	int count;
	printf("Enter two intergers : ");
	scanf_s("%d %d", &a, &b);

	sum = 0;

	if (a >= b)
	{
		count = b;
		while (count <= a)
		{
			sum += count;
			count++;
		}

	}
	else
	{
		count = a;
		while (count <= b)
		{
			sum += count;
			count++;
		}
	}
	printf("The sum of all intergers between %d and %d is %d \n", a, b, sum);

	return 0;
		
}
