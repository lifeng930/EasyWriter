// EasyWriteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <process.h>
#include <exception> 
#include "EasyWrite.h"
#include "EasyWriteDlg.h"
#include "WriteCommand.h"
#include "common/common.h"
#include "FileHandler.h"
#include "EncryptWrite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static unsigned int g_success_chip = 0;
static unsigned int g_failed_chip = 0;

static unsigned int g_usb_number  = 0; //连接电脑烧录器的数目
 static UINT BASED_CODE indicators[]=   
 //状态栏的指示器列表，如有未定义的字符串名，需在Resource View的String Table中添加定义
{
      ID_INDICATOR_CAPS,
     ID_INDICATOR_NUM,
  //需在Resource View的String Table中添加定义
   };
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEasyWriteDlg 对话框




CEasyWriteDlg::CEasyWriteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEasyWriteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEasyWriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEasyWriteDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CEasyWriteDlg::OnBnTestUid)
	ON_BN_CLICKED(IDC_ENCRYPT, &CEasyWriteDlg::OnBnClickedEncrypt)
	ON_BN_CLICKED(IDC_SELECT, &CEasyWriteDlg::OnBnClickedSelect)

ON_BN_CLICKED(IDC_BUTTON2, &CEasyWriteDlg::OnBnClickedButton2)

ON_BN_CLICKED(IDC_SINGLE_WRITE, &CEasyWriteDlg::OnBnClickedSingleWrite)
END_MESSAGE_MAP()

#if 0
int CEasyWriteDlg::SetIdleStatus()
{
	
	DWORD dwFunc = CLRDTR;
	if( !EscapeCommFunction(m_serial_handle,dwFunc))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"failed Clears the DTR (data-terminal-ready) signal: %d.",GetLastError());
		return 1;
	}
	SaveFormattedLog(LOG_RUN_LEVEL,"success Clears the DTR (data-terminal-ready) signal.");
	return 0;
}
#endif

int CEasyWriteDlg::SetSuccess()
{
    DWORD dwFunc = CLRRTS;
	if(!EscapeCommFunction(m_serial_handle,dwFunc))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"failed clear RTS :success  signal");
		return 0;
	}
	SaveFormattedLog(LOG_RUN_LEVEL,"success clear the RTS: success  signal: %d",GetLastError());
	return 1;
}

#if 0
int CEasyWriteDlg::SetBusyStatus()
{
	/*  默认失败位清除 */
	DWORD dwFunc = SETRTS;
	if( !EscapeCommFunction(m_serial_handle,dwFunc))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"s set RTS (failed signal) signal: %d.",GetLastError());
		return 1;
	}
	SaveFormattedLog(LOG_RUN_LEVEL,"success set RTS (failed signal) signal: %d",GetLastError());


	dwFunc = SETDTR;
	if( !EscapeCommFunction(m_serial_handle,dwFunc))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"failed SET the DTR (data-terminal-ready) signal: %d.",GetLastError());
		return 1;
	}
	SaveFormattedLog(LOG_RUN_LEVEL,"success SET the DTR (data-terminal-ready) signal.");
	return 0;
}
#endif

//// CEasyWriteDlg 消息处理程序
BOOL CEasyWriteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	RECT     m_Rect;   
    GetClientRect(&m_Rect);         //获取对话框的矩形区域
    m_Rect.top=m_Rect.bottom-160;    //设置状态栏的矩形区域

	m_StatBar.CreateEx(WS_EX_LEFT,SBT_TOOLTIPS,m_Rect,this,IDC_STATUS);
    m_StatBar.ShowWindow(SW_SHOW);

	m_pDrawWnd = GetDlgItem(IDC_STATIC);//获得静态窗口对象指针
	

    int strPartDim[4]= {200, 400, 600, -1};
    m_StatBar.SetParts(4, strPartDim);
 
  //  m_StatBar.SetText(_T("EasyWrite "),0,0);
    m_StatBar.SetText(_T("huanglifeng@unisound.com"), 1, 0);
	
    g_success_chip = 0;
	g_failed_chip = 0;
	TCHAR temp[100] = {0};
	memset(temp,0,100* sizeof(TCHAR));
	_sntprintf(temp,99,_T("烧写成功芯片数：%d,失败数:%d"),g_success_chip,g_failed_chip);
    m_StatBar.SetText(temp, 2, 0);

   //memset(temp,0,100* sizeof(TCHAR));
   //_sntprintf(temp,99,_T("烧写成功芯片数：%d"),g_failed_chip);
   //m_StatBar.SetText(temp, 3, 0);

   g_usb_number = GetBusNumber();
   memset(temp,0,100* sizeof(TCHAR));
   _sntprintf(temp,99,_T("连接烧录器数目：%d"),g_usb_number);
   m_StatBar.SetText(temp, 0, 0);

	//设置编辑框字体
	CFont *editor_font = new CFont; 
	editor_font->CreateFont(30,15,0,0,100,
	FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,_T("Arial"));
	CEdit *editor = (CEdit*) GetDlgItem(IDC_PATH);
	editor->SetFont(editor_font);
	CButton *button = (CButton*)GetDlgItem(IDC_SELECT);
	button->SetFont(editor_font);

	button = (CButton*)GetDlgItem(IDC_BUTTON1);
	button->SetFont(editor_font);



	CComboBox *box = (CComboBox *)GetDlgItem(IDC_COM_SET);
	box->SetFont(editor_font);
	
	CStatic* pstatic = (CStatic*)GetDlgItem(IDC_STATIC);
	pstatic->SetFont(editor_font);

	pstatic = (CStatic*)GetDlgItem(IDC_STATIC2);
	pstatic->SetFont(editor_font);
    EnumAllComm();
	m_serial_handle = INVALID_HANDLE_VALUE;
    m_listen_thread_handle = NULL;
	m_work_thread_handle = NULL;



	HWND my_hWnd=::FindWindow(NULL,_T("EasyWrite")); 
	//设置窗口总在最前
	::SetWindowPos(my_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEasyWriteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}



