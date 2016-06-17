#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdio.h>



typedef enum app_state
{
	STARTING = 0,
	SUSPENDING,
	STOPPED, 
	INTERRUPT //��ʱ�侲��ʱ���ж�
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

//�����д�ļ����� 2k
#define MAX_BIN_FILE_LEN   1024 * 2

// �ļ�ͷ��󳤶�
#define MAX_HEAD_LEN      12

typedef struct msg_item
{
	char *pBuffer;
	int  nBufferLen;
}msg_item_t;

typedef void(*NotifyFunction)(int state, const char* info, int length);

struct speech_segment {
	unsigned int start;	//������ʼ�㣬sample��λ��
	unsigned int end;	//������ֹ�㣬sample��λ��
	float segTime;		//��һ��������ʱ��
	float velocity;		//��һ����������ǿ
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