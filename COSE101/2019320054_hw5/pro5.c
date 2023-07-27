#include<stdio.h>

int Sorting(int a[]);

int main(void){
	int list[5][5];
	int average[5];
	for (int i = 0; i < 5; i++) {
		printf("INDEX %d\n", i);
		printf("Enter 5 Quiz scores : \n");
		average[i] = 0;
		for (int j = 0; j < 5; j++) {
			scanf_s("%d", &list[i][j]);
			average[i] += list[i][j];
		}
		average[i] /= 5;
		printf("\n");
	}
	printf("From Highest Scores to Lowest :\n");
	Sorting(average);
	return 0;
}
int Sorting(int a[]) {
	int tmp;
	while (1) {
		for (int i = 0; i < 5; i++) {
			if (a[i] <= a[i + 1]) {
				tmp = a[i + 1];
				a[i + 1] = a[i];
				a[i] = tmp;
			}
		}
		if (a[0] >= a[1] && a[1] >= a[2] && a[2] >= a[3] && a[3] >= a[4]) break;
	}
	for (int i = 0; i < 5; i++)
		printf("%d ", a[i]);
}
