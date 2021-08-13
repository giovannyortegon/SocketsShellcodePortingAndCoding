#include<windows.h>

int main(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	GetStartupInfo(&si);
	
	CreateProcess("execed.exe", NULL, NULL, NULL, FALSE,  0, NULL, NULL, &si, &pi);
	
	return (0);
}
