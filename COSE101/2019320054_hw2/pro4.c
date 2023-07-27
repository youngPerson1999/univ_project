#include<stdio.h>

int main(void)
{
	int a, b;
	int ch;
	printf("Input two integers: ");
	scanf_s("%d %d", &a, &b);
	if (a % b == 0) {
		ch = a / b;
		printf(" %d is a multiple of %d by a factor of %d", a, b, ch);
	}
	else printf("%d is not a multiple of %d", a, b);

	return 0;
}
