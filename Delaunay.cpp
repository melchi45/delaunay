// Delaunay.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Delaunay.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DelaunayDoc.h"
#include "DelaunayView.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelaunayApp

BEGIN_MESSAGE_MAP(CDelaunayApp, CWinApp)
	//{{AFX_MSG_MAP(CDelaunayApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(IDC_TIPOFTHEDAY, OnTipOfTheDay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelaunayApp construction

CDelaunayApp::CDelaunayApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDelaunayApp object

CDelaunayApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDelaunayApp initialization

BOOL CDelaunayApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif // MFC 6.0 or earlier

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("MFC Forum"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DELAUNTYPE,
		RUNTIME_CLASS(CDelaunayDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDelaunayView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindowEx(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// This method handles the 'Tip of the Day' component.
	ShowTipAtStartup();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	//enum { IDD = IDD_ABOUTBOX };
	CStatic m_txtTitle;
	CStatic m_txtCopyright;
	CStatic m_txtAppName;
	CXTButton m_btnOk;
	CXTHyperLink m_txtURL;
	CXTHyperLink m_txtEmail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// App command to run the dialog
void CDelaunayApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDelaunayApp message handlers

void CDelaunayApp::ShowTipAtStartup()
{
	// This method handles the 'Tip of the Day' component.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CXTTipOfTheDay dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}
}

void CDelaunayApp::OnTipOfTheDay() 
{
	// This method handles the 'Tip of the Day' component.
	CXTTipOfTheDay dlg;
	dlg.DoModal();
}

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ABOUTBOX, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_TXT_TITLE, m_txtTitle);
	DDX_Control(pDX, IDC_TXT_COPYRIGHT, m_txtCopyright);
	DDX_Control(pDX, IDC_TXT_URL, m_txtURL);
	DDX_Control(pDX, IDC_TXT_EMAIL, m_txtEmail);
	DDX_Control(pDX, IDC_TXT_APPNAME, m_txtAppName);
	DDX_Control(pDX, IDOK, m_btnOk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// get a pointer to CWinApp.
	CWinApp* pApp = AfxGetApp( );
	ASSERT( pApp != NULL );

	// construct the about title.
	CString strAppName;
	strAppName.Format( _T( "MFC Forum" ));

	// set the strings for app and version.
	m_txtAppName.SetWindowText( strAppName );
	m_txtTitle.SetWindowText( xtAfxData.GetXTVersion( ) );

	// set the about dialog's title.
	CString strWindowText;
	strWindowText.Format( _T("About %s" ), strAppName );
	SetWindowText( strWindowText );

	// construct the copyright text.
	CString strCopyright;
	strCopyright.Format( _T( "1999-%d MFC Forum, All Rights Reserved" ),
		CTime::GetCurrentTime( ).GetYear( ) );
	m_txtCopyright.SetWindowText( strCopyright );

	// define the url for our hyperlinks.
	m_txtURL.SetURL( _T( "http://www.mfcforum.pe.kr/~melchi" ) );
	m_txtURL.SetUnderline( false );
	
	m_txtEmail.SetURL( _T( "mailto:melchi@mfcforum.pe.kr" ) );
	m_txtEmail.SetUnderline( false );

	// set the title text to bold font.
	m_txtTitle.SetFont( &xtAfxData.fontBold );

	// set OK button style.
	m_btnOk.SetXButtonStyle( BS_XT_SEMIFLAT | BS_XT_HILITEPRESSED );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
