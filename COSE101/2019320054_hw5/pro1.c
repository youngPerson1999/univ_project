#include <stdio.h>
#include<stdlib.h>
#include<time.h>

int main(void) {
	
	int arr[20];

	srand(time(NULL));

	printf("Nonrepetitve array values are:\n");

	for (int i = 0; i < 20; i++) {
		arr[i] = rand() % 21;
		for (int j = 0; j < i; j++) {
			if (arr[i] == arr[j]) {
				arr[i] = rand() % 21;
				i--;
				break;
			}
		}
	}
	for (int k = 0; k < 20; k++)
		printf("Array[ %d ] = %d\n", k, arr[k]);

	return 0;
}
