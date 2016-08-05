#include <process.h>
#include "Task.h"
#include "common/log.h"

CTask::CTask(void)
{
	m_thread_handle = NULL;
}

CTask::~CTask(void)
{
	if(m_thread_handle != NULL)
	{

		CloseHandle(m_thread_handle);
	}
}

CTask::CTask(char* task_content)
{
	m_thread_handle = NULL;
	m_thread_handle =  (HANDLE)_beginthreadex(NULL, 0, CTask::ExcuterCMD, task_content, 0,NULL);
	if(NULL == m_thread_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Create task thread failed!");
	}

}

HANDLE CTask::GetThreadHanlde()
{
	return m_thread_handle;
}

unsigned int CTask::ExcuterCMD(void *arg)
{
	char *command = (char *)arg;
	int error_code = system(command);
	SaveFormattedLog(LOG_RUN_LEVEL,"finish excute command %s",command);
	return error_code;
}