#include<stdio.h>

int main(void)
{
	int i,j,k;
	for (i = 0 ; i < 6 ; i++)
	{
		if (i > 0) {
			for (k = 5; k > i; k--)
				printf(" ");
		}
		for (j = 1; j < (i * 2); j++)
			printf("*");
		 if(i > 0) printf("\n");
	}
	for (i = 0 ; i < 4 ; i++)
	{
		for (k = 0; k < i+1; k++)
			printf(" ");
		for (j = 7; j > (i * 2); j--)
			printf("*");
		if(i < 3) printf("\n");
	}

	return 0;
}
