#include <stdio.h>

void i_to_a(int num);

int main(void) {
	int n;
	while (1) {
		printf("Enter the inputs : ");
		scanf_s("%d", &n);
		if (n >= 100000 && n <= 999999) break;
		else printf("Input Error!\n");
	}

	i_to_a(n);
	printf("\n");

	return 0;
}
void i_to_a(int num) {
	char ask;
	if (num >= 10) {
		i_to_a(num / 100);
		ask = (num % 100) % 26 + 'A';
		printf("%c ", ask);
	}
}
