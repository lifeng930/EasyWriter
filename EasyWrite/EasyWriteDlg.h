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
	CStatusBarCtrl     m_StatBar;
	HICON m_hIcon;
	CWnd* m_pDrawWnd;
	TCHAR m_serial_port_array[10][20];

	//�������ڵĿؼ�Ԫ��
	int FreezeWindowComponents();

   //�������ڿؼ�Ԫ��
	int EnableAllWindowComponents();

	// ��ȡ�û��ṩ�Ĳ�������������в����Ƿ�Ϸ���
	// �Ϸ�����0�����򷵻�1
	int CheckRunPara();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	// ѡ��ִ�а�ť��Ӧ����
	afx_msg void OnBnTestUid();

	
	int current_status;

public:

	// ѡ��̼��ļ���ť��Ӧ����
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

		// 1 ��ʾ��¼��0��ʾ����оƬ, 2��ʾУ��оƬ
		int m_task_type;

		//�̼������ַ
		char m_sys_data_path[MAX_PATH];

};


