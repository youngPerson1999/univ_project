#include <stdio.h>

int main(void)
{
	int a, max;
	int i = 1;
	printf("Enter the number: ");
	scanf_s("%d", &a);
	max = a;
	do {
		printf("Enter the number: ");
		scanf_s("%d", &a);
		if (a >= max) max = a;
	} while (++i < 5);

	printf("Largest is %d", max);

	return 0;
}
