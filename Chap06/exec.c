#include<stdio.h>
#include<process.h>
#include<windows.h>

int main(void)
{
	const char * argv[] = {"execed", NULL};
	
	Sleep(3000);
	
	execv("execed",  argv);
	
	printf("Never reached");

	return (0);
}