unsigned int WINAPI CEasyWriteDlg::SingleWriteOneChip(void *arg)
{

	SaveFormattedLog(LOG_RUN_LEVEL, "WriteOneChip");
	CEasyWriteDlg *pDlg = (CEasyWriteDlg*)arg;
	int error_code = 1;
	char command[512] = { 0 };
	CEdit* pEdit = (CEdit*)pDlg->GetDlgItem(IDC_PATH);
	pEdit->GetWindowTextW(pDlg->m_src_file_path);
	int len = WideCharToMultiByte(CP_ACP, 0, pDlg->m_src_file_path, -1, NULL, 0, NULL, NULL);
	char * temp_str = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, pDlg->m_src_file_path, -1, temp_str, len, NULL, NULL);
	CFileHandler handler(temp_str);

	memset(command, 0, 512);
	sprintf(command, "dpcmd -p %s -a 0x0", temp_str);
	delete[]temp_str;
	pDlg->m_src_file_path.ReleaseBuffer();
	
	pDlg->m_StatBar.SetText(TEXT("当前状态：正在擦除芯片"),3,SBT_NOBORDERS);
	error_code = system("dpcmd -e");
	if(error_code != 0)
	{
	SaveFormattedLog(LOG_RUN_LEVEL,"dpcmd -e error: %d",error_code);
	pDlg->SendSerialSignal(FAILED_WRITE_CHIP);
	pDlg->m_StatBar.SetText(TEXT("当前状态：擦除芯片失败"),3,SBT_NOBORDERS);

	return 1;
	}
	
	pDlg->m_StatBar.SetText(TEXT("当前状态：正在烧写固件"), 3, SBT_NOBORDERS);

	Sleep(200);


	error_code = system(command);
	if (error_code != 0)
	{
		SaveFormattedLog(LOG_RUN_LEVEL, "%s error!", command);
		pDlg->m_StatBar.SetText(TEXT("当前状态：烧写固件失败"), 3, SBT_NOBORDERS);
		pDlg->SendSerialSignal(FAILED_WRITE_CHIP);
		return 1;
	}

#if 0
	pDlg->m_StatBar.SetText(TEXT("当前状态：正在写加密数据"), 3, SBT_NOBORDERS);

	CEncryptWrite writer;
	error_code = writer.GenerateFile();
	if (0 != error_code)
	{

		pDlg->m_StatBar.SetText(TEXT("当前状态：生成加密数据失败"), 3, SBT_NOBORDERS);
		pDlg->SendSerialSignal(FAILED_WRITE_CHIP);

		return 1;
	}
	error_code = writer.WriteEncrypy();
	if (error_code != 0)
	{
		pDlg->SendSerialSignal(FAILED_WRITE_CHIP);
		pDlg->m_StatBar.SetText(TEXT("当前状态：烧写加密数据失败"), 3, SBT_NOBORDERS);
		SaveFormattedLog(LOG_RUN_LEVEL, "write encrypt data failed!");
	}
	else
	{
		pDlg->SendSerialSignal(SUCCESSFUL_WRTIE_CHIP);
		pDlg->m_StatBar.SetText(TEXT("当前状态：烧写加密数据成功！"), 3, SBT_NOBORDERS);
		SaveFormattedLog(LOG_RUN_LEVEL, "write encrypt data success!");
	}
