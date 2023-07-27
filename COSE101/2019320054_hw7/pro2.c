#include<stdio.h>
#include<ctype.h>
#include<string.h>

int main(void) {
	char str[3][80];
	char*string;
	int count;

	printf("Enter three lines of text:\n");
	fgets(str[0], 80, stdin);
	fgets(str[1], 80, stdin);
	fgets(str[2], 80, stdin);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; str[i][j]; j++)
			if (str[i][j] >= 'A'&&str[i][j] <= 'Z') str[i][j] = tolower(str[i][j]);
	}

	printf("\nThe total occurrences of each character:\n");

	for (int i = 'a'; i <= 'z'; i++) {
		count = 0;
		for (int j = 0; j < 3; j++) {
			string = &str[j];
			string--;
			do {
				string = strchr(string + 1, i);
				if (string != NULL) count++;
			} while (string != 0);
		}
		printf("%c: %3d\n", i, count);
	}
	return 0;
}