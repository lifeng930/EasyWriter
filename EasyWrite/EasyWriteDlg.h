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
	
	int  ReadBinPath();

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

	int TestSignal();

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
	CString            m_src_file_path;
	CStatusBarCtrl     m_StatBar;
	HICON m_hIcon;
	CWnd* m_pDrawWnd;
	TCHAR m_serial_port_array[10][20];

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCheck();

	afx_msg void OnBnTestUid();

	
	int current_status;
public:
	afx_msg void OnBnClickedEncrypt();
public:
	afx_msg void OnBnClickedSelect();

public:

	void RefreshChipNo();

	static unsigned int WINAPI WriteOneSerialPort(void *arg);

    static unsigned int WINAPI MultiWriteSpecifiedSysData(void *arg);
	static unsigned int WINAPI WriteSpecifiedSysData(void *arg);
	static unsigned int WINAPI WriteSpecifiedEncryptData(void *arg);

	static unsigned int WINAPI WriteSysData(void *arg);
	static unsigned int WINAPI WriteEncryptData(void *arg);

	// 采用电脑单独烧写一个芯片
	static unsigned int WINAPI SingleWriteOneChip(void *arg);

	static unsigned int WINAPI WriteOneChip(void *arg);

	static unsigned int WINAPI WriteSpecifiedChip(void *arg);

	static unsigned int WINAPI MultiWriteChip(task_thread_para *arg);

	static unsigned int WINAPI TaskDispatcher(void *arg);
	
	//监听上位机线程，确认下一个芯片已经安装到位
	static unsigned int WINAPI CheckNewChip(void *arg);

	static unsigned int WINAPI Go2Die(void *arg);

	int InitialSerialPort();
	int InitialSerialPortArray();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedSingleWrite();
};


