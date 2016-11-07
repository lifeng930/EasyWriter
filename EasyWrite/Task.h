#pragma once
#include <windows.h>



class CTask
{
public:
	CTask(void);
	CTask(char* task_content);
public:
	~CTask(void);
	static unsigned int __stdcall ExcuterCMD(void *arg);
	HANDLE GetThreadHanlde();

private:
	HANDLE m_thread_handle;
	
};
