#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int main(void) {
	FILE*inform;
	char name[100], major[100];
	int std_id = 0, grade = 0, stdnum = 0;

	inform = fopen("student_file.txt", "w+");

	for (int i = 0; i < 5; i++) {
		printf("Name Student_number Grade Major: ");
		fscanf(stdin,"%s %d %d %s", name, &std_id, &grade, major);
		fprintf(inform, "%s %d %d %s\n", name, std_id, grade, major);
	}
	
	printf("Insert the student number to find:");
	scanf("%d", &stdnum);

	rewind(inform);
	while (!feof(inform)) {
		fscanf(inform, "%s %d %d %s", name, &std_id, &grade, major);
		if (stdnum == std_id) {
			printf("The major of student with ID %d is %s", stdnum, major);
			break;
		}
	}

	fclose(inform);
	
	return 0;
}
