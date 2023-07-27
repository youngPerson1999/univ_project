#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(void) {
	int i, j;
	int mat1[5][6];
	int mat2[5][6];
	int result[5][6];
	srand(time(NULL));
	printf("matrix1: \n");
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			mat1[i][j] = rand() % 101;
			printf("%d\t", mat1[i][j]);
		}
		printf("\n");
	}
	printf("\nmatrix2: \n");
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			mat2[i][j] = rand() % 101;
			printf("%d\t", mat2[i][j]);
		}
		printf("\n");
	}
	printf("\nresult: \n");
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			result[i][j] = mat1[i][j] + mat2[i][j];
			printf("%d\t", result[i][j]);
		}
		printf("\n");
	}

	return 0;
}
