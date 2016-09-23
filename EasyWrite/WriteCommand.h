#pragma once
#include "common/Log.h"
#include  "FileHandler.h"
#include <windows.h>


#define MAX_COMMAND_LEN  512
class CWriteCommand
{
public:
	CWriteCommand(void);
    CWriteCommand(CFileHandler &handle);
	//Detect device
	int DetectDevice();

	static unsigned int WINAPI CommandThread(void *args);


	int ProgramFillDevice(unsigned int ui_start_position,char fill_char);

	
	//擦除芯片
	int EraseChip();

	// device_index start from 1
	int EraseChip(int device_index);

	// device_index start from 1
	int ProgramSysData(int position,const int device_index);
	int ProgramSysData(int position);

	//device_index start from 1
	int ProgramEncryptData(int position,const int device_index);
	int ProgramEncryptData(int position);

	char *GetCommand(){return m_command_str;}

	int ValidationSysData(const char *sys_data_path);
	int ValidationSysData(int index,const char *sys_data_path);

	int ValidationEncryptData(const char *encrypt_data_path);
	int ValidationEncryptData(int index,const char *encrypt_data_path);

public:
	~CWriteCommand(void);
private:
	char m_command_str[MAX_COMMAND_LEN];

	//单个命令超时时间
	unsigned int m_timeout_ui;

	CFileHandler m_file_handler;

	int ExcuteCommand();
	int SetFileHandler(CFileHandler &handler);
};
