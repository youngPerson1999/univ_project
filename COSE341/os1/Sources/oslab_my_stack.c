#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

int stack[100];
int size = 0;

void	print_stack(void){	//stack을 print해주는 함수
	static	int	i;	
	i = size;
	printk("Stack Top -----------------------\n");
	while (--i >= 0){
		printk("%d\n", stack[i]);
	}
	printk("Stack Bottom --------------------\n");
}

SYSCALL_DEFINE1(os2023_push, int, a){
	static int i,d;
	i = size;
	d = 1;					//중복이면 0, 아니면 1
	while(--i>=0){			//중복 검사해주는 단계
		if(stack[i]==a){
			d = 0;
			break;
		}
	}
	if (d==1){				//push를 해주는 과정 맨위에 a를 넣고 size를 늘림
		stack[size++] = a;
	}
	printk("[System Call] os2023_push :\n");
	print_stack();
}

SYSCALL_DEFINE0(os2023_pop){
	int	ret;
	ret = -1;
	if (size > 0){			//stack에 원소가 있을 때만, pop 가능
		ret = stack[--size];//ret에 top에 있는 값을 넣어주고 size를 줄여준다.
	}
	printk("[System Call] os2023_pop :\n");
	print_stack();
	return (ret);			
}
