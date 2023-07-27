#include <unistd.h>
#include <stdio.h>

void	push(int a){			//os2023_push를 호출, 스택에 원소를 넣기 위한 함수, 
	printf("Push %d\n", a);
	syscall(335, a);
}

void	pop(void){				//os2023_pop을 호출하는 함수
	int	ret;
	ret = (int)syscall(336);	//int로 scaling한 후 넣어줌
	printf("Pop %d\n", ret);
}

int	main(void){					//간단하게 숫자 몇개를 push, pop해줌
	push(1);
	push(1);
	push(2);
	push(3);
	pop();
	pop();
	pop();
	return (0);
}
