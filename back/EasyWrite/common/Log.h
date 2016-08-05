#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <fstream>
#include <windows.h>

typedef enum E_SAVE_TYPE
{
	LOG_RUN_LEVEL
}SAVE_TYPE;

void Char2Tchar(const char* src_char, TCHAR *dst_tchar);
void Tchar2Char(const TCHAR * tchar, char * _char);

void SaveFormattedLog(SAVE_TYPE type, const char *format_control_info, ...);



class Log
{
public:
	Log(const std::string& strFileName);
	~Log();
	bool Write(const std::wstring& strContent);
	static std::wstring GetAppDataPath();
	
private:
	std::wofstream m_outFile;
	E_SAVE_TYPE m_eLogType;
	int StringToWString(const std::string &str, std::wstring &wstr);
};


extern Log run_info;


#define LOG_RUN(content)   run_info.Write(std::wstring(content))

#endif //__LOG_H__

