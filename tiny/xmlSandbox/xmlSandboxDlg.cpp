
// xmlSandboxDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "xmlSandbox.h"
#include "xmlSandboxDlg.h"
#include "afxdialogex.h"

#include "tinyxml.h"
#include "crtdbg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CxmlSandboxDlg ダイアログ



CxmlSandboxDlg::CxmlSandboxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XMLSANDBOX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CxmlSandboxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CxmlSandboxDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TEST_A, &CxmlSandboxDlg::OnBnClickedButtonTestA)
	ON_BN_CLICKED(IDC_BUTTON_TEST_A2_LOADXML, &CxmlSandboxDlg::OnBnClickedButtonTestA2Loadxml)
	ON_BN_CLICKED(IDC_BUTTON_TEST_A3_PUTXML, &CxmlSandboxDlg::OnBnClickedButtonTestA3Putxml)
END_MESSAGE_MAP()


// CxmlSandboxDlg メッセージ ハンドラー

BOOL CxmlSandboxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CxmlSandboxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CxmlSandboxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CxmlSandboxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

const unsigned int NUM_INDENTS_PER_SPACE=2;

const char * getIndent( unsigned int numIndents )
{
	static const char * pINDENT="                                      + ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

// same as getIndent but no "+" at the end
const char * getIndentAlt( unsigned int numIndents )
{
	static const char * pINDENT="                                        ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;

	return &pINDENT[ LENGTH-n ];
}

int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	const char* pIndent=getIndent(indent);
	_RPT0(_CRT_WARN, "\n");
	while (pAttrib)
	{
		_RPTN(_CRT_WARN,  "%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());

		if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    _RPTN(_CRT_WARN, " int=%d", ival); 
		if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) _RPTN(_CRT_WARN, " d=%1.1f", dval);
		_RPT0(_CRT_WARN, "\n" );
		i++;
		pAttrib=pAttrib->Next();
	}
	return i;	
}

void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 )
{
	if ( !pParent ) return;

	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	char dbg_str_tt_[256];
	//printf("%s", getIndent(indent));
	_RPTN(_CRT_WARN, "[%s]", getIndent(indent));
	//_RPTWN(_CRT_WARN, L"indent:[%s]\n", getIndent(indent));
//	_RPTN(_CRT_ERROR, "indent:[%s]", getIndent(indent));
//	snprintf(dbg_str_tt_, sizeof(dbg_str_tt_), "%s", getIndent(indent));
//	OutputDebugStringA(dbg_str_tt_);
	int num;

	switch ( t )
	{
	case TiXmlNode::TINYXML_DOCUMENT:
		OutputDebugStringA( "Document" );
		break;

	case TiXmlNode::TINYXML_ELEMENT:
		snprintf(dbg_str_tt_, sizeof(dbg_str_tt_), "Element [%s]", pParent->Value() );
	OutputDebugStringA(dbg_str_tt_);
		num=dump_attribs_to_stdout(pParent->ToElement(), indent+1);
		switch(num)
		{
			case 0:  OutputDebugStringA( " (No attributes)"); break;
			case 1: 	
				snprintf(dbg_str_tt_, sizeof(dbg_str_tt_), "%s1 attribute", getIndentAlt(indent)); 
				OutputDebugStringA(dbg_str_tt_);
				break;
			default:
				snprintf(dbg_str_tt_, sizeof(dbg_str_tt_), "%s%d attributes", getIndentAlt(indent), num); 
				OutputDebugStringA(dbg_str_tt_);
				break;
		}
		break;

	case TiXmlNode::TINYXML_COMMENT:
		snprintf(dbg_str_tt_, sizeof(dbg_str_tt_), "Comment: [%s]", pParent->Value());
		OutputDebugStringA(dbg_str_tt_);
		break;

	case TiXmlNode::TINYXML_UNKNOWN:
		OutputDebugStringA( "Unknown" );
		break;

	case TiXmlNode::TINYXML_TEXT:
		pText = pParent->ToText();
		snprintf(dbg_str_tt_, sizeof(dbg_str_tt_), "Text: [%s]", pText->Value() );
		OutputDebugStringA(dbg_str_tt_);
		break;

	case TiXmlNode::TINYXML_DECLARATION:
		OutputDebugStringA( "Declaration" );
		break;
	default:
		break;
	}
	OutputDebugStringA( "\n" );
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		dump_to_stdout( pChild, indent+1 );
	}
}

// load the named file and dump its structure to STDOUT
void dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		dump_to_stdout( &doc ); // defined later in the tutorial
	}
	else
	{
		char dbg_str_tt_[256];
		snprintf(dbg_str_tt_, sizeof(dbg_str_tt_), "Failed to load file \"%s\"\n", pFilename);
		OutputDebugStringA(dbg_str_tt_);
	}
}

void CxmlSandboxDlg::OnBnClickedButtonTestA()
{
	TiXmlDocument doc( "demo.xml" );
	doc.LoadFile();
}



void CxmlSandboxDlg::OnBnClickedButtonTestA2Loadxml()
{
//	::dump_to_stdout("example1.xml");
	::dump_to_stdout("日本語example1.xml");

}
/**
<?xml version="1.0" ?>
<MyApp>
    <!-- Settings for MyApp -->
    <Messages>
        <Welcome>Welcome to MyApp</Welcome>
        <Farewell>Thank you for using MyApp</Farewell>
    </Messages>
    <Windows>
        <Window name="MainFrame" x="5" y="15" w="400" h="250" />
    </Windows>
    <Connection ip="192.168.0.1" timeout="123.456" />
</MyApp>
**/
void CxmlSandboxDlg::OnBnClickedButtonTestA3Putxml()
{
	TiXmlDocument doc;  
	TiXmlElement* msg;
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  
 
	TiXmlElement * root = new TiXmlElement( "MyApp" );  
	doc.LinkEndChild( root );  

	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue(" Settings for MyApp " );  
	root->LinkEndChild( comment );  
 
	TiXmlElement * msgs = new TiXmlElement( "Messages" );  
	root->LinkEndChild( msgs );  
 
	msg = new TiXmlElement( "Welcome" );  
	msg->LinkEndChild( new TiXmlText( "Welcome to MyApp" ));  
	msgs->LinkEndChild( msg );  
 
	msg = new TiXmlElement( "Farewell" );  
	msg->LinkEndChild( new TiXmlText( "Thank you for using MyApp" ));  
	msgs->LinkEndChild( msg );  
 
	TiXmlElement * windows = new TiXmlElement( "Windows" );  
	root->LinkEndChild( windows );  

	TiXmlElement * window;
	window = new TiXmlElement( "Window" );  
	windows->LinkEndChild( window );  
	window->SetAttribute("name", "MainFrame");
	window->SetAttribute("x", 5);
	window->SetAttribute("y", 15);
	window->SetAttribute("w", 400);
	window->SetAttribute("h", 250);

	TiXmlElement * cxn = new TiXmlElement( "Connection" );  
	root->LinkEndChild( cxn );  
	cxn->SetAttribute("ip", "192.168.0.1");
	cxn->SetDoubleAttribute("timeout", 123.456); // floating point attrib
	
	dump_to_stdout( &doc );
	OutputDebugString(L"===============================\n");
	doc.SaveFile( "tt_appsettings.xml" ); 
}

