#include <stdio.h>

int main(void)
{
	int a,b;
	int count = 1;
	while (count <= 4)
	{
		printf("Enter Employee %d's sales in dollars: ", count);
		scanf_s("%d", &a);
		if (a >= 3000) {
			b = 200 + a * 9 / 100;
			printf("Employee %d's salary is : %d\n\n", count, b);
		}
		else
			printf("Employee %d's salary is : %d\n\n", count, 200);

		count ++;
	}

	return 0;
}
