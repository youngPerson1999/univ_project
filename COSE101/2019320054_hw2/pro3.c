#include<stdio.h>

int main(void)
{
	int integer, copy;
	int count = 0;
	printf("Enter a 5-digit number: ");
	scanf_s("%d", &integer);
	copy = integer;
	if (integer < 0) return 1;
	if (integer > 99999) return 1;


	while (integer > 0)
	{
		if (integer % 10 == 7)
		{
			count++;
		}
		integer /= 10;
	}

	printf("The number %d has %d seven(s) in it", copy, count);

	return 0;
}

