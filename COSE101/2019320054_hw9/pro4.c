#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

int main(void) {
	FILE *test;
	char name[30], word[30], buffer[200], nul;
	int count = 0, total = 1;

	while (1) {
		printf("Enter the file name and word : ");
		scanf("%s %s", name, word);
		test = fopen(name, "r");
		if (test == NULL) {
			printf("File %s read error\n", name);
			continue;
		}
		else {
			printf("The finding ratio : ");
			while (!feof(test)) {
				fgets(buffer, 200, test);
				for (int i = 0; i < strlen(buffer); i++) {
					nul = buffer[i];
					if (nul == ' ' || nul == '\n') total++;
				}
				if (strstr(buffer, word) != NULL) count++;
			}
			printf("%d/%d", count,total);
			break;
		}
	}

	fclose(test);

	return 0;
}