#endif
	return 0;
}

void AlarmVoice()
{
	for(int i = 0;i < 10;i++)
	{
     MessageBeep(MB_ICONERROR);
	 Sleep(500);
	}
}

unsigned int WINAPI CEasyWriteDlg::WriteSysData(void *arg)
{
	SaveFormattedLog(LOG_RUN_LEVEL,"WriteOneChip");
	CEasyWriteDlg *pDlg = (CEasyWriteDlg*)arg;
	int error_code = 1;
	CEdit* pEdit = (CEdit*)pDlg->GetDlgItem(IDC_PATH);
	pEdit->GetWindowTextW(pDlg->m_src_file_path);
	int len =WideCharToMultiByte(CP_ACP,0,pDlg->m_src_file_path,-1,NULL,0,NULL,NULL);  
	char * temp_str = new char[len + 1];
	WideCharToMultiByte(CP_ACP,0,pDlg->m_src_file_path,-1,temp_str,len,NULL,NULL ); 	
	CFileHandler handler(temp_str);

	pDlg->m_StatBar.SetText(TEXT("当前状态：正在烧写固件"),3,SBT_NOBORDERS); 

	CWriteCommand excuter(handler);
	error_code = excuter.ProgramSysData(0);
	delete []temp_str;
	pDlg->m_src_file_path.ReleaseBuffer(); 	
  
	if(error_code != 0)
	{
		TCHAR temp_disp[100] = {0};
		_stprintf(temp_disp,L"当前状态：烧写固件失败:%d",error_code);
		 pDlg->m_StatBar.SetText(temp_disp/*TEXT("当前状态：烧写固件失败")*/,3,SBT_NOBORDERS);
		 pDlg->SendSerialSignal(FAILED_WRITE_CHIP);
	     AlarmVoice();
		 return 1;
	}
	pDlg->m_StatBar.SetText(TEXT("当前状态：烧写固件成功"),3,SBT_NOBORDERS);
	return 0;
}

unsigned int WINAPI CEasyWriteDlg::WriteEncryptData(void *arg)
{
	CEasyWriteDlg *pDlg = (CEasyWriteDlg*)arg;
	int error_code = 1;

		pDlg->m_StatBar.SetText(TEXT("当前状态：正在写加密数据"),3,SBT_NOBORDERS); 

	CEncryptWrite writer;
	error_code = writer.GenerateFile();
	if(0 != error_code)
	{
	
		pDlg->m_StatBar.SetText(TEXT("当前状态：生成加密数据失败"),3,SBT_NOBORDERS);
		pDlg->SendSerialSignal(FAILED_WRITE_CHIP);
		AlarmVoice();
		return 1;
	}
	error_code = writer.WriteEncrypy();
	if(error_code != 0)
	{
		pDlg->SendSerialSignal(FAILED_WRITE_CHIP);
		pDlg->m_StatBar.SetText(TEXT("当前状态：烧写加密数据失败"),3,SBT_NOBORDERS);
		SaveFormattedLog(LOG_RUN_LEVEL,"write encrypt data failed");
		AlarmVoice();
	}
	else
	{
		pDlg->SendSerialSignal(SUCCESSFUL_WRTIE_CHIP);
		pDlg->m_StatBar.SetText(TEXT("当前状态：烧写加密数据成功！"),3,SBT_NOBORDERS);
		SaveFormattedLog(LOG_RUN_LEVEL,"write encrypt data  success");
	}
	return 0;
}
#define MAX_WRITE_CHIP_TIME  25000

