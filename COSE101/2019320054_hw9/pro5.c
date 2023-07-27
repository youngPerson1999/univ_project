#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>

void line();
void fun1(struct employee info);
void fun2(struct employee info);
void fun3(struct employee info);

int count = 0;

struct employee{
	int employee_id;
	char *name;
};


int main(void) {
	struct employee info = { 0 };
	int choice = 0;

	while (1) {
		system("cls");
		printf("\n");
		line();
		printf("MENU:\n");
		line();
		printf("1. Add a record\n");
		printf("2. Display the file\n");
		printf("3. Update the record\n");
		printf("4. Exit\n");
		line();
		while (1) {
			printf("Enter your choice:");
			scanf("%d", &choice);
			if (choice >= 1 || choice <= 4) break;
		}
		if (choice == 1) fun1(info);
		else if (choice == 2) fun2(info);
		else if (choice == 3) fun3(info);
		else if (choice == 4) break;
	}

	return 0;
	}
void line() {
	printf("====================\n");
}
void fun1(struct employee info) {
	FILE *record;
	char namecpy[100];

	info.name = namecpy;

	if (count == 0) {
		count++;
		record = fopen("record.txt", "wb+");
	}
	else  record = fopen("record.txt", "ab+");
	
	printf("ENTER THE EMPLOYEE ID :\n");
	scanf("%d", &info.employee_id);
	getchar();
	fwrite(&info.employee_id, sizeof(int), 1, record);
	printf("ENTER THE EMPLOYEE NAME:\n");
	fgets(info.name, 100,stdin);
	
	fwrite(info.name,sizeof(namecpy), 1, record);
	
	fclose(record);
}
void fun2(struct employee info) {

	FILE *record;
	char namecpy[100];

	line();
	printf("ID\tNAME\n");
	line();
	if (count == 0) printf("NO EMPLOYEE\n");
	else {
		record = fopen("record.txt", "rb");

		while ((fread(&info.employee_id, sizeof(int), 1, record))==1) {
			fread(namecpy, 100, 1, record);
			info.name = namecpy;
			printf("%d\t%s", info.employee_id, info.name);
		}
		fclose(record);
	}
	line();
	getch(); 
}
void fun3(struct employee info) {
	
	FILE *record;
	char update[100];
	info.name = update;
	int num, cnt = 0;
	
	line();
	if (count == 0) printf("NO EMPLOYEE\n");
	else {
		record = fopen("record.txt", "rb+");

		printf("ENTER THE EMPLOYEE ID FOR UPDATE :\n");
		scanf("%d", &num);
		getchar();
		while (!feof(record)) {
			fread(&info.employee_id, sizeof(int) + sizeof(update), 1, record);
			if (num != info.employee_id) {
				cnt++;
			}
			else break;
		}
		if (num == info.employee_id) {
				rewind(record);
				printf("ENTER THE EMPLOYEE NAME TO BE UPDATED:\n");
				fseek(record, (cnt) * (sizeof(int) + sizeof(update))+sizeof(int), SEEK_SET);
				fgets(update, 100, stdin);
				fwrite(&update, sizeof(update), 1, record);
				fclose(record);
				
		}
			}
}
