#include<stdio.h>

int fun(const char*str1,const char*str2);

int main(void) {
	char string1[80], string2[80];
	printf("Enter two strings: ");
	scanf_s("%s", &string1,80);
	scanf_s("%s", &string2, 80);
	printf("%s and %s are %s", string1, string2,fun(string1,string2)?"equal":"not equal");
	return 0;

}
int fun(const char*str1, const char*str2) {
	while (*str1 != '\0'|| *str2 != '\0') {
		if (*str1 != *str2) return 0;
		str1++;
		str2++;
	}
	return 1;
}
