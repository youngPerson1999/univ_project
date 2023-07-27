#include <stdio.h>

int main(void)
{
	int a;
	int num = 0;
	
	printf("Decimal\t\tBinary\n");
	for (a = 0; a <= 15; a++)
	{
		printf("%d\t\t", a);
		printf("%c", num < 16 && num >= 8 ? '1' : '0');
		printf("%c", num%8 >= 4 ? '1' : '0');
		printf("%c", num%4 >= 2 ? '1' : '0');
		printf("%c\t", num%2 == 1 ? '1' : '0');
		if(a<15) printf("\n");
		num++;
	}
	return 0;
}