unsigned int WINAPI CEasyWriteDlg:: WriteOneChip(void *arg)
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD exit_code = 1;
	CEasyWriteDlg *pDlg = (CEasyWriteDlg*)arg;
    HANDLE write_sys_thread_handle = (HANDLE)_beginthreadex(NULL, 0, CEasyWriteDlg::WriteSysData, pDlg, 0,NULL);	
	if(NULL == write_sys_thread_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Create write sys thread failed!");
		g_failed_chip++;
		pDlg->RefreshChipNo();
		return 1;
	}
	if(WAIT_TIMEOUT  == WaitForSingleObject(write_sys_thread_handle ,MAX_WRITE_CHIP_TIME))
	{
		TerminateThread(write_sys_thread_handle ,2);	
		SaveFormattedLog(LOG_RUN_LEVEL,"Write sys data timeout!");
		g_failed_chip++;
		pDlg->RefreshChipNo();
		return 2;
	}
	GetExitCodeThread(write_sys_thread_handle,&exit_code);
	CloseHandle(write_sys_thread_handle);
	if(exit_code != 0)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Write sys data failed,early exit!");
		g_failed_chip++;
		pDlg->RefreshChipNo();
		return 1;
	}

	HANDLE write_encrypt_thread_handle = (HANDLE)_beginthreadex(NULL, 0, CEasyWriteDlg::WriteEncryptData, pDlg, 0,NULL);	
	if(NULL == write_encrypt_thread_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Create write encrypt thread failed!");
		g_failed_chip++;
		pDlg->RefreshChipNo();
		return 1;
	}
	if(WAIT_TIMEOUT  == WaitForSingleObject(write_encrypt_thread_handle,MAX_WRITE_CHIP_TIME))
	{
		TerminateThread(write_encrypt_thread_handle,2);	
		SaveFormattedLog(LOG_RUN_LEVEL,"Write encrypt data timeout!");
		g_failed_chip++;
		pDlg->RefreshChipNo();
		return 2;
	}
	GetExitCodeThread(write_encrypt_thread_handle,&exit_code);
	if(0 != exit_code)
	{
		g_failed_chip++;
		pDlg->RefreshChipNo();
	}
	CloseHandle(write_encrypt_thread_handle);

	g_success_chip++;
    pDlg->RefreshChipNo();
	return 0;
}

void CEasyWriteDlg::RefreshChipNo()
{
	TCHAR temp[100] = {0};
	memset(temp,0,100* sizeof(TCHAR));
	_sntprintf(temp,99,_T("烧写成功芯片数：%d,失败数:%d"),g_success_chip,g_failed_chip);
    m_StatBar.SetText(temp, 2, 0);


   SaveFormattedLog(LOG_RUN_LEVEL,"Already success write chip: %d,failed write chip: %d ",g_success_chip,g_failed_chip);
}

unsigned int WINAPI CEasyWriteDlg::CheckNewChip(void *arg)
{
	DWORD temp;
	int count = 0;
	CEasyWriteDlg *pDlg = (CEasyWriteDlg *)arg;
	DWORD mask = EV_RING;
	OVERLAPPED os;  
	memset(&os,0,sizeof(OVERLAPPED));  
    os.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);  
	os.Internal = 0;
    os.InternalHigh = 0;
    os.Offset = 0;
    os.OffsetHigh = 0;
	
    if(os.hEvent==NULL)  
    {  
        SaveFormattedLog(LOG_RUN_LEVEL,"create event for CheckNewChip failed"); 
        return 1;
    } 
	while(1)
	{	
		BOOL ret =  WaitCommEvent(pDlg->m_serial_handle,&mask,&os);
		if(ret == TRUE)
		{
			if(mask & EV_RING)
			{
				count++;
				if(2 == count)
				{
					WriteOneChip(pDlg);
					count = 0;
				}
			}
		}
		else
		{
			if(GetLastError()==ERROR_IO_PENDING)  
            {  
                GetOverlappedResult(pDlg->m_serial_handle,&os,&temp,TRUE);   
				SaveFormattedLog(LOG_RUN_LEVEL,"pending"); 
				WaitForSingleObject(os.hEvent,INFINITE);
				if(mask & EV_RING)
				{
					count++;
					if(2 == count)
					{
						
						WriteOneChip(pDlg);
						count = 0;
					}
				}
            } 
		}
		Sleep(50);

	}
	return 0;
}

