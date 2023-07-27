#include<stdio.h>

long long factorial(int num);
long long combination(int num1, int num2);

int main(void) {

	int n, r;
	while (1) {
		printf("Enter the inputs : ");
		scanf_s("%d %d", &n, &r);
		if (n >= 5 && n <= 20 && r >= 5 && r <= 20 && n >= r) break;
		else printf("Input Error!\n\n");
	}
	printf("nCr : %lld", combination(n, r));
	return 0;
}
long long factorial(int num) {

	if (num >= 1) {
		return num * factorial(num - 1);
	}
	else return 1;
}
long long combination(int num1, int num2) {
	return factorial(num1) / (factorial(num2)*factorial(num1 - num2));
}
