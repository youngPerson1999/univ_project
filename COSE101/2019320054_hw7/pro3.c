#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

int main(void) {
	char str[3][80];
	char*ptr;
	int count[80] = { 0 };
	int length;

	printf("Enter three lines of text:\n");
	for (int i = 0; i < 3; i++) 
		fgets(str[i],80,stdin);
	
	for (int i = 0; i < 3; i++) {
		ptr = strtok(str[i],". \n");
		while (ptr != NULL) {
			length = strlen(ptr);
			count[length] += 1;
			ptr = strtok(NULL, ". \n");
		}
	}

	printf("\n");

	for (int i = 0; i<80; i++) {
		if (count[i] == 1)printf("%2d word of length %2d\n", count[i], i);
		if (count[i] > 1)printf("%2d words of length %2d\n", count[i], i);
	}


	return 0;
}
