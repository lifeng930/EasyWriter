#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdio.h>



typedef enum app_state
{
	STARTING = 0,
	SUSPENDING,
	STOPPED, 
	INTERRUPT //长时间静音时的中断
}app_state_t;


#define SUCCESSFUL_WRTIE_CHIP 2
#define FAILED_WRITE_CHIP     4

#define BUFFER_SIZE 640
#define MAX_RESULT_LEN 512
#define WEB_CONTENT_MAX_LENGTH 1024
#define HTTP_URL_MAX_LEN 1024
#define LOG_BUFFER_MAX_LEN 1024
#define FILE_NAME_MAX_LEN 1024
#define VERSION_STR_MAX_LEN 512
#define MAX_FILE_PATH_LEN   1024

//最大烧写文件长度 2k
#define MAX_BIN_FILE_LEN   1024 * 2

// 文件头最大长度
#define MAX_HEAD_LEN      12

typedef struct msg_item
{
	char *pBuffer;
	int  nBufferLen;
}msg_item_t;

typedef void(*NotifyFunction)(int state, const char* info, int length);

struct speech_segment {
	unsigned int start;	//人声起始点，sample的位置
	unsigned int end;	//人声终止点，sample的位置
	float segTime;		//这一段人声的时长
	float velocity;		//这一段人声的音强
	speech_segment(int s, int e, float b = 0, float v = 1) :
		start(s),
		end(e),
		segTime(b),
		velocity(v)
	{
	}
};


void DisplayMessage(int count);

#endif