#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void encryptor(char word[][10], char cipher[]);
int decryptor(char word[][10], char cipher[]);

int main(void) {
	srand(time(NULL));
	char word[10][10] = { "craftbeer","metallica","whitewine","champagne","bourgogne","sandiego","lafayette","pinotnoir","sauvignon","dondiablo" };
	char ciphertext[10];

	encryptor(word, ciphertext);	

	decryptor(word, ciphertext);

	return 0;
}
void encryptor(char word[][10], char ciphertext[]) {

	int rnd = rand() % 10;
	int key = rand() % 19 + 1;
	
	printf("\t- Encryptor -\n");
	printf("Selected word: %s\n", word[rnd]);
	printf("Selected value of the Key: %d\n", key);
	
	for (int j = 0; j < 10; j++) {
		ciphertext[j] = word[rnd][j] + key;
	}
	ciphertext[9] = '\0';
	printf("Ciphertext: %s\n", ciphertext);
}
int decryptor(char word[][10], char cipher[]) {
	char tmp[10];

	printf("\t- Decode -\n");
	printf("Ciphertext input: %s\n", cipher);

	for (int i = 1; i<20; i++) {
		strcpy(tmp, cipher);

		printf("\n> Guess %d <\n", i);
		printf(" Key: %d\n", i);
		for (int j = 0; j < 10; j++)
			tmp[j] -= i;
		tmp[9] = '\0';
		printf(" Plaintext: %s\n", tmp);

		for (int k = 0; k < 10; k++) {
			if (strcmp(tmp, word[k]) == 0) {
				printf("Found the correct answer!\n\n");
				printf("result: '%d' was selected as the key\n", i);
				printf("\t'%s' was the Plaintext.", tmp);
				return 0;
			}
		}
		printf("Matching word not found\n");
	}
	return 0;
}