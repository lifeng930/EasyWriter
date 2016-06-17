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


	//��ָ��λ�ÿ�ʼ���
	int ProgramDevice(unsigned int ui_start_position);

	int ProgramFillDevice(unsigned int ui_start_position,char fill_char);

	
	//����оƬ
	int EraseChip();

	int ProgramEncryptData(int position);

public:
	~CWriteCommand(void);
private:
	char m_command_str[MAX_COMMAND_LEN];

	//�������ʱʱ��
	unsigned int m_timeout_ui;

	CFileHandler m_file_handler;

	int ExcuteCommand();
	int SetFileHandler(CFileHandler &handler);
};
