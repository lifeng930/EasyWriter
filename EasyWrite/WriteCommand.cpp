#include "StdAfx.h"
#include "WriteCommand.h"
#include "common/common.h"
#include <process.h>


CWriteCommand::CWriteCommand(void)
{
	memset(m_command_str,0,MAX_COMMAND_LEN);
	m_timeout_ui = 30;
}

CWriteCommand::~CWriteCommand(void)
{

}

CWriteCommand::CWriteCommand(CFileHandler &handle)
{
	SetFileHandler(handle);
}

unsigned int WINAPI CWriteCommand::CommandThread(void *args)
{
	int exit_code = 1;
	CWriteCommand *pCommand = (CWriteCommand *)args;
	try
	{
	   exit_code = system(pCommand->GetCommand());
	}
	catch(...)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"excution command %s failed,unknown exception",pCommand->GetCommand());
		return 1;
	}
	if(exit_code != 0)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"failed to excute command:%s,retrun code: %d",pCommand->GetCommand(),exit_code);
	}
	else
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"success to excute command:%s",pCommand->GetCommand(),exit_code);
	}
	return exit_code;
}

int CWriteCommand::ExcuteCommand()
{
	DWORD exit_code = 1;
	
    HANDLE command_thread_handle = (HANDLE)_beginthreadex(NULL, 0, CWriteCommand::CommandThread, this, 0,NULL);	
	if(NULL == command_thread_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Create command  thread failed!");
		return 1;
	}
	if(WAIT_TIMEOUT  == WaitForSingleObject(command_thread_handle ,25000))
	{
		TerminateThread(command_thread_handle ,2);	
		SaveFormattedLog(LOG_RUN_LEVEL,"command thread:%s timeout!",m_command_str);
		return 2;
	}
	GetExitCodeThread(command_thread_handle,&exit_code);
	CloseHandle(command_thread_handle);
	int ret_code = exit_code;
	return ret_code; 
}

int  CWriteCommand::DetectDevice()
{
	int error_code = 1;
	memset(m_command_str,0,MAX_COMMAND_LEN);
	strcpy(m_command_str,"dpcmd -d");
	SaveFormattedLog(LOG_RUN_LEVEL,"begin excute %s ",m_command_str);
	error_code = ExcuteCommand();
	return error_code;
}

int CWriteCommand::ProgramDevice(unsigned int ui_start_position)
{
	int error_code = 1;
	if(0 == strlen(m_file_handler.GetFilePath()))
	{
		MessageBox(NULL,TEXT("file path not specified!"),TEXT("warning"),MB_OK);
		return error_code;
	}
	memset(m_command_str,0,MAX_COMMAND_LEN);
	sprintf(m_command_str,"dpcmd -p %s  -a  0x%x",m_file_handler.GetFilePath(),ui_start_position);
	SaveFormattedLog(LOG_RUN_LEVEL,"begin excute %s ",m_command_str);
	error_code = ExcuteCommand();
	return error_code;
}

int CWriteCommand::SetFileHandler(CFileHandler &handler)
{
	m_file_handler = handler;
	return 0;
}

int CWriteCommand::ProgramSysData(int position,int index)
{
	int error_code = 1;
	sprintf(m_command_str,"dpcmd -p %s -a 0x%x  --type W25Q64FV -v",m_file_handler.GetFilePath(),position);
	
	error_code = ExcuteCommand();
	return error_code;
}

int CWriteCommand::ProgramSysData(int position)
{
	int error_code = 1;
	sprintf(m_command_str,"dpcmd -p %s -a 0x%x --type W25Q64FV -v",m_file_handler.GetFilePath(),position);
	
	error_code = ExcuteCommand();
	return error_code;
}

int CWriteCommand::ProgramEncryptData(int position,int index)
{
	int error_code = 1;
	sprintf(m_command_str,"dpcmd -p %s  -a 0x%x --device %d ",m_file_handler.GetFilePath(),position,index + 1);
	
	error_code = ExcuteCommand();
	return error_code;

}

int CWriteCommand::ProgramEncryptData(int position)
{
	int error_code = 1;
	sprintf(m_command_str,"dpcmd -p %s  -a 0x%x ",m_file_handler.GetFilePath(),position);
	
	error_code = ExcuteCommand();
	return error_code;
}

int CWriteCommand::EraseChip()
{
	int error_code = 1;
	memset(m_command_str,0,MAX_COMMAND_LEN);
	sprintf(m_command_str,"dpcmd -e");
	error_code = ExcuteCommand();
	return error_code;
}