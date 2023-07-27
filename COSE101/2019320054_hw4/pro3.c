#include <stdio.h>

int gcd(int num1, int num2);

int main(void) {

	int a, b, c;
	while(1){
		printf("Enter two positive integers: ");
		scanf_s("%d %d", &a, &b);
		if (a <= 0 || b <= 0) printf("Wrong Input, Try again!\n\n");
		else break;
	}
	printf("GCD of %d and %d is %d", a, b, gcd(a,b));
	return 0;
}
int gcd(int num1, int num2) {
	if (num2 > num1) return gcd(num2, num1);
	if (num1 % num2 == 0) return num2;
	else return gcd(num2, num1%num2);
}
