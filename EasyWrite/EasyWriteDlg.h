// EasyWriteDlg.h : 头文件
//

#pragma once

#define IDC_STATUS 5001

// CEasyWriteDlg 对话框
class CEasyWriteDlg : public CDialog
{
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

//	int SetBusyStatus();

//	int SetIdleStatus();

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
	CString            m_src_file_path;
	CStatusBarCtrl     m_StatBar;
	HICON m_hIcon;
	CWnd* m_pDrawWnd;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	
	afx_msg void OnBnTestUid();

	
	int current_status;
public:
	afx_msg void OnBnClickedEncrypt();
public:
	afx_msg void OnBnClickedSelect();


public:

	void RefreshChipNo();

	static unsigned int WINAPI WriteSysData(void *arg);
	static unsigned int WINAPI WriteEncryptData(void *arg);

	// 采用电脑单独烧写一个芯片
	static unsigned int WINAPI SingleWriteOneChip(void *arg);

	static unsigned int WINAPI WriteOneChip(void *arg);
	
	//监听上位机线程，确认下一个芯片已经安装到位
	static unsigned int WINAPI CheckNewChip(void *arg);



	int InitialSerialPort();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedSingleWrite();
};
