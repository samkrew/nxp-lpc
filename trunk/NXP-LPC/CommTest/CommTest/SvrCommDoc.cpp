// PingTestDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "SvrCommDoc.h"
#include "MainFrm.h"
#include "SvrCommFrm.h"
#include "SvrRecvView.h"


// CSvrCommDoc

IMPLEMENT_DYNCREATE(CSvrCommDoc, CDocument)

CSvrCommDoc::CSvrCommDoc()
{
 
}

BOOL CSvrCommDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSvrCommDoc::~CSvrCommDoc()
{
}


BEGIN_MESSAGE_MAP(CSvrCommDoc, CDocument)
END_MESSAGE_MAP()


// CSvrCommDoc 诊断

#ifdef _DEBUG
void CSvrCommDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSvrCommDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSvrCommDoc 序列化

void CSvrCommDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
#endif


// CSvrCommDoc 命令
 void CSvrCommDoc::StartTask( )
{
	 
}

void CSvrCommDoc::StopTask( )
{
	m_SvrComm.ShutDown();
}


void CSvrCommDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	StopTask();

	CDocument::OnCloseDocument();
}

void CSvrCommDoc::OpenSvrCommFrm( )
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	if (pFrame == NULL)
		return;	

	CMDIChildWnd *pChildWnd = NULL;

	CCreateContext context;
	context.m_pCurrentDoc = this;
	context.m_pCurrentFrame = pFrame;
	context.m_pNewDocTemplate = (CDocTemplate*) theApp.m_pClientDocTemplate;
	context.m_pNewViewClass = RUNTIME_CLASS(CSvrRecvView);

	CRuntimeClass* pFrameClass = RUNTIME_CLASS(CSvrCommFrm);   		 

	pChildWnd = (CMDIChildWnd*)pFrameClass->CreateObject();
	if (pChildWnd == NULL)
	{
		TRACE(traceAppMsg, 0, "Warning: Dynamic create of frame %hs failed.\n",	pFrameClass->m_lpszClassName);
		return  ;
	}
	ASSERT_KINDOF(CMDIChildWnd, pChildWnd);

	if (context.m_pNewViewClass == NULL)
		TRACE(traceAppMsg, 0, "Warning: creating frame with no default view.\n");

	// create new from resource
	if (!pChildWnd->LoadFrame(IDR_PACKET_DECODE,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
		NULL, &context))
	{
		TRACE(traceAppMsg, 0, "Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return  ;
	}
	HMENU hMenu = NULL;       // default menu resource for this frame
	HACCEL hAccelTable = NULL;       // accelerator table
	//	hAccelTable = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_PACKET_DECODE));
	// 	//	ASSERT( hAccelTable );
	hMenu = ::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_PACKET_DECODE));
	ASSERT(hMenu);
	pChildWnd->SetHandles(hMenu,hAccelTable);

	pChildWnd->InitialUpdateFrame(NULL, TRUE);		
//	pDoc->m_pDecodeFrm = (CPacketDecodeFrm *)pChildWnd;
//	pFrame->MDIActivate( pDoc->m_pDecodeFrm  ); 	

}
