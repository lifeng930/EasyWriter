#pragma once

#include "LibUniEncrypt.h"
#include "FileHandler.h"

#define DEVICE_UID_LEN 15
class CEncryptWrite
{
public:

	// device_index start from 1
	int WriteEncrypy(int device_index);
	int WriteEncrypy();
	CEncryptWrite(void);

//	CEncryptWrite(CFileHandler &handler);
	int GenerateFile();

	// start from 1
	int GenerateFile(int device_index);

//	int SetSrcFilePath(const char *file_path);
	int SaveOutFile(int index);
	int SaveOutFile();
	int ProgramEncrypt(int pos);

	
	int GetDeviceID(unsigned char *out_buf);

	// index start from 1
	int GetDeviceID(int device_index ,unsigned char *out_buf);

public:
	~CEncryptWrite(void);
private:

	static encrypt_data m_encrypt_pfn;
	//源文件对象
	CFileHandler m_file_handler;
	
	unsigned char    *encrypt_buff;

	unsigned char     *output_buff;

	unsigned char     m_out_head[MAX_HEAD_LEN + 1];

	char                    m_device_uid[DEVICE_UID_LEN];

};