int  CEasyWriteDlg::ReadBinPath()
{
	int error_code = 1;
	CEdit *editor = (CEdit*) GetDlgItem(IDC_PATH);
	TCHAR path_t[FILE_NAME_MAX_LEN] = { 0 };
	char path[FILE_NAME_MAX_LEN] = { 0 };
	GetPrivateProfileString(_T("options"), _T("bin_path"), _T(".\\bin\\*.*"), path_t, sizeof(path_t), _T(".\\writer.ini"));
	Tchar2Char(path_t, path);
	if(NULL == strstr(path,".\\bin\\*.*"))
	{
		editor->SetWindowTextW(path_t);
		return 0;
	}

	WIN32_FIND_DATA  file_info;
	HANDLE  search_handle = FindFirstFile(_T(".\\bin\\*.*"),&file_info);
	if(INVALID_HANDLE_VALUE == search_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"the out directory no file!");
		return error_code;
	}

	while(1)
	{
		memset(path,0,FILE_NAME_MAX_LEN);
		Tchar2Char(file_info.cFileName,path);
		SaveFormattedLog(LOG_RUN_LEVEL,"search path: %s ",path);
		
		if(!strcmp(path,".") ||
			!strcmp(path,".."))
		{
			int error_code = FindNextFile(search_handle,&file_info);
			if(0 == error_code)
			{
				SaveFormattedLog(LOG_RUN_LEVEL,"the out directory does not exist");
				break;
			}

		}
		else
		{
			//找到第一个文件即可	
			char file_full_path[FILE_NAME_MAX_LEN] = {0};
			
			TCHAR ch_buf[FILE_NAME_MAX_LEN];
			ZeroMemory(ch_buf,FILE_NAME_MAX_LEN);
			
			if(0 == GetModuleFileName(NULL,ch_buf,FILE_NAME_MAX_LEN))
			{
				SaveFormattedLog(LOG_RUN_LEVEL,"Get current path failed!");
				return error_code;
			}

			/* 向前查找找到截止符 / */
			
			int len = _tcslen(ch_buf);
			int pre_path_len = 0;
			TCHAR *file_path_end = ch_buf + len;
			while(file_path_end-- > ch_buf)
			{
				if(*file_path_end == L'\\')
				{
					pre_path_len = file_path_end - ch_buf;
					SaveFormattedLog(LOG_RUN_LEVEL,"pre index: %d ",pre_path_len);
					break;
				}
			}
			SaveFormattedLog(LOG_RUN_LEVEL,"the path length: %d",len);
			TCHAR *pos = ch_buf + pre_path_len + 1;
			//加上文件夹bin
			TCHAR dir[10] = _T("bin\\");
			_tcscpy(pos,dir);
			int new_len = _tcslen(dir);
			pos += new_len;
			_tcscpy(pos,file_info.cFileName);
			
			editor->SetWindowTextW(ch_buf);
			Tchar2Char(ch_buf,file_full_path);
			SaveFormattedLog(LOG_RUN_LEVEL,"the source file %s",file_full_path);
			error_code = 0;
			break;
		}	
	}	
	return error_code;
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEasyWriteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		CDialog::OnPaint();
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CEasyWriteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CEasyWriteDlg::GetBusNumber()
{
	int index = 0;
	FILE *pipe = NULL;
	char command_str[256] = {0};
	strcpy(command_str,"dpcmd --list-device-id 0");
	char buffer[1024] = {0};
	try
	{
		pipe = _popen(command_str,"r");
		while(!feof(pipe))
		{
			if(fgets(buffer,1024,pipe))
			{
				std::string temp_str(buffer);
				if(temp_str.find_first_of(",") != std::string::npos)
				{
					index++;
				}
			}
		}
	}
	catch(...)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"unknown exception for get usb number!");
		return 0;
	}
    _pclose(pipe);
	return index;	
}

void CEasyWriteDlg::OnBnTestUid()
{
	if(m_serial_handle == INVALID_HANDLE_VALUE)
	{
		if(InitialSerialPort())
		{
			SaveFormattedLog(LOG_RUN_LEVEL,"Initial serial port failed!");
			MessageBox(_T("初始化串口失败"),_T("警告"),MB_OK);
			return ;
		}
	}

	m_listen_thread_handle = (HANDLE)_beginthreadex(NULL, 0, CEasyWriteDlg::CheckNewChip, this, 0,NULL);	
	if(NULL == m_listen_thread_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Create check thread failed!");
	}	
	CloseHandle(m_listen_thread_handle);
	m_listen_thread_handle = NULL;
	

	m_work_thread_handle =  (HANDLE)_beginthreadex(NULL, 0, CEasyWriteDlg::WriteOneChip, this, 0,NULL);
	if(NULL == m_work_thread_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"Create work thread failed!");
	}
	CloseHandle(m_work_thread_handle);
	m_work_thread_handle = NULL;
	SaveFormattedLog(LOG_RUN_LEVEL,"Create two  threads success!");

	CButton* pButton = (CButton *)GetDlgItem(IDC_BUTTON1);
	pButton->SetWindowTextW(L"正在烧写");
	pButton->EnableWindow(FALSE);

}



