// EasyWriteDlg.h : ͷ�ļ�
//

#pragma once

#define IDC_STATUS 5001

class CEasyWriteDlg ;
typedef struct tag_task_thread_para
{
	CEasyWriteDlg *pDlg;
	int            serial_port_index;  // start from 0

}task_thread_para;

// CEasyWriteDlg �Ի���
class CEasyWriteDlg : public CDialog
{

private:
	CRITICAL_SECTION m_critical_section;

	int AddSuccessChip();
	int AddFailedChip();
// ����
public:
	CEasyWriteDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CEasyWriteDlg();
// �Ի�������
	enum { IDD = IDD_EASYWRITE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	int  ReadBinPath();

	int AddEnvironmentVariable(char *item,char *value);

	//
	// 1 ��д�̼��ɹ�
	// 2 ��д�������ݳɹ�
	// 3 ��д�̼�ʧ��
	//4  ��д��������ʧ��
	int SendSerialSignal(int signal);

	int EnumAllComm( );

	int ResetPin();


	int SetSuccess();

	int TestSignal();

	//��ȡ���ӵ��Ե���¼����Ŀ
	int GetBusNumber();

// ʵ��
protected:

	HANDLE m_listen_thread_handle;
	HANDLE	m_work_thread_handle;
	//���ھ��
	HANDLE m_serial_handle;
	HANDLE m_serial_array_handle[10];

	// ���ӵĴ�����Ŀ
	int  m_serial_port_number;
	CString            m_src_file_path;
	CStatusBarCtrl     m_StatBar;
	HICON m_hIcon;
	CWnd* m_pDrawWnd;
	TCHAR m_serial_port_array[10][20];

	// ���ɵ���Ϣӳ�亯��
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

	// ���õ��Ե�����дһ��оƬ
	static unsigned int WINAPI SingleWriteOneChip(void *arg);

	static unsigned int WINAPI WriteOneChip(void *arg);

	static unsigned int WINAPI WriteSpecifiedChip(void *arg);

	static unsigned int WINAPI MultiWriteChip(task_thread_para *arg);

	static unsigned int WINAPI TaskDispatcher(void *arg);
	
	//������λ���̣߳�ȷ����һ��оƬ�Ѿ���װ��λ
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


