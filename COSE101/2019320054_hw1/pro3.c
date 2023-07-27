#include <stdio.h>

int main(void)
{
	int a, b, c, d, e;
	
	printf("Input 5 integers : ");
	scanf_s("%d %d %d %d %d", &a, &b, &c, &d, &e);

	int max, min;

	if (b>=a)
	{
		max = b;
		if (c >= b) {
			max = c;
			if (d >= c) {
				max = d;
				if (e >= d) 	max = e;
			}
		}
	}
	if(a>=b)
	{
		max = a;
		if (c >= a)
		{
			max = a;
			if(d>=c)
			{
				max = d;
				if (e >= d) max = e;
			}
		}

	}

	if (a >= b)
	{
		min = b;
		if (b >= c)
		{
			min = c;
			if (c >= d)
			{
				min = d;
				if (d >= e) min = e;
			}
		}
	}
	if(b>=a)
	{
		min = a;
		if (a >= c)
		{
			min = c;
			if (c >= d)
			{
				min = d;
				if (d >= e) min = e;
			}
		}
	}
	printf("The largest value is %d \n", max);
	printf("The smallest value is %d \n", min);
	return 0;

}