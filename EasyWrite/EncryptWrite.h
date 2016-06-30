#pragma once

#include "LibUniEncrypt.h"
#include "FileHandler.h"

class CEncryptWrite
{
public:
	int WriteEncrypy(int index);
	int WriteEncrypy();
	CEncryptWrite(void);

//	CEncryptWrite(CFileHandler &handler);
	int GenerateFile();

//	int SetSrcFilePath(const char *file_path);
	int SaveOutFile(int index);
	int SaveOutFile();
	int ProgramEncrypt(int pos);
	int GetDeviceID(unsigned char *out_buf);

	int SaveTestBin(unsigned char *source_uid);
public:
	~CEncryptWrite(void);
private:

	static encrypt_data m_encrypt_pfn;
	//源文件对象
	CFileHandler m_file_handler;
	
	unsigned char    *encrypt_buff;

	unsigned char     *output_buff;

	unsigned char     m_out_head[MAX_HEAD_LEN + 1];

};
