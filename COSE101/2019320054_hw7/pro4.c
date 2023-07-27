#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

int bubblesort(char a[][50]);

int main(void) {
	char name[10][50];
	for (int i = 0; i < 10; i++){
		printf("Enter a string: ");
		scanf_s(" %49s", name[i],50);
	}
	bubblesort(name);
	printf("The strings in sorted order are:\n");
	for (int i = 0; i < 10; i++)
		printf("%s\n", name[i]);
	return 0;
}
int bubblesort(char a[][50]) {
	char tmp[50] = { 0 };
	for (int i = 0; i < 10; i++) {
		for (int j = i+1; j < 10; j++) {
			if (strcmp(a[i], a[j]) >= 0) {
				strcpy(tmp, a[i]);
				strcpy(a[i], a[j]);
				strcpy(a[j], tmp);
			}
		}
	}

}
