// EasyWrite.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEasyWriteApp:
// �йش����ʵ�֣������ EasyWrite.cpp
//

class CEasyWriteApp : public CWinApp
{
public:
	CEasyWriteApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEasyWriteApp theApp;