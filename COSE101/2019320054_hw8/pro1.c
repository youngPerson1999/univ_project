#include<stdio.h>
#define NUM 79

void back();
void line();
void fun1();
void fun2();
void fun3();
void fun4();
void bubblesort();

typedef struct class_grade {
	char name[30];
	int student_id;
	int attendance;
	int midterm_exam;
	int final_exam;
	int assignment;
	int term_project;
	int total_score;
}Grade;
Grade info[NUM];
int num = 0;
typedef struct class_report {
	int attendance;
	int midterm_exam;
	int final_exam;
	int assignment;
	int term_project;
	int total_score;
}Report;
Report aver[1];
Report medi[NUM];

int main(void) {

	int select;

	do {
		system("cls");
		printf("  COSE101 Grade Management System\n");
		line();
		printf("1. View Registered Student\n");
		printf("2. Add Student\n");
		printf("3. Class Report\n");
		printf("4. View Final Grade\n");
		printf("5. Exit\n\n");
		line();

		do {
			printf("Select Menu  :");	
			scanf_s("%d", &select);
		} while (select < 1 || select > 5);

		if (select == 1) fun1();
		else if (select == 2) {
			if (num < 78) fun2();
			else continue;
		}
		else if (select == 3) fun3();
		else if (select == 4) fun4();
		else if (select == 5) break;
	} while (1);

	return 0;
}
void back() {
	int menu;
	do {
		printf("Enter 1 ro Go Back : ");
		scanf_s("%d", &menu);
		if (menu == 1) break;
	} while (1);
}
void line(){
	printf("================================\n");
}
void fun1() {
	system("cls");
	line();
	printf("\tRegistered Student\n");
	line();
	if (num == 0)printf("There are no registed students\n");
	for (int i = 0; i < num; i++) {
		if (num == 0) break;
		else printf("%d\t\t%s\n", info[i].student_id, info[i].name);
	}
	line();
	back();
}
void fun2() {
	system("cls");
	line();
	printf("Student Name :");
	scanf_s("%s", info[num].name, 30);
	do {
		printf("Student ID : ");
		scanf_s("%d", &info[num].student_id);
	} while (info[num].student_id / 1000000000 == 0 || info[num].student_id / 1000000000 >= 10);
	do {
		printf("Attendance (0 ~ 10) : ");
		scanf_s("%d", &info[num].attendance);
	} while (info[num].attendance < 0 || info[num].attendance>10);
	do {
		printf("Assignment (0 ~ 10) : ");
		scanf_s("%d", &info[num].assignment);
	} while (info[num].assignment < 0 || info[num].assignment>10);
	do {
		printf("Midterm Exam (0 ~ 30) : ");
		scanf_s("%d", &info[num].midterm_exam);
	} while (info[num].midterm_exam < 0 || info[num].midterm_exam>30);
	do {
		printf("Final Exam (0 ~ 30) : ");
		scanf_s("%d", &info[num].final_exam);
	} while (info[num].final_exam < 0 || info[num].final_exam>30);
	do {
		printf("Term Project (0 ~ 20) : ");
		scanf_s("%d", &info[num].term_project);
	} while (info[num].term_project < 0 || info[num].term_project>20);
	info[num].total_score = info[num].attendance + info[num].assignment + info[num].midterm_exam + info[num].final_exam + info[num].term_project;
	num++;
}
void fun3() {
	system("cls");
	aver[0].attendance = 0;
	aver[0].midterm_exam = 0;
	aver[0].final_exam = 0;
	aver[0].assignment = 0;
	aver[0].term_project = 0;
	aver[0].total_score = 0;

	for (int i = 0; i < num; i++) {
		medi[i].attendance = info[i].attendance;
		medi[i].midterm_exam = info[i].midterm_exam;
		medi[i].final_exam = info[i].final_exam;
		medi[i].assignment = info[i].assignment;
		medi[i].term_project = info[i].term_project;
		medi[i].total_score = info[i].total_score;
	}

	printf("\tClass report\n");
	line();
	printf(" Field\t     Average  | Median\n");
	line();

	if (num == 0) {
		printf("Attendance  :         |\n");
		printf("Assignment  :         |\n");
		printf("Midterm\t    :         |\n");
		printf("Final\t    :         |\n");
		printf("Term Project:         |\n");
		printf("Total\t    :         |\n\n");
	}
	else {
		for (int i = 0; i <= num; i++) {
			aver[0].attendance += info[i].attendance;
			aver[0].midterm_exam += info[i].midterm_exam;
			aver[0].final_exam += info[i].final_exam;
			aver[0].assignment += info[i].assignment;
			aver[0].term_project += info[i].term_project;
			aver[0].total_score += info[i].total_score;
		}
		aver[0].attendance /= num;
		aver[0].midterm_exam /= num;
		aver[0].final_exam /= num;
		aver[0].assignment /= num;
		aver[0].term_project /= num;
		aver[0].total_score /= num;

		bubblesort();
		int odd = num / 2;
		int even = num / 2 - 1;

		printf("Attendance  :%5d    |  %5d\n", aver[0].attendance,(num%2)==1?medi[odd].attendance:(medi[odd].attendance+medi[even].attendance)/2);
		printf("Assignment  :%5d    |  %5d\n", aver[0].assignment, (num % 2)==1? medi[odd].assignment : (medi[odd].assignment + medi[even].assignment) / 2);
		printf("Midterm\t    :%5d    |  %5d\n", aver[0].midterm_exam, (num % 2)==1 ? medi[odd ].midterm_exam : (medi[odd].final_exam + medi[even].final_exam) / 2);
		printf("Term Project:%5d    |  %5d\n", aver[0].term_project, (num % 2)==1 ? medi[odd].term_project : (medi[odd].term_project + medi[even].term_project) / 2);
		printf("Total\t    :%5d    |  %5d\n\n", aver[0].total_score, (num % 2)==1 ? medi[odd].total_score : (medi[odd].total_score + medi[even].total_score) / 2);
	}
	line();
	back();
}
void fun4() {
	system("cls");
	printf("===========================================\n");
	printf("\t\tFinal Grade\n");
	printf("===========================================\n");
	printf("StudentID\t  Total Score\tGrade\n");
	if (num == 0) printf("There are no registed students\n");
	else {
		for (int i = 0; i < num; i++) {
			printf("%10d\t%4d\t",info[i].student_id,info[i].total_score);
			if (info[i].total_score >= 90)printf("\tA\n");
			else if (info[i].total_score < 90 && info[i].total_score >= 80)printf("\tB\n");
			else if (info[i].total_score < 80 && info[i].total_score >= 70)printf("\tC\n");
			else printf("\tD\n");
		}
	}
	printf("===========================================\n");
	back();
}
void bubblesort() {
	int tmp;
	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - 1; j++) {
			if (medi[j].attendance > medi[j + 1].attendance) {
				tmp = medi[j].attendance;
				medi[j].attendance = medi[j + 1].attendance;
				medi[j + 1].attendance = tmp;
			}
		}
	}
	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - 1; j++) {
			if (medi[j].midterm_exam > medi[j + 1].midterm_exam) {
				tmp = medi[j].midterm_exam;
				medi[j].midterm_exam = medi[j + 1].midterm_exam;
				medi[j + 1].midterm_exam = tmp;
			}
		}
	}
	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - 1; j++) {
			if (medi[j].final_exam > medi[j + 1].final_exam) {
				tmp = medi[j].final_exam;
				medi[j].final_exam = medi[j + 1].final_exam;
				medi[j + 1].final_exam = tmp;
			}
		}
	}
	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - 1; j++) {
			if (medi[j].assignment > medi[j + 1].assignment) {
				tmp = medi[j].assignment;
				medi[j].assignment = medi[j + 1].assignment;
				medi[j + 1].assignment = tmp;
			}
		}
	}
	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - 1; j++) {
			if (medi[j].term_project > medi[j + 1].term_project) {
				tmp = medi[j].term_project;
				medi[j].term_project = medi[j + 1].term_project;
				medi[j + 1].term_project = tmp;
			}
		}
	}
	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - 1; j++) {
			if (medi[j].total_score > medi[j + 1].total_score) {
				tmp = medi[j].total_score;
				medi[j].total_score = medi[j + 1].total_score;
				medi[j + 1].total_score = tmp;
			}
		}
	}
}