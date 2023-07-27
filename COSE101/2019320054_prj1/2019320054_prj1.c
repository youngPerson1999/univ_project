#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE 49

void fun1(void);
void fun2(int voc1,int voc2,int txt1, int txt2, float data1, float data2);
void fun3(int voc[],int txt[],float data[]);
void fun4(int voc[], int txt, float data[],float extra[], float price[]);
void fun5(int voc[], int txt[], float data[], float price[]);

int main(void) {
	int n, voc[3] = { 0 }, txt[3] = { 0 }, exit;
	float data[3] = { 0 }, extra[3] = { 0 }, price[3] = {0};
	char back;
	srand(time(NULL));
	voc[0] = rand() % 700 + 1; voc[1] = rand() % 700 + 1; //1 to 700
	txt[0]= rand() % 301; txt[1] = rand() % 301; //0 to 300
	data[0] = (rand() % 14951)*0.01 + 0.50; data[1] = (rand() % 14951)*0.01 + 0.50; //0.50 to 150.00
	for (int i = 0; i < 2; i++) {		//전달 요금계산
		price[i] = 45000;
		if (voc[i] - 300 > 0) price[i] += (voc[i] - 300) * 10;
		if (txt[i] - 100 > 0) price[i] += (txt[i] - 100) * 30;
		if (data[0] < 30 && data[0] <= 27 && data[1]>33) price[1] += (data[1] - 33) * 5000;
		else if (data[0] < 30 && data[0] > 27 && data[1]+data[0]>60) price[1] += (data[1] - 30 - (30 - data[0])) * 5000;
		if (data[0] - 30 > 0)price[i] += (data[i] - 30) * 5000;
		
	}
	price[2] = 45000;
		do {
		system("cls");		//아스키토드로 메뉴창 출력
		printf("%c", 0x01);
		for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
		printf("%c\n%c         KU Mobile Phone Bill Calculator         %c\n%c", 2, 5, 5,25);
		for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
		printf("%c\n%c    1 : Display plans                            %c\n", 23, 5, 5);
		printf("%c    2 : Usage information for past two months    %c\n", 5, 5);
		printf("%c    3 : Input this month usage                   %c\n", 5, 5);
		printf("%c    4 : Total fee for past 3 months              %c\n", 5, 5);
		printf("%c    5 : Best plan recommendation                 %c\n", 5, 5);
		printf("%c    6 : Exit                                     %c\n%c", 5, 5, 3);
		for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
		printf("%c\nInput menu number 1~6 : ", 4);
		scanf_s("%d", &n);
		if (n == 1) fun1(n);
		else if (n == 2) fun2(voc[0], voc[1], txt[0], txt[1], data[0], data[1]);
		else if (n == 3) {
			fun3(voc, txt, data);
			if (voc[2] - 300 > 0) price[2] += (voc[2] - 300) * 10;
			if (txt[2] - 100 > 0) price[2] += (txt[2] - 100) * 30;
			if (data[2]>30 && data[1] < 30 && 30 - data[1]>3)price[2] += (data[2] - 33) * 5000;	//이월데이터 계산
			else if(data[2] > 30 && data[1] < 30 && 30 - data[1] < 3)price[2] += (data[1] + data[2] - 60) * 5000;	//이월데이터 계산
		}
		else if (n == 4) fun4(voc, txt, data,extra,price);
		else if (n == 5) fun5(voc, txt, data, price);
		else if (n == 6) return 0;
		else continue;
		if (n > 0 && n < 6) {
			while (1) {
				printf("Back to main menu Y/N : ");
				scanf_s(" %c", &back);
				if (back == 'Y') break;
				else if (back == 'N') continue;
				else {
					printf("Please input Y/N\n");
					continue;
				}
			}
		}
		} while (n);
	return 0;
}
void fun1(void){
	int m;
	system("cls");
	printf("%c", 0x01);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n%c                   Basic Plan                    %c\n", 2, 5, 5);
	printf("%c", 25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n", 23);
	printf("%c    Included in plan   |  Additional usages      %c\n", 5, 5);
	printf("%c", 25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n", 23);
	printf("%c Voice      %c  500m free   %c %c 50 / 1m           %c\n", 5, 5, 5,92,5);
	printf("%c Text       %c  100t free   %c %c 10 / 1t           %c\n", 5,5,5,92,5);
	printf("%c Data       %c  2.00GB free %c %c 1000 / 0.1GB      %c\n", 5, 5,5,92,5);
	printf("%c", 25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n", 23);
	printf("%c Basice fee | %c 20000                            %c\n%c", 5,92, 5, 3);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n\n",4);
	printf("%c", 0x01);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n%c                 More Data Plan                  %c\n", 2, 5, 5);
	printf("%c", 25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n", 23);
	printf("%c    Included in plan   |  Additional usages      %c\n", 5, 5);
	printf("%c", 25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n", 23);
	printf("%c Voice      %c  300m free   %c %c 50 / 1m           %c\n", 5, 5, 5, 92, 5);
	printf("%c Text       %c  100t free   %c %c 30 / 1t           %c\n", 5, 5, 5, 92, 5);
	printf("%c Data       %c  30.00GB free%c %c 5000 / 0.1GB      %c\n%c", 5, 5, 5, 92, 5,25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n%c Basice fee | %c 45000                            %c\n%c",23, 5, 92, 5, 3);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n", 4);	
}
void fun2(int voc1,int voc2,int txt1,int txt2,float data1,float data2){
	system("cls");
	printf("%c", 0x01);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n%c              Last two months usage              %c\n%c", 2, 5, 5, 25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n%c My Plan : More Data Plan                        %c\n%c", 23, 5, 5,25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n%c          |   February    |    March             %c\n%c", 23, 5, 5, 25);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n%c Voice    %c   %5d min   %c   %5d min          %c\n", 23, 5, 5, voc1, 5, voc2, 5);
	printf("%c Text     %c   %5d text  %c   %5d text         %c\n", 5, 5, txt1, 5, txt2, 5);
	printf("%c Data     %c  %6.2f GB    %c  %6.2f GB           %c\n%c",5,5, data1,5, data2,5,3);
	for (int i = 1; i <= SIZE; i++)	printf("%c", 6);
	printf("%c\n", 4);
}
void fun3(int voc[],int txt[],float data[]) {
	system("cls");
	printf("================================\n");
	while (1) {
		printf("Input your usages of voice : ");
		scanf_s("%d", &voc[2]);
		if (getchar() != '\n') {
			while (getchar() != '\n');
			continue;
		}
		else if (voc[2] < 0) continue;
		else break;
	}
	while (1) {
		printf("Input your usages of text : ");
		scanf_s("%d", &txt[2]);
		if (getchar() != '\n') {
			while (getchar() != '\n');
			continue;
		}
		else if (txt[2] < 0) continue;
		else break;
	}
	while (1) {
		printf("Input your usages of data : ");
		scanf_s("%f", &data[2]);
		if (getchar() != '\n') {
			while (getchar() != '\n');
			continue;
		}
		else if (data[2] < 0) continue;
		else break;
	}
}
void fun4(int voc[], int txt[], float data[],float extra[], float price[]) {
	system("cls");
	for (int i = 0; i < 3; i++) {
		if (30 - data[i] > 0) extra[i] = 30 - data[i];
		else extra[i] = 0;
		if (extra[i] >= 3) extra[i] = 3;
	}
	printf("%c", 0x01);
	for (int i = 1; i <= SIZE+2; i++)	printf("%c", 6);
	printf("%c\n%c               My three months usage               %c\n%c", 2, 5, 5, 25);
	for (int i = 1; i <= SIZE+2; i++)	printf("%c", 6);
	printf("%c\n%c My Plan  :  More Data Plan                        %c\n%c", 23, 5, 5, 25);
	for (int i = 1; i <= SIZE+2; i++)	printf("%c", 6);
	printf("%c\n%c          |  February   |   March     |   April    %c\n%c", 23, 5, 5, 25);
	for (int i = 1; i <= SIZE+2; i++)	printf("%c", 6);
	printf("%c\n%c Voice    %c  %5d min  %c  %5d min  %c  %5d min %c\n", 23, 5, 5, voc[0], 5, voc[1],5,voc[2], 5);
	printf("%c Text     %c  %5d text %c  %5d text %c  %5d text%c\n", 5, 5, txt[0], 5, txt[1],5,txt[2], 5);
	printf("%c Data     %c %6.2f GB   %c %6.2f GB   %c %6.2f GB  %c\n%c", 5, 5, data[0], 5, data[1],5,data[2], 5, 25);
	for (int i = 1; i <= SIZE+2; i++)	printf("%c", 6);
	printf("%c\n%cExtra Data%c %6.2f GB   %c %6.2f GB   %c %6.2f GB  %c\n%c",23, 5, 5, extra[0], 5, extra[1], 5, extra[2], 5,25);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	printf("%c\n%cTotal Fee %c \\ %6.0f    %c \\ %6.0f    %c \\ %6.0f   %c\n%c",23, 5, 5, price[0], 5, price[1], 5, price[2], 5, 3);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	printf("%c\n", 4);
}
void fun5(int voc[],int txt[], float data[],float price[]) {
	system("cls");
	float basic[3] = { 0 };
	for (int i = 0; i < 3; i++) {
		basic[i] = 20000;
		if (voc[i] - 500 > 0) basic[i] += 50 * (voc[i] - 500);
		if (txt[i] - 100 > 0)basic[i] += 10*(txt[i] - 100);
		if (data[i] > 2)basic[i] += 10000 * (data[i] - 2);
	}

	printf("%c", 0x01);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	printf("%c\n%c               Average usage of 3-month            %c\n%c", 2, 5, 5, 25);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	printf("%c\n%c My Plan  :  More Data Plan                        %c\n%c", 23, 5, 5, 25);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	printf("%c\n%c Average usage of voice :    %3d                   %c\n%c", 23, 5,(int)((voc[0]+voc[1]+voc[2])/3 + 0.5) ,5, 5);
	printf(" Average usage of text :     %3d                   %c\n%c",(int)((txt[0] + txt[1] + txt[2]) / 3+0.5), 5, 5);
	printf(" Average usage of data :   %4.2f                   %c\n%c",(data[0] + data[1] + data[2]) / 3, 5, 25);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	printf("%c\n%c Basic Plan Fee :      \\  %6.0f                   %c\n", 23, 5, (basic[0] + basic[1] + basic[2]) / 3 + 0.5, 5);
	printf("%c More Data Plan Fee :  \\  %6.0f                   %c\n%c", 5, (price[0] + price[1] + price[2]) / 3, 5, 25);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	if (basic[0] + basic[1] + basic[2] > price[0] + price[1] + price[2]) printf("%c\n%c  We recommend to use More Data Plan               %c\n%c",23,5,5,3);
	else printf("%c\n%c  We recommend to use Basic Plan                   %c\n%c", 23, 5, 5,3);
	for (int i = 1; i <= SIZE + 2; i++)	printf("%c", 6);
	printf("%c\n", 4);
}