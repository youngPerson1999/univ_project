#include<stdio.h>

int add(int, int);
int sub(int, int);
int mul(int, int);

int main(void) {
	int(*func[3])(int, int) = {add, sub, mul};
	int i=-1,a,b;
	do {
		printf("0.Addition\n1.Subtraction\n2.Multiplication\n3.End\n");
		while (i < 0 || 3 < i) {
			printf("select the operation: ");
			while (0 == scanf_s("%d", &i)) {
				printf("select the operation number: ");
				getchar();
			}
		}
		if (i == 3) break;
		printf("Enter the two numbers: ");
		scanf_s("%d %d", &a, &b);
		if (i == 0) printf("\n\t%d + %d = %d\n\n", a, b, func[0](a, b));
		else if(i==1) printf("\n\t%d - %d = %d\n\n", a, b, func[1](a, b));
		else if(i==2) printf("\n\t%d * %d = %d\n\n", a, b, func[2](a, b));
		i = -1;
	} while (1);

	return 0;
}
int add(int a, int b) {
	return a + b;
}
int sub(int a, int b) {
	return a - b;
}
int mul(int a, int b) {
	return a * b;
}