void CEasyWriteDlg::OnBnClickedEncrypt()
{
    if(m_serial_handle == INVALID_HANDLE_VALUE)
	{
		if(InitialSerialPort())
		{
			SaveFormattedLog(LOG_RUN_LEVEL,"Initial serial port failed!");
			MessageBox(_T("初始化串口失败"),_T("警告"),MB_OK);
			return ;
		}		
	}
    ResetPin();
	// TODO: 在此添加控件通知处理程序代码
	int error_code = 1;
	char command[512] = {0};
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PATH);
	pEdit->GetWindowTextW(m_src_file_path);
	int len =WideCharToMultiByte(CP_ACP,0,m_src_file_path,-1,NULL,0,NULL,NULL);  
	char * temp_str = new char[len + 1];
	WideCharToMultiByte(CP_ACP,0,m_src_file_path,-1,temp_str,len,NULL,NULL ); 	
	CFileHandler handler(temp_str);

	memset(command,0,512);
	sprintf(command,"dpcmd -p %s -a 0x0",temp_str);
	delete []temp_str;
	m_src_file_path.ReleaseBuffer(); 
    m_StatBar.SetText(TEXT("当前状态：正在擦除芯片"),3,SBT_NOBORDERS); 
	error_code = system("dpcmd -e");
	if(error_code != 0)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"dpcmd -e error!;");
		SendSerialSignal(FAILED_WRITE_CHIP);
		 m_StatBar.SetText(TEXT("当前状态：擦除芯片失败"),3,SBT_NOBORDERS); 
		return;
	}
    m_StatBar.SetText(TEXT("当前状态：正在烧写固件"),3,SBT_NOBORDERS); 

	Sleep(200);
	UpdateWindow();
	error_code = system(command);
	if(error_code != 0)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"%s error!",command);
		 m_StatBar.SetText(TEXT("当前状态：烧写固件失败"),3,SBT_NOBORDERS);
		 SendSerialSignal(FAILED_WRITE_CHIP);
	
		return;
	}

	m_StatBar.SetText(TEXT("当前状态：正在写加密数据"),3,SBT_NOBORDERS); 
	UpdateWindow();
	Sleep(300);
	SaveFormattedLog(LOG_RUN_LEVEL,"begin CEncryptWrite writer;");
	CEncryptWrite writer;
	

	error_code = writer.GenerateFile();
	if(0 != error_code)
	{
		MessageBox(TEXT("Encrypt Data failed!"),TEXT("ERROR"),MB_OK);
		m_StatBar.SetText(TEXT("当前状态：生成加密数据失败"),3,SBT_NOBORDERS);
		SendSerialSignal(FAILED_WRITE_CHIP);
		return;
	}
	
	error_code = writer.WriteEncrypy();
	if(error_code != 0)
	{
		SendSerialSignal(FAILED_WRITE_CHIP);
		m_StatBar.SetText(TEXT("当前状态：烧写加密数据失败"),3,SBT_NOBORDERS);
	}
	else
	{
		SendSerialSignal(SUCCESSFUL_WRTIE_CHIP);
		m_StatBar.SetText(TEXT("当前状态：烧写加密数据成功！"),3,SBT_NOBORDERS);
	}
}

int CEasyWriteDlg::EnumAllComm(  )
{
    HKEY hkey;  
    int result;
    int i = 0;
 
   
	char temp[200] ={0};
	TCHAR buf[200] = {0};
 
    result = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                            _T( "Hardware\\DeviceMap\\SerialComm" ),
                            NULL,
                            KEY_READ,
                            &hkey );
 
    if( ERROR_SUCCESS == result )   //   打开串口注册表   
    {   
        TCHAR portName[ 0x100 ], commName[ 0x100 ];
        DWORD dwLong, dwSize;
 
        do
        {   
            dwSize = sizeof( portName ) / sizeof( TCHAR );
            dwLong = dwSize;
            result = RegEnumValue( hkey, i, portName, &dwLong, NULL, NULL, ( LPBYTE )commName, &dwSize );
            if( ERROR_NO_MORE_ITEMS == result )
            {
                //   枚举串口
                break;   //   commName就是串口名字
            }
            _tcscpy( buf, commName );

			CComboBox *box = (CComboBox *)GetDlgItem(IDC_COM_SET);
			box->AddString(buf);
			box->SetCurSel(0);
			UpdateData(FALSE);
			TCHAR *pos = buf;
            pos += ( _tcslen( buf ) + 1 );
            i++;   
        } while ( 1 );
 
        RegCloseKey( hkey );   
    }  
 
    *buf = 0;
 
    return i;
}

int CEasyWriteDlg::ResetPin()
{

	DCB serial_opt;
	GetCommState(m_serial_handle,&serial_opt);
	serial_opt.BaudRate = CBR_9600;
	serial_opt.DCBlength = sizeof(DCB);
	serial_opt.fParity = 1; //允许奇偶校验
	serial_opt.ByteSize = 8; //通信字节位数
	serial_opt.StopBits = ONESTOPBIT; //停止位
	serial_opt.fRtsControl = RTS_CONTROL_ENABLE;
	serial_opt.fDtrControl = DTR_CONTROL_ENABLE;
	
	if(0 == SetCommState(m_serial_handle,&serial_opt))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"SetCommState failed: %d",GetLastError());
		return 1;
	}
	return 0;
}

