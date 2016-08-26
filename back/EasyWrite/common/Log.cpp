#include "Log.h"
#include "common.h"
#include <Shlobj.h>  
#include <stdio.h>  
#include <locale.h>
#include <time.h>
#include <winnt.h>
#include <tchar.h>


static int g_log_switcher = 0;


Log run_info("status.txt");


void Char2Tchar(const char* src_char, TCHAR *dst_tchar)
{
#ifndef _UNICODE
	strcpy(dst_tchar, src_char);
#else
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src_char, strlen(src_char), dst_tchar, HTTP_URL_MAX_LEN);
#endif
}

void Tchar2Char(const TCHAR * tchar, char * _char)
{
	int iLength;
	//获取字节长度   
#ifndef _UNICODE
	strcpy(_char, tchar);
#else
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);

	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
#endif
}

void SaveFormattedLog(SAVE_TYPE type, const char *format_control_info, ...)
{
	char ch_info[LOG_BUFFER_MAX_LEN * 2] = { 0 };
	va_list arg_ptr;
	va_start(arg_ptr, format_control_info);
	_vsnprintf(ch_info, sizeof(ch_info) - 1, format_control_info, arg_ptr);

	//日志开关
	if (0 == g_log_switcher)
	{
		printf("%s \n", ch_info);
		return;
	}
	

	TCHAR tch_info[LOG_BUFFER_MAX_LEN * 2] = { 0 };
	Char2Tchar(ch_info, tch_info);
	
	if(LOG_RUN_LEVEL ==type)
	{
		LOG_RUN(tch_info);
	}
	else
	{
		printf("unknown log type! \r\n");
	}
}


BOOL Log::StringToWString(const std::string &str, std::wstring &wstr)
{
	int nLen = (int)str.length();
	wstr.resize(nLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}

   void    ReturnPath()  
   {   
	   TCHAR file_name[512] = {0}; 
   GetModuleFileName(NULL,file_name,MAX_PATH);   
   printf(" \r\n GetModuleFileName: %ls \r\n ",file_name);
   }

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

void GetDLLPath()
{
#if 0
	WCHAR   DllPath[MAX_PATH] = {0};
	GetModuleFileNameW((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
	printf("\r\n  GetDLLPath: %ls \r\n",DllPath);

	std::wstring thisPath(L"");
    WCHAR path[MAX_PATH];
    HMODULE hm;
	TCHAR thisDllDirPath[512] = {0};
    if( GetModuleHandleExW( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
                            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            (LPWSTR) &thisDllDirPath, &hm ) )
    {
        GetModuleFileNameW( hm, path, sizeof(path) );
		printf("hahaha %ls \r\n \r\n",path);
        //PathRemoveFileSpecW( path );
        //thisPath = CStringW( path );
        //if( !thisPath.IsEmpty() && 
        //    thisPath.GetAt( thisPath.GetLength()-1 ) != '\\' ) 
        //    thisPath += L"\\";
    }
#endif
    DWORD  nBufferLength = 512;
	TCHAR pBuffer[512] = {0};
	GetCurrentDirectory(nBufferLength,pBuffer);
	printf("\r\n GetCurrentDirectory: %ls \r\n",pBuffer);


}

Log::Log(const std::string& strFileName)
{
  

  
	//日志开关
	g_log_switcher = GetPrivateProfileInt(_T("options"), _T("log_enable"), 0, _T(".\\writer.ini"));
	if (0 == g_log_switcher)
	{
		// close file log
		return;
	}
	std::wstring wstr_log_file_path(L"");
	TCHAR path_t[FILE_NAME_MAX_LEN] = { 0 };
	char path[FILE_NAME_MAX_LEN] = { 0 };
	GetPrivateProfileString(_T("options"), _T("log_path"), _T(".\\"), path_t, sizeof(path_t), _T(".\\writer.ini"));
	Tchar2Char(path_t, path);

	std::string log_file_path(path);
	log_file_path += std::string("\\");
	std::string full_name = std::string(__DATE__) + std::string("_") + strFileName;
	log_file_path += full_name;
	StringToWString(log_file_path, wstr_log_file_path);

	m_outFile.open(wstr_log_file_path.c_str(), std::ios::out | std::ios::app);
}

Log::~Log()
{
	if (g_log_switcher)
	{
		m_outFile.close();
	}	
}

bool Log::Write(const std::wstring& strContent)
{
	time_t t = time(NULL);
	TCHAR tchTime[260];

	if (!m_outFile.is_open())
	{
		return false;
	}
	wcsftime(tchTime, sizeof(tchTime), L"%Y/%m/%d %H:%M:%S", localtime(&t));
	try
	{
		m_outFile << tchTime << ", " << strContent << std::endl;
	}
	catch (...)
	{
		
	}
	
	return true;
}

std::wstring Log::GetAppDataPath()
{
	TCHAR szPath[260];
	memset(szPath, 0, sizeof(szPath));
	SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath));
	return std::wstring(szPath);
}