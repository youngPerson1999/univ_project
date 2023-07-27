#include<stdio.h>

void uplow(char*str);

int main(void) {
	char arr[90] = { "PoINteR is vERy ImPorTAnt in C pRoGraMMing, SO STudEnt nEEd tO pRACtiCe usING poINTer" };
	printf("%s\n",arr);
	uplow(&arr);
	printf("%s", arr);
	return 0;
}
void uplow(char*str) {
	for (int i = 0; *str; i++) {
		if (*str > 'A' && *str < 'Z') *str += 'a' - 'A';
		else if (*str > 'a'&&*str < 'z')*str -= 'a' - 'A';
		++str;
	}
}
