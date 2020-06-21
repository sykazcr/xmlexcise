
// xmlSandbox.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// メイン シンボル


// CxmlSandboxApp:
// このクラスの実装については、xmlSandbox.cpp を参照してください
//

class CxmlSandboxApp : public CWinApp
{
public:
	CxmlSandboxApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CxmlSandboxApp theApp;
