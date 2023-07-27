#include<stdio.h>

int main(void)
{
	int n;
	int fact = 1;
	int number=-1;
	
	while (number < 0)
	{
		printf("Enter positive number: ");
		scanf_s("%d", &number);
		}
	n = number;

	while (n >= 0) {
			if (n > 0) {
				fact *= n;
			}
			--n;
		}
	if (number == 0) fact = 1;
	printf("%d! is %d\n", number, fact);

	return 0;

}
