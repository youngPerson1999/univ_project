#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

// 함수 원형 선언

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 2배씩 증가
void load_names(FILE *fp, int year_index, tNames *names);

// 구조체 배열을 화면에 출력
void print_names(tNames *names, int num_year);

// qsort를 위한 비교 함수
int compare(const void *n1, const void *n2);

// 함수 정의

// 이름 구조체 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc(sizeof(tNames));

	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	tNames *names;

	int num = 0;
	FILE *fp;
	int num_year = 0;

	if (argc == 1) return 0;

	// 이름 구조체 초기화
	names = create_names();

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = atoi(&argv[1][3]);

	for (int i = 1; i < argc; i++)
	{
		num_year++;
		fp = fopen(argv[i], "r");
		assert(fp != NULL);

		int year = atoi(&argv[i][3]); // ex) "yob2009.txt" -> 2009

		fprintf(stderr, "Processing [%s]..\n", argv[i]);

		// 연도별 입력 파일(이름 정보)을 구조체에 저장
		load_names(fp, year - start_year, names);
		fclose(fp);
	}

	// 정렬 (이름순 (이름이 같은 경우 성별순))
	qsort( names->data, names->len, sizeof(tName), compare);

	// 이름 구조체를 화면에 출력
	print_names( names, num_year);

	// 이름 구조체 해제
	destroy_names(names);

	return 1;
}
void load_names(FILE *fp, int year_index, tNames *names) {				//표준 출력이므로 redirection 필요 *.txt > list로
//linear search를 통해 겹치는지 확인, 겹치면 기존 것에 입력, 안겹치면 새로 생성
	char buffer[50];	char name[20]={' '};	char sex;	int freq;	int i;
	tName person;
	while (fscanf(fp, "%s", buffer) != EOF) {
		i = 0;
		char* ptr = strtok(buffer, ",");
		strcpy(name, ptr);							//name에 이름 insert
		ptr = strtok(NULL, ",");
		char sex = *ptr;						//sex에 성 insert
		ptr = strtok(NULL, ",");
		int freq = atoi(ptr);					//freq에 인원 insert
		strcpy(person.name , name);
		person.sex = sex;
		person.freq[year_index] = freq;
		
		int check = 0;		//같은 이름 있으면 1, 아니면 0

		for (i = 0; i < names->len; i++) {
			if (strcmp(name, names->data[i].name) == 0&&names->data[i].sex==person.sex) {
				names->data[i].freq[year_index] = person.freq[year_index];
				check++;
				break;
			}
		}
		if (check != 0)	continue;
		names->len++;
		if (names->len > names->capacity) {		//만약 len>=capacity라면 2배로 늘려줌
			names->capacity *= 2;
			names->data = (tName*)realloc(names->data, names->capacity * 2 * sizeof(tName));
		}
		strcpy(names->data[i].name, person.name);		//person의 데이터를 삽입
		names->data[i].sex = person.sex;
		for (int j = 0; j < MAX_YEAR_DURATION; j++) {
			names->data[i].freq[j] = 0;
		}
		names->data[i].freq[year_index] = person.freq[year_index];
	}

}
void print_names(tNames *names, int num_year) {
	FILE *out;
	out = fopen("result", "w");
	for(int i=0;i<names->len;i++) {
		fprintf(out, "%s\t%c", names->data[i].name, names->data[i].sex);
		for (int j = 0; j < MAX_YEAR_DURATION; j++) {
			fprintf(out, "\t%d", names->data[i].freq[j]);
		}
		fprintf(out, "\n");
	}
}
int compare(const void *n1, const void *n2) {
	tName *a=(tName*)n1;
	tName *b=(tName*)n2;
	
	if(strcmp(a->name,b->name)>0)	return 1;
	else if(strcmp(a->name,b->name)<0)	return -1;
	else{
		if(a->sex>b->sex)	return 1;
		else return -1;
	}
}








