#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "stdlib.h"
#include "typedefs.h"
#include "sched.h"
#include "signal.h"
#include "unistd.h"

#define FIBER_STACK 8192

int32 a;
void *stack;

int32 do_something()
{
	printf("This is son,the pid is:%d, the a is:%d\n", getpid(), ++a);
	free(stack);
	exit(1);
}

int main(int argc, char *argv[])
{
	void *stack;

	a = 1;

	stack = malloc(FIBER_STACK);
	if( NULL == stack )
	{
		printf("the stack faild\n");
	}

	printf("creating son thread!!!\n");
	clone(do_something, (char *)stack + FIBER_STACK, CLONE_VM|CLONE_VFORK, 0);
	printf("this is father,my pid is:%d,the a is:%d\n ", getpid(), a);

	//����develop��֧ʹ��
	printf("this is father,my pid is:%d,the a is:%d\n ", getpid(), a);
	//����git push֮ǰ���������뱾�ز�һ��
	
	//������ֱ�Ӹ������ļ�����

	//���ر���ת��֮����push�������������صı�����GB18030,����������UTF-8
		
	return 0;
}

