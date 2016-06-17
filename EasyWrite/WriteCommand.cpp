#include "StdAfx.h"
#include "WriteCommand.h"
#include "common/common.h"


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

int CWriteCommand::ExcuteCommand()
{
    int ret_code = system(m_command_str);
	if(ret_code != 0)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"failed to excute command:%s,retrun code: %d",m_command_str,ret_code);
	}
	else
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"success to excute command:%s",m_command_str,ret_code);
	}
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

int CWriteCommand::ProgramEncryptData(int position)
{
	int error_code = 1;
	sprintf(m_command_str,"dpcmd -p %s -a 0x%x",m_file_handler.GetFilePath(),position);
	
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