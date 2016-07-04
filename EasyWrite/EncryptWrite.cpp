#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "EncryptWrite.h"
#include "common/Log.h"
#include "WriteCommand.h"


CEncryptWrite::CEncryptWrite(void)
{
	unsigned char temp[MAX_HEAD_LEN + 1] = {0x53,0x46,0x53,0x46,
	                               0x05,0x00,0x00,0x00,
	                               0x00,0xA5,0x3B,0xCA,
	                               0x00};
	memcpy(m_out_head,temp,MAX_HEAD_LEN);
}


int CEncryptWrite::ProgramEncrypt(int pos)
{
	return 0;
}


CEncryptWrite::~CEncryptWrite(void)
{
	if(NULL != output_buff)
	{
		delete []output_buff;
		output_buff = NULL;
	}
	if(NULL != encrypt_buff)
	{

		delete []encrypt_buff;
		encrypt_buff = NULL;
	}
}



int TestOrder(char *file_name,unsigned char *buff,int len)
{

	FILE *fp = fopen(file_name,"w");
	if(fp == NULL)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Test Order failed");
		return 1;
	}
	fwrite(buff,1,len,fp);
	fclose(fp);
	return 0;
}

int CEncryptWrite::WriteEncrypy()
{
	output_buff = new unsigned char[MAX_BIN_FILE_LEN + MAX_HEAD_LEN + 1];
	if(NULL == output_buff)
	{
		return 1;
	}
	memset(output_buff,0,MAX_BIN_FILE_LEN + MAX_HEAD_LEN + 1);
	memcpy(output_buff,m_out_head,MAX_HEAD_LEN);

	unsigned char *temp = output_buff + MAX_HEAD_LEN;
	unsigned char *pos = encrypt_buff;
	unsigned int *inverse_data = new unsigned int[1024*2/4];
	
	/*  循环写2k加密数据 */
	for(int i = 0;i < 1024 *2/4;i++)
	{
		inverse_data[i] = pos[4*i] + (pos[4*i +1] << 8) + (pos[4*i + 2] << 16) + (pos[4*i + 3]<<24);
	}
	memcpy(temp,inverse_data,MAX_BIN_FILE_LEN);	
	
	delete []inverse_data;

	int error_code = SaveOutFile();
	
	return error_code;
}

int CEncryptWrite::SaveOutFile()
{
	/*  先判断文件是否存在， */
	if(-1 != _access("C:\\encrypt.bin",0) )
	{
		//删除文件
		DeleteFile(TEXT("encrypy.bin"));
	}
	FILE *fp = fopen("C:\\encrypt.bin","wb");
	if(NULL == fp)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"save generate file failed! ");
		return 1;
	}
	int write_len = fwrite(output_buff,sizeof(char),MAX_BIN_FILE_LEN + MAX_HEAD_LEN,fp);

	/* 附加14k的随机数据 */
	unsigned int count = 14 * 1024;
	for(unsigned int i = 0;i < count;i++)
	{
		unsigned char temp_char = rand()%255;
		fwrite(&temp_char,1,1,fp);
	}
	//将文件指针移动到尾部
	fseek(fp,0,SEEK_END); //把指针移动到文件的结尾 ，获取文件长度
	unsigned int len=ftell(fp); //获取文件长度

	fclose(fp);
	fp = NULL;

	CFileHandler handler("C:\\encrypt.bin");
	CWriteCommand excuter(handler);
	excuter.ProgramEncryptData(0x300000);
	return 0;

}



int CEncryptWrite::GenerateFile()
{
	int error_code = 1;
	HINSTANCE instance = LoadLibrary(TEXT("LibUniEncrypt.dll"));
	if(NULL == instance)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"call LoadLibrary error: %d",GetLastError());
		return error_code;
	}
	LPCSTR lstr = "encrypt_data";
    encrypt_data GenerateFunc = (encrypt_data)GetProcAddress(instance,lstr);
	if(NULL == GenerateFunc)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"call GetProcAddress error: %d",GetLastError());
		return error_code;
	}

	/* 获取设备的uid  */
	unsigned char temp_id[15] = {0};
	try
	{
		error_code = GetDeviceID(temp_id);
	}
	catch(...)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"GetDeviceID unknown exception for get device id");
		return 1;
	}
	if(error_code != 0)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Get device uid failed! ");
		return error_code;
	}
	encrypt_buff = new unsigned char[MAX_BIN_FILE_LEN];
	memset(encrypt_buff ,0 ,MAX_BIN_FILE_LEN);
	if(NULL == encrypt_buff)
	{
		return error_code;
	}
	try
	{
		error_code = GenerateFunc((uint8_t*)temp_id,8,(uint8_t*)encrypt_buff,MAX_BIN_FILE_LEN);
	}
	catch(...)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"call GenerateFunc unknown exception");
		return 1;
	}
	FreeLibrary(instance);
	SaveFormattedLog(LOG_RUN_LEVEL,"call  encrypt function return: %d",error_code);
	return 0;
}

typedef int (*AddFunc) (int x,int y);

//c++ 版本 split
void split(std::string& s, std::string& delim,std::vector< std::string >* ret)  
{  
    size_t last = 0;  
    size_t index=s.find_first_of(delim,last);  
    while (index!=std::string::npos)  
    {  
        ret->push_back(s.substr(last,index-last));  
        last=index+1;  
        index=s.find_first_of(delim,last);  
    }  
    if (index-last>0)  
    {  
        ret->push_back(s.substr(last,index-last));  
    }  
}  

