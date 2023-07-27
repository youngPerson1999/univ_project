#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int main(void) {
	FILE*read;
	FILE*write;

	char file[80];
	char buffer;
	int count = 0;

	while(1) {
		printf("Enter the name of file to read:");
		scanf("%s", file);
		read = fopen(file, "r");
		if (read == NULL) {
			printf("File %s read error\n", file);
			continue;
		}
		else break;
	}
	printf("Enter the name of file to write:");
	scanf("%s", file);
	write = fopen(file,"w");

	while (!feof(read)) {
		buffer = fgetc(read);
		if (buffer == '\n') count++;
		if (count == 2) {
			count = 0;
			continue;
		}
		fputc(buffer, write);
	}

	fclose(read);
	fclose(write);
	printf("Copy txt complete!");

	return 0;
}
