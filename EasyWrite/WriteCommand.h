#pragma once
#include "common/Log.h"
#include  "FileHandler.h"


#define MAX_COMMAND_LEN  512
class CWriteCommand
{
public:
	CWriteCommand(void);
    CWriteCommand(CFileHandler &handle);
	//Detect device
	int DetectDevice();


	//从指定位置开始编程
	int ProgramDevice(unsigned int ui_start_position);

	int ProgramFillDevice(unsigned int ui_start_position,char fill_char);

	
	//擦除芯片
	int EraseChip();

	int ProgramEncryptData(int position);

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
