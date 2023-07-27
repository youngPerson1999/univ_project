#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void Reverse(int*arr);

int main(void) {
	srand(time(NULL));
	int arr[10];
	for (int i = 0; i < 10; i++) {
		arr[i] = rand() % 100;
	}
	Reverse(&arr);

	return 0;
}
void Reverse(int*arr) {
	for (int i = 0; i < 10; i++) {
		printf("%2d ",*arr);
		arr ++;
	}
	printf("\n");
	for (int i = 10; i > 0; i--) {
		arr--;
		printf("%2d ", *arr);
	}
}
