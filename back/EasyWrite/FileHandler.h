#pragma once
#include <string>
#include "common/common.h"
#include "common/log.h"

class CFileHandler
{
public:
	CFileHandler(void);
	CFileHandler(CFileHandler &other);
	CFileHandler(std::string &path);
	CFileHandler(const char *path);
	int SetFilePath(char *file_path);
	const char *GetFilePath();
	CFileHandler& operator=(CFileHandler &other);
	unsigned int GetFileLength() const ;
public:
	~CFileHandler(void);
private:

	// Bin ÎÄ¼þÂ·¾¶
	char m_file_path[MAX_FILE_PATH_LEN];
//	unsigned char *m_total_buffer;
};
