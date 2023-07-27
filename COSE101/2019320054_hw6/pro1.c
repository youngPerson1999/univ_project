#include<stdio.h>

void fun(int *ap, int *bp, int *cp);

int main(void) {
	int a, b, c;
	int *ap, *bp, *cp;
	printf("Enter three numbers: ");
	scanf_s("%d %d %d", &a, &b, &c);
	ap = &a;
	bp = &b;
	cp = &c;
	printf("Before: a = %d, b = %d, c = %d", a, b, c);
	fun(&a, &b, &c);
	printf("\nAfter: a = %d, b= %d, c = %d",a,b,c);
	return 0;
}
void fun(int *ap, int *bp, int*cp) {
	int temp = 0;
	++*ap;
	temp = *bp**cp;
	*bp = *cp;
	*cp = temp;
}