int CEasyWriteDlg::InitialSerialPort()
{
    

	TCHAR port_name[50] ={0};
	CComboBox *box = (CComboBox *)GetDlgItem(IDC_COM_SET);
	int index = box->GetCurSel();
	box->GetLBText(index,port_name);
	char temp_name[200] = {0};
	Tchar2Char(port_name,temp_name);

	m_serial_handle =  CreateFile(
	port_name,
	GENERIC_WRITE , //只允许写
	FILE_SHARE_WRITE,           //写方式
	NULL,       //安全属性
	OPEN_EXISTING, //新创建,
	FILE_FLAG_OVERLAPPED, //采用重叠IO方式
	NULL);
	if(m_serial_handle == INVALID_HANDLE_VALUE)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"open serial port %s error:%d",temp_name,GetLastError());
		return 1;
	}

	// 配置选项
	DCB serial_opt;
	GetCommState(m_serial_handle,&serial_opt);
	serial_opt.BaudRate = CBR_9600;
	serial_opt.DCBlength = sizeof(DCB);
	serial_opt.fParity = 1; //允许奇偶校验
	serial_opt.ByteSize = 8; //通信字节位数
	serial_opt.StopBits = ONESTOPBIT; //停止位
	serial_opt.fRtsControl = RTS_CONTROL_ENABLE;
	serial_opt.fDtrControl = DTR_CONTROL_ENABLE;
	
	if(0 == SetCommState(m_serial_handle,&serial_opt))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"SetCommState failed: %d",GetLastError());
		return 1;
	}

	if (!SetCommMask(m_serial_handle, EV_RING | EV_ERR))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"SetCommMask failed:%d",GetLastError());
		return 1;
	}

	DWORD dwFunc = SETDTR;

	if(EscapeCommFunction(m_serial_handle,dwFunc))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"success to send faild signal");
	}
	else
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"failed to send faild signal");
	}

	
	DWORD dwInQueue = 128;
    DWORD dwOutQueue = 128;

	if(0 == SetupComm(m_serial_handle,dwInQueue,dwOutQueue))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"SetupComm failed: %d",GetLastError());
	}
	SaveFormattedLog(LOG_RUN_LEVEL,"Initial serial port success,m_serial_handle: %d",m_serial_handle);
	return 0;
}

// 2 烧写加密数据成功哦你
//4  烧写加密数据失败
int CEasyWriteDlg::SendSerialSignal(int signal)
{
	 if(signal == SUCCESSFUL_WRTIE_CHIP)
	{
		/*
		OVERLAPPED  writeOS;
		memset(&writeOS,0,sizeof(OVERLAPPED)); 
		writeOS.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
		char content[1800] = {0};
		DWORD dwBytesWrite=1800;
		DWORD dwTranslate = 0;
		BOOL bWriteStat = WriteFile(m_serial_handle,&content,dwBytesWrite,&dwBytesWrite,&writeOS);
		if(!bWriteStat)
		{
			if(GetLastError() == ERROR_IO_PENDING)   
			{
				GetOverlappedResult(m_serial_handle,&writeOS,&dwTranslate,TRUE);
			}
			SaveFormattedLog(LOG_RUN_LEVEL,"write serial port failed: %d,m_serial_handle: %d!",GetLastError(),m_serial_handle);
			return 1;
		}
		CloseHandle(writeOS.hEvent);
		SaveFormattedLog(LOG_RUN_LEVEL,"success low voltage TX pin ,SUCCESSFUL_WRTIE_CHIP ,");
	*/
		SetSuccess();
	 }

	else if(signal == FAILED_WRITE_CHIP)
	{
		DWORD dwFunc = CLRDTR;

		if(EscapeCommFunction(m_serial_handle,dwFunc))
		{
			SaveFormattedLog(LOG_RUN_LEVEL,"success to send faild signal");
		}
		else
		{
			SaveFormattedLog(LOG_RUN_LEVEL,"failed to send faild signal");
		}

		SaveFormattedLog(LOG_RUN_LEVEL,"FAILED_WRITE_CHIP");
	}

	return 0;

}

CEasyWriteDlg::~CEasyWriteDlg()
{
	CloseHandle(m_serial_handle);
	m_serial_handle = INVALID_HANDLE_VALUE;
	if(NULL != m_work_thread_handle)
	{
	//	TerminateThread(m_work_thread_handle,1);
		CloseHandle(m_work_thread_handle);
		m_work_thread_handle = NULL;
	}

}