int CEncryptWrite::SaveTestBin(unsigned char *source_uid)
{
	int error_code = 1;
	unsigned char *order_byte = source_uid;
	unsigned char out_buffer[8] = { 0 };

	out_buffer[0] = source_uid[4];
	out_buffer[1] = source_uid[5];
	out_buffer[2] = source_uid[6];
	out_buffer[3] = source_uid[7];
	out_buffer[4] = source_uid[0];
	out_buffer[5] = source_uid[1];
	out_buffer[6] = source_uid[2];
	out_buffer[7] = source_uid[3];

	SaveFormattedLog(LOG_RUN_LEVEL, "source_uid: %x %x %x %x %x %x %x %x", source_uid[0], source_uid[1], source_uid[2], source_uid[3], source_uid[4], source_uid[5], source_uid[6], source_uid[7]);
	encrypt_buff = new unsigned char[MAX_BIN_FILE_LEN];
	memset(encrypt_buff, 0, MAX_BIN_FILE_LEN);
	if (NULL == encrypt_buff)
	{
		return error_code;
	}

	HINSTANCE instance = LoadLibrary(TEXT("LibUniEncrypt.dll"));
	if (NULL == instance)
	{
		SaveFormattedLog(LOG_RUN_LEVEL, "call LoadLibrary error: %d", GetLastError());
		return error_code;
	}
	LPCSTR lstr = "encrypt_data";
	encrypt_data GenerateFunc = (encrypt_data)GetProcAddress(instance, lstr);
	if (NULL == GenerateFunc)
	{
		SaveFormattedLog(LOG_RUN_LEVEL, "call GetProcAddress error: %d", GetLastError());
		return error_code;
	}
	error_code = GenerateFunc((uint8_t*)out_buffer, 8, (uint8_t*)encrypt_buff, MAX_BIN_FILE_LEN);

	FreeLibrary(instance);
	
	

	
	WriteEncrypy();
	FILE *fp = fopen("C:\\raw.bin", "wb");
	if (NULL == fp)
	{
		SaveFormattedLog(LOG_RUN_LEVEL, "open file handler for write raw data failed!");
		return 1;
	}
	fwrite(encrypt_buff, sizeof(char), MAX_BIN_FILE_LEN, fp);
	fclose(fp);
	fp = NULL;
	
	return 0;
}

extern void AlarmVoice();

// out_buffer 至少有13个存储字节
int CEncryptWrite::GetDeviceID(unsigned char *out_buffer)
{
	int error_code = 1;

	//存储按顺序排列的字节，中间结果
	unsigned char order_byte[8] = {0};
	char command_str[256] = {0};
	strcpy(command_str,"dpcmd --raw-instruction \"4B\" --raw-require-return 12");
	char buffer[1024] = {0};
	int index = 0;
	unsigned int temp_interpretor[8] = {0};
	FILE *pipe = _popen(command_str,"r");
	if(!pipe)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"create pipe failed,error code: %d!",errno);
		AlarmVoice();
		Sleep(5000);
		return 1;
	}

	while(!feof(pipe))
	{
		if(fgets(buffer,1024,pipe))
		{
			if(index == 1)
			{
				std::string temp_str(buffer);
				std::vector< std::string > byte_vec;
				std::string delim(" ");
				split(temp_str,delim,&byte_vec);


				int i = 0;
				for(std::vector<std::string>::iterator iter = byte_vec.begin();
					iter != byte_vec.end(),i<=7;iter++)
				{
					if(iter->find("00") != std::string::npos)
					{
						
					}
					else if(iter->find(" ") == std::string::npos)
					{
						//直接用out_buffer 会造成地址越界，需要temp_interpretor 转换一下
						sscanf(iter->c_str(),"%x",&temp_interpretor[i]);
						i++;
					}

				}
				for(int j = 0;j< 8;j++)
				{
					order_byte[j] = temp_interpretor[j];
				}
				unsigned char log_buff[256]={0};
				sprintf((char *)log_buff,"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
					order_byte[0],order_byte[1],order_byte[2],order_byte[3],
					order_byte[4],order_byte[5],order_byte[6],order_byte[7]);
				
				
				
				out_buffer[0] = order_byte[4];
				out_buffer[1] = order_byte[5];
				out_buffer[2] = order_byte[6];
				out_buffer[3] = order_byte[7];
				out_buffer[4] = order_byte[0];
				out_buffer[5] = order_byte[1];
				out_buffer[6] = order_byte[2];
				out_buffer[7] = order_byte[3];

				memset(log_buff,0,256);
				sprintf((char *)log_buff,"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
					out_buffer[0],out_buffer[1],out_buffer[2],out_buffer[3],
					out_buffer[4],out_buffer[5],out_buffer[6],out_buffer[7]);
				SaveFormattedLog(LOG_RUN_LEVEL,"device uid sequence: %s ",log_buff);
				error_code = 0;
				index++;
				break;
			}
			if(NULL != strstr(buffer,"issuing raw instruction \"4B\" returns 12 bytes as required:"))
			{
				index++;
			}
		}
	}
	_pclose(pipe);
	return error_code;
}


