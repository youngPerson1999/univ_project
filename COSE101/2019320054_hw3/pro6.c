#include<stdio.h>

int main(void) {
	int num,copy, i;
	i = 1;
	printf("Input the value of range edge : ");
	
	scanf_s("%d", &num);
	
	
	if (num >= 50 && num <= 100)
	{
		printf("The Roman numeral equivalence Table from 1 to %d\n", num);
		printf("------------------------------------------------\n");
		printf("Demical Numbers Roman Numerals\n");
		printf("------------------------------------------------\n");
		do {
			printf("%d\t\t", i);
			copy = i;
			if (i == 100) {
				printf("C");
				copy %= 100;
			}
			if (copy >= 90 ) {
				printf("XC");
				copy %= 90;
			}
			if (copy >= 50) {
				printf("L");
				copy %= 50;
			}
			if (copy >= 40) {
				printf("XL");
				copy %= 40;
			}
			while (copy >= 10) {
				printf("X");
				copy -= 10;
			}
			if (copy >= 9) {
				printf("IX");
				copy %= 9;
			}
			if (copy >= 5) {
				printf("V");
				copy %= 5;
			}
			if (copy >= 4) {
				printf("IV");
				copy %= 4;
			}
			while (copy >= 1) {
				printf("I");
				copy--;
			}
			i++;
			if(i<=num) printf("\n");
		} while (i <= num);
			
	}
	else printf("Wrong Input! range edge should be from 50 ~ 100");
	return 0;
}