int CEasyWriteDlg::AddEnvironmentVariable(char *item,char *value)
{
	TCHAR item_t[512] = {0};
	TCHAR value_t[512] = {0};
	Char2Tchar(item,item_t);
	Char2Tchar(value,value_t);
	int error_code = SetEnvironmentVariable(item_t,value_t);
	if(0!= error_code)
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"SetEnvironmentVariable return code: %d",error_code);
	}
	SaveFormattedLog(LOG_RUN_LEVEL,"set environment variable %s : %s",item,value);
	int retrive_len = 0;
	TCHAR variable_buffer[2000] = {0};
	char temp[4500] = {0};
	if(0 != (retrive_len= GetEnvironmentVariable(_T("path"),variable_buffer,2000)))
	{
		
		Tchar2Char(variable_buffer,temp);
		SaveFormattedLog(LOG_RUN_LEVEL,"the path value length: %d",retrive_len);
		SaveFormattedLog(LOG_RUN_LEVEL,"the path value: %s",temp);
	}
	else
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"call GetEnvironmentVariable error: %d ",GetLastError());		
	}
	int len = strlen(temp);
	
	if(len < 3900)
	{
		char *pos = temp + len;
		strcpy(pos,";C:\\Program Files\\DediProg\\SF1003");
		SaveFormattedLog(LOG_RUN_LEVEL,"new environment: %s",temp);
		len = strlen(";C:\\Program Files\\DediProg\\SF1003");
		SaveFormattedLog(LOG_RUN_LEVEL,"Append environment len: %d",len);
		len = strlen(temp);
		SaveFormattedLog(LOG_RUN_LEVEL,"hahahaha new environment len: %d",len);
		TCHAR temp_t[4000];
		Char2Tchar(temp,temp_t);
		error_code = SetEnvironmentVariable(_T("path"),temp_t);
		if(0 == error_code)
		{
			SaveFormattedLog(LOG_RUN_LEVEL,"call SetEnvironmentVariable: %d",GetLastError());			
		}


	}
	if(0 == system("dpcmd -d"))
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"set environment variable success");
	}
	else
	{
		SaveFormattedLog(LOG_RUN_LEVEL,"set environment variable failed");
	}
	return error_code;
}

void CEasyWriteDlg::OnBnClickedSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile = _T("");

    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.sfs)|*.sfs|All Files (*.*)|*.*||"), NULL);
	
    if (dlgFile.DoModal())
    {
        strFile = dlgFile.GetPathName();
    }
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_PATH);
	pEdit->SetWindowTextW(strFile);
	m_src_file_path = strFile;

}




int CEasyWriteDlg::TestSignal()
{
	SaveFormattedLog(LOG_RUN_LEVEL,"write serial port ");
	OVERLAPPED  writeOS;
	memset(&writeOS,0,sizeof(OVERLAPPED)); 
	writeOS.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	char content[1800] = {0};
	DWORD dwBytesWrite=1800;
	DWORD dwTranslate = 0;
	BOOL bWriteStat = WriteFile(m_serial_handle,&content,dwBytesWrite,&dwBytesWrite,&writeOS);
	if(!bWriteStat)
	{
		if(GetLastError() == ERROR_IO_PENDING)   
		{
			WaitForSingleObject(writeOS.hEvent ,INFINITE);
	//		GetOverlappedResult(m_serial_handle,&writeOS,&dwTranslate,TRUE);
		}
		SaveFormattedLog(LOG_RUN_LEVEL,"write serial port failed: %d,m_serial_handle: %d!",GetLastError(),m_serial_handle);
		return 1;
	}
	CloseHandle(writeOS.hEvent);
	SaveFormattedLog(LOG_RUN_LEVEL,"success low voltage TX pin ,SUCCESSFUL_WRTIE_CHIP ,");
	return 0;
}


void CEasyWriteDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	HANDLE temp_handle = (HANDLE)_beginthreadex(NULL, 0, CEasyWriteDlg::SingleWriteOneChip, this, 0, NULL);
	if (NULL == temp_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL, "Create work thread failed!");
	}
	CloseHandle(temp_handle);
	temp_handle = NULL;

}




void CEasyWriteDlg::OnBnClickedSingleWrite()
{
	// TODO:  在此添加控件通知处理程序代码
	HANDLE temp_handle = (HANDLE)_beginthreadex(NULL, 0, CEasyWriteDlg::SingleWriteOneChip, this, 0, NULL);
	if (NULL == temp_handle)
	{
		SaveFormattedLog(LOG_RUN_LEVEL, "Create work thread failed!");
	}
	CloseHandle(temp_handle);
	temp_handle = NULL;
}



