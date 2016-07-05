// EasyWriteDlg.h : ͷ�ļ�
//

#pragma once

#define IDC_STATUS 5001

// CEasyWriteDlg �Ի���
class CEasyWriteDlg : public CDialog
{
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

//	int SetBusyStatus();

//	int SetIdleStatus();

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
	CString            m_src_file_path;
	CStatusBarCtrl     m_StatBar;
	HICON m_hIcon;
	CWnd* m_pDrawWnd;

	// ���ɵ���Ϣӳ�亯��
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

	// ���õ��Ե�����дһ��оƬ
	static unsigned int WINAPI SingleWriteOneChip(void *arg);

	static unsigned int WINAPI WriteOneChip(void *arg);
	
	//������λ���̣߳�ȷ����һ��оƬ�Ѿ���װ��λ
	static unsigned int WINAPI CheckNewChip(void *arg);



	int InitialSerialPort();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedSingleWrite();
};
