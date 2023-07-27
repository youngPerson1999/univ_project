#include <stdio.h>

void tower(int c, int first, int end, int temp);

int main(void) {
	int i;

	printf("Enter the starting number of disks: ");
	scanf_s("%d", &i);

	tower(i, 1, 3, 2);

	return 0;
}
void tower(int c, int first, int end, int temp) {
	if (c == 1)
		printf("%d --> %d\n", first, end);
	else {
		tower(c - 1, first, temp, end);

		tower(1, first, end, temp);

		tower(c - 1, temp, end, first);
	}
}