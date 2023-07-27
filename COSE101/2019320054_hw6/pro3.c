#include<stdio.h>

void fun(char*str1, char*str2);

int main(void) {
	char string1[80], string2[80];
	printf("Enter two strings: ");
	scanf_s("%s", &string1, 80);
	scanf_s("%s", &string2, 80);
	fun(string1, string2);
	printf("%s", string1);

	return 0;
}
void fun( char*str1, char*str2) {
	while (*str1)
		str1++;
		while (*str2) {
			*str1 = *str2;
			str1++;
			str2++;
		}
		*str1 = 0;

}
