#include "FileHandler.h"

CFileHandler::CFileHandler(void)
{
	memset(m_file_path,0,MAX_FILE_PATH_LEN);
}



CFileHandler::~CFileHandler(void)
{


}

CFileHandler::CFileHandler(std::string &path)
{
	memset(m_file_path,0,MAX_FILE_PATH_LEN);
	strcpy(m_file_path,path.c_str());

}


CFileHandler::CFileHandler(CFileHandler &other)
{
	memset(m_file_path,0,MAX_FILE_PATH_LEN);
	strcpy(m_file_path,other.GetFilePath());
}

const char *CFileHandler::GetFilePath() 
{

	return m_file_path;
}

CFileHandler& CFileHandler::operator =(CFileHandler &other)
{
	memset(m_file_path,0,MAX_FILE_PATH_LEN);
	strcpy(m_file_path,other.GetFilePath());
	FILE *file_pointer = fopen(m_file_path,"r");

	return *this;
}

CFileHandler::CFileHandler(const char *path)
{
	
	if(strlen(path) > MAX_FILE_PATH_LEN - 1)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"tool long file path");
		return ;
	}

	memset(m_file_path,0,MAX_FILE_PATH_LEN);
	strcpy(m_file_path,path);
}

unsigned int CFileHandler::GetFileLength() const
{
	FILE *file_pointer = fopen(m_file_path,"r");
	if(NULL == file_pointer)
	{
		return -1;
	}
	fseek(file_pointer,0,SEEK_END); //把指针移动到文件的结尾 ，获取文件长度
	unsigned int len=ftell(file_pointer); //获取文件长度
	fclose(file_pointer);
	return len;
}

int CFileHandler::SetFilePath(char *file_path)
{
    if(strlen(file_path) > MAX_FILE_PATH_LEN - 1)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"file path too long");
		return 1;
	}
	memset(m_file_path,0,MAX_FILE_PATH_LEN);
	strcpy(m_file_path,file_path);
	return 0;
}
