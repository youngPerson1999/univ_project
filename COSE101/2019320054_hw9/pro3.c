#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int main(void) {
	FILE *input1, *input2, *result;
	input1 = fopen("input1.txt", "r");
	input2 = fopen("input2.txt", "r");
	result = fopen("result.txt", "w");
	
	int mat1[2][3] = { 0 };
	int mat2[2][3] = { 0 };
	int res[2][3] = { 0 };
	
	for (int i = 0; i<3; i++) {
		for (int j = 0; j < 3; j++) {
			fscanf(input1, "%d", &mat1[i][j]);
			fscanf(input2, "%d", &mat2[i][j]);
		}
	}
	
	printf("input1 matrix:\n");
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d\t",mat1[i][j]);
		}
		printf("\n");
	}

	printf("input2 matrix:\n");
	for (int i = 0; i < 2; i++) {
		printf("\n");
		for (int j = 0; j < 3; j++) {
			printf("%d\t",mat2[i][j]);
		}
	}

	printf("\nresult matrix:\n");
	for (int i = 0; i < 2; i++) {
		printf("\n");
		for (int j = 0; j < 3; j++) {
			res[i][j] = mat1[i][j] + mat2[i][j];
			fprintf(result,"%d\t",res[i][j]);
			printf("%d\t", res[i][j]);
		}
		fprintf(result,"\n");
	}
	fclose(input1);
	fclose(input2);
	fclose(result);
	return 0;
}
