// EasyWriteDlg.h : 头文件
//

#pragma once

#define IDC_STATUS 5001

class CEasyWriteDlg ;
typedef struct tag_task_thread_para
{
	CEasyWriteDlg *pDlg;
	int            serial_port_index;  // start from 0

}task_thread_para;

// CEasyWriteDlg 对话框
class CEasyWriteDlg : public CDialog
{

private:
	CRITICAL_SECTION m_critical_section;

	int AddSuccessChip();
	int AddFailedChip();
// 构造
public:
	CEasyWriteDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CEasyWriteDlg();
// 对话框数据
	enum { IDD = IDD_EASYWRITE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	


	int AddEnvironmentVariable(char *item,char *value);

	//
	// 1 烧写固件成功
	// 2 烧写加密数据成功
	// 3 烧写固件失败
	//4  烧写加密数据失败
	int SendSerialSignal(int signal);

	int EnumAllComm( );

	int ResetPin();


	int SetSuccess();



	//获取连接电脑的烧录器数目
	int GetBusNumber();

// 实现
protected:

	HANDLE m_listen_thread_handle;
	HANDLE	m_work_thread_handle;

	//串口句柄
	HANDLE m_serial_handle;
	HANDLE m_serial_array_handle[10];

	// 连接的串口数目
	int  m_serial_port_number;
	CStatusBarCtrl     m_StatBar;
	HICON m_hIcon;
	CWnd* m_pDrawWnd;
	TCHAR m_serial_port_array[10][20];

	//锁定窗口的控件元素
	int FreezeWindowComponents();

   //解锁窗口控件元素
	int EnableAllWindowComponents();

	// 读取用户提供的参数，并检查运行参数是否合法，
	// 合法返回0，否则返回1
	int CheckRunPara();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	// 选择执行按钮响应函数
	afx_msg void OnBnTestUid();

	
	int current_status;

public:

	// 选择固件文件按钮响应函数
	afx_msg void OnBnClickedSelect();

public:

	void RefreshChipNo();

	static unsigned int WINAPI ValidationChipArraySysData(void *arg);
	static unsigned int WINAPI ValidationSpecifiedChipSysData(void *arg);

    static unsigned int WINAPI MultiWriteSpecifiedSysData(void *arg);
	static unsigned int WINAPI WriteSpecifiedSysData(void *arg);
	static unsigned int WINAPI WriteSpecifiedEncryptData(void *arg);

	static unsigned int WINAPI WriteSysData(void *arg);
	static unsigned int WINAPI WriteEncryptData(void *arg);


	static unsigned int WINAPI WriteSpecifiedChip(void *arg);

	static unsigned int WINAPI CleanSpecifiedChip(void *arg);
	static unsigned int WINAPI CleanMultiChip(void *arg);

	static unsigned int WINAPI MultiWriteChip(task_thread_para *arg);

	static unsigned int WINAPI TaskDispatcher(void *arg);
	

	int InitialSerialPort();
	int InitialSerialPortArray();


private:
		task_thread_para *m_listen_thread_para_array;

		// 1 表示烧录，0表示擦除芯片, 2表示校验芯片
		int m_task_type;

		//固件保存地址
		char m_sys_data_path[MAX_PATH];

};


