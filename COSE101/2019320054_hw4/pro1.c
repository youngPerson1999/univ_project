#include<stdio.h>

int ulcase(int a);

int main(void) {
	char a;
	int b;
	printf("Enter the Character : ");
	
	scanf_s("%c", &a);

	b = ulcase(a);
	if (b == 2) printf("%c: Uppercase", a);
	if (b == 1)printf("%c: Lowercase", a);
	if(b==3) printf("INPUT IS NOT ALPHABET");
	return 0;
}
int ulcase(int a) {
	if (a >= 65 && a <= 90) return 2;
	if (a >= 97 && a <= 122) return 1;
	else return 3;

}
