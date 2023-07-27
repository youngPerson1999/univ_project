#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct listNode {
	char input[50];
	struct listNode *nextPtr;
};

typedef struct listNode ListNode;
typedef ListNode *ListNodePtr;

void insert(ListNodePtr *sPtr, char name[]);
void deletenode(ListNodePtr *sPtr);
void mergelist(ListNodePtr sPtr, ListNodePtr *startPtr);
int isEmpty(ListNodePtr sPtr);
void printlist(ListNodePtr currentPtr);
void infilelist(ListNodePtr currentPtr);

FILE *output;


int main(void) {
	ListNodePtr startPtr1 = NULL;
	ListNodePtr startPtr2 = NULL;
	ListNodePtr startPtr3 = NULL;
	FILE *input1, *input2;

	char name[50];
	
	input1 = fopen("input1.txt", "r");
	input2 = fopen("input2.txt", "r");
	
	
	if (input1 == NULL) printf("File is not existed\n");
	else {
		printf("Input linked list 1: ");
		while (!feof(input1)) {
			fscanf(input1,"%s", name);
			insert(&startPtr1, name);
		}
		printlist(startPtr1);
	}
	printf("\n\n");

	if (input2 == NULL) printf("File is not existed\n");
	else {
		printf("Input linked list 2: ");
		while (!feof(input2)) {
			fscanf(input2, "%s", name);
			insert(&startPtr2, name);
		}
		printlist(startPtr2);
	}
	printf("\n\n");

	rewind(input1);	rewind(input2);

	printf("Result linked list: ");
	mergelist(startPtr1, &startPtr3);
	mergelist(startPtr2, &startPtr3);
	deletenode(&startPtr3);
	infilelist(startPtr3);
	printlist(startPtr3);

	fclose(input1);
	fclose(input2);
	return 0;
}
void mergelist(ListNodePtr sPtr, ListNodePtr *startPtr3) {

	ListNodePtr newPtr,curPtr,prePtr;
	ListNodePtr prevPtr, currPtr, tempPtr;
	
	char copy[20];

	while (sPtr != NULL) {
		strcpy(copy, sPtr->input);

		newPtr = malloc(sizeof(ListNode));

		if (newPtr != NULL) {
			strcpy(newPtr->input, copy);
			newPtr->nextPtr = NULL;
			prePtr = NULL;
			curPtr = *startPtr3;

			while (curPtr != NULL && strcmp(copy, curPtr->input) > 0) {
				prePtr = curPtr;
				curPtr = curPtr->nextPtr;
			}
			if (prePtr == NULL) {
				newPtr->nextPtr = *startPtr3;
				*startPtr3 = newPtr;
			}
			else {
				prePtr->nextPtr = newPtr;
				newPtr->nextPtr = curPtr;
			}
		}
		else printf("Name is not inserted\n");

		sPtr = sPtr->nextPtr;
	}
}
void insert(ListNodePtr *sPtr, char name[]) {
	ListNodePtr newPtr, prevPtr, currentPtr;

	newPtr = malloc(sizeof(ListNode));


	if (newPtr != NULL) {
		
		strcpy(newPtr->input, name);
		newPtr->nextPtr = NULL;
		prevPtr = NULL;
		currentPtr = *sPtr;

		while (currentPtr != NULL) {
			
			prevPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
		}
		if (prevPtr == NULL) {
			newPtr->nextPtr = *sPtr;
			*sPtr = newPtr;
		}
		else {
			prevPtr->nextPtr = newPtr;
			newPtr->nextPtr = currentPtr;
		}
	}
	else printf("%s not inserted. No memory available\n", name);
}
void deletenode(ListNodePtr *sPtr) {
	ListNodePtr currPtr = *sPtr, prevPtr = NULL;

		
	while (currPtr != NULL) {
			
		if (currPtr == NULL) return;
		if (prevPtr == NULL) {
		prevPtr = currPtr;
			currPtr = currPtr->nextPtr;
		}
		if (currPtr != NULL && !(strcmp(prevPtr->input, currPtr->input))) {
		currPtr = currPtr->nextPtr;
		prevPtr->nextPtr = currPtr;
		}
		else {
			prevPtr = currPtr;
			currPtr = currPtr->nextPtr;
		}
	}
	
}
int isEmpty(ListNodePtr sPtr) {
	return sPtr == NULL;
}
void printlist(ListNodePtr currentPtr) {
	while (currentPtr != NULL) {
		printf("%s", currentPtr->input);
		currentPtr = currentPtr->nextPtr;
		if (currentPtr != NULL) printf(" -> ");
	}
}
void infilelist(ListNodePtr currentPtr) {
	output = fopen("output.txt", "w+");
	while (currentPtr != NULL) {
		fprintf(output, "%s\n", currentPtr->input);
		currentPtr = currentPtr->nextPtr;
	}
	fclose(output);
}
