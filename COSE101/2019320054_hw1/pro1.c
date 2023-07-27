#include <stdio.h>

int main(void) {

	int a;
	printf("Input an integer : ");
	scanf_s("%d", &a);

	if(a%2 == 1)
	{
		printf("%d is an odd integer", a);

	}
	
	if(a%2 == 0)
	{
		printf("%d is an even integer", a);
	}

	return 0;
}