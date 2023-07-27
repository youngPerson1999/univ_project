#include<stdio.h>

void stringReverse(char strArray[]);

int main(void) {

	char strArray[30] = "Print this string baskward.";
	printf("%s\n", strArray);
	stringReverse(strArray);
	
	return 0;
}
void stringReverse(char strArray[]) {
	if (strArray[0] != '\0') {
		stringReverse(&strArray[1]);
		printf("%c", strArray[0]);
	}
}
