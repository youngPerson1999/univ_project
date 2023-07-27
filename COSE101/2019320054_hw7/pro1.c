#include<stdio.h>
#include<stdlib.h>

int main(void) {
	char arr[10] = { 0 };
	int sum = 0, d = 0, i = 0;
	while (i<4) {
		printf("Enter an integer string: ");
		scanf_s("%s",&arr,10);
		d = atoi(arr);
		if (d > -9999 && d < 99999) {
			sum += d;
			i++;
		}
	}
	printf("\nThe total of the values is %d", sum);
	return 0;
}
