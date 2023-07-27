#include<stdio.h>

int main(void)
{
	int i = 0;
	int b;
	double sum = 0;
	printf("Enter pairs of item numbers and quantities.\n");
	printf("Enter -1 for the item numbers to end input.\n");
	while (1)
	{
		scanf_s("%d", &i);
		if (i == -1) break;
		scanf_s("%d", &b);
		switch (i) {
		case 1:
			sum += b*2.98;
			break;

		case 2:
			sum += b*4.5;
			break;
		case 3:
			sum += b*9.98;
			break;
		case 4:
			sum += b*4.49;
			break;
		case 5:
			sum += b*6.87;
			break;
		default:
			printf("Invalid product code: %d\n", i);
			printf("   \t    Quantity: %d\n", b);
			break;

		}
	
	}
	printf("The total retail value was: %.2lf", sum);
	return 0;
}
