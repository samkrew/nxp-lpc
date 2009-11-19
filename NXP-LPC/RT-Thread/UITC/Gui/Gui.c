/****************************************Copyright (c)**************************************************
**                                      
**                                      
**                                      
**
**                           
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��:  
**��   ��   ��:  
**����޸�����: 
**��        ��:  
**
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������:  
** ��  ��:  
** �ա���:  
** �衡��: 
**
**--------------��ǰ�汾�޶�------------------------------------------------------------------------------
** �޸���:
** �ա���:
** �衡��:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "applib.h"
#include "GuiList.h"
#include "keydrv.h"

#include "ScreenBase.h"
#include "guisystem.h"
#include "GuiEvent.h"
#include "ExtScreenDef.h"

#include "versionScr.h"
#include "devicecfgscreen.h"
#include "lcddriver.h"

//�������
struct SCREEN_MGR
{
	struct GuiListNode  ScrHeadNode;  // ����ָ������
	INT16U 	    ScreensCnt;	 // ��������
	INT8U       ActiveScrID; //��ǰ�����ID

//	CScreenBase *ScreenArray[MAX_SCREEN_ID];   // ����ָ������
};

typedef struct SCREEN_MGR CScreenMgr;

static CScreenMgr g_ScreenLib;

 
/*********************************************************************************************************
** ��������: GetCurrentScreenPtr
** ��������: GetCurrentScreenPtr
**
** ����������  �õ���ǰ��Ļ��ָ��
**
** �䡡��:  CScreenMgr * pMgr
**          
** �䡡��:   CScreenBase*
**         
** ȫ�ֱ���:  
** ����ģ��: ��
**
** ������:  LiJin
** �ա���:  2009��11��12��
** ��  ע:  
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** ��  ע: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CScreenBase* GetCurrentScreenPtr(CScreenMgr *pMgr)
{
	CScreenBase *pScr = NULL ;
	struct GuiListNode *pNode = NULL;
	if(pMgr && GuiListIsEmpty(& pMgr->ScrHeadNode ) )
	{
		ASSERT(pMgr->ActiveScrID > ID_SCREEN_NULL && pMgr->ActiveScrID < MAX_SCREEN_ID);
		if ( pMgr->ActiveScrID > ID_SCREEN_NULL && pMgr->ActiveScrID < MAX_SCREEN_ID )
		{//ѭ������
			GuiListForEach ( pNode,  & pMgr->ScrHeadNode)
		//	for ( n = pMgr->ScrHeadNode.pNext ; n != & pMgr->ScrHeadNode; n = n->pNext )
			{
				pScr = CONTAINING_RECORD(pNode, CScreenBase,List);
				ASSERT(pScr);
				if (pScr && pScr->CurrentID == pMgr->ActiveScrID )
				{
					return pScr;
				}
			}
		}
	}
 	return NULL;
}  
// �õ�ָ�����ScreenID��Ļ��ָ��
CScreenBase* GetScreenPtr(CScreenMgr *pMgr ,INT8U nScreenID)
{
	CScreenBase *pScr = NULL ;
	struct GuiListNode *pNode = NULL;
	if(pMgr && GuiListIsEmpty(& pMgr->ScrHeadNode ) )
	{
		ASSERT(pMgr->ActiveScrID > ID_SCREEN_NULL && pMgr->ActiveScrID < MAX_SCREEN_ID);
		if ( pMgr->ActiveScrID > ID_SCREEN_NULL && pMgr->ActiveScrID < MAX_SCREEN_ID )
		{//ѭ������
			GuiListForEach ( pNode,  & pMgr->ScrHeadNode)
		//	for ( n = pMgr->ScrHeadNode.pNext ; n != & pMgr->ScrHeadNode; n = n->pNext )
			{
				pScr = CONTAINING_RECORD(pNode, CScreenBase,List);
				ASSERT(pScr);
				if (pScr && pScr->CurrentID == nScreenID )
				{
					return pScr;
				}
			}
		}
	}
	return NULL;
}
/*********************************************************************************************************
** ��������: SwitchScreen
** ��������: SwitchScreen
**
** ���������� �л������ScreenID��Ļ 
**
** �䡡��:  INT8U nScreenID
**          
** �䡡��:   void
**         
** ȫ�ֱ���:  
** ����ģ��: ��
**
** ������:  LiJin
** �ա���:  2009��11��12��
** ��  ע:  
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** ��  ע: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SwitchScreen(INT8U nScreenID)
{
	CScreenBase * pScreen = NULL ;
	CScreenMgr *pMgr = &g_ScreenLib;

 	pScreen=GetCurrentScreenPtr(pMgr);

	//��������ԭ����
	if(pScreen && pScreen->pfnDestory)
	{
		pScreen->pfnDestory(pScreen);
	}

	//�л�����һ����
 	pScreen=GetScreenPtr(pMgr,nScreenID);

	if(pScreen == NULL)
		return;

 	//����л�����������򱣴���������򷵻ش���
 //	if(nScreenID == ID_PASSWORD || nScreenID == ID_MODALDIALOG)
 //		SetFatherID(& pScreen->Base,GetActiveWinID());

// 	SetActiveWinID(nScreenID);

 	//�����Ļ,ģʽ�Ի�����ΪҪ�����ϸ������ͼ�����в���������
 //	if(nScreenID != ID_MODALDIALOG)
 //		ClearScreen();
	if(pScreen != NULL && pScreen->pfnInit)
 		pScreen->pfnInit(pScreen,0,0);// ����Ĭ�ϳ�ʼ�����룬���罨����Դ

}
/*********************************************************************************************************
** ��������: InitScreenLib
** ��������: InitScreenLib
**
** ����������  ��ʼ�����е���Ļ
**
**
** �䡡��:
**
** ȫ�ֱ���:
** ����ģ��: ��
**
** ������:  LiJin
** �ա���:  2009��11��11��
** ��  ע:
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** ��  ע:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void InitScreenLib( )
{
	INT8U i = 0;
	CScreenBase *pScreen = NULL;

	//��ʼ��������
	GuiListInit(&(g_ScreenLib.ScrHeadNode));

	//����LOGO SCREEN
	//pScreen = ( );

	pScreen = InitVerScreen();

	//���ӵ�������
	GuiListAppend(&(g_ScreenLib.ScrHeadNode),&(pScreen->List));
	g_ScreenLib.ScreensCnt ++;

	pScreen = InitDevCfgScreen();
	GuiListAppend(&(g_ScreenLib.ScrHeadNode),&(pScreen->List));
	g_ScreenLib.ScreensCnt ++;


//	InitScreenMgr(&g_ScreeLib);
//	InitLogoScreen( &LogoScreen );
// 	InitDefaultScreen(&DefScreen);
//	InitMainMenuScreen(&MainMenuScreen);
//
//	InitRealTimeValScreen(&RealTimeValScr);
//
//	for ( i=0;i<sizeof(screens)/sizeof(CScreenBase * );i++)
//	{
//		AppendScreen(&g_ScreeLib, screens[i]);
//	}
//	//����Ĭ�ϻ���
	SwitchScreen( ID_START_SCREEN);
} 
/*********************************************************************************************************
** ��������: SendEventN
** ��������: SendEventN
**
** ����������  �������ͣ��͸���ǰ��Ĵ���
**
** �䡡��:  INT32U msg
** �䡡��:  INT32U param
**          
** �䡡��:   INT8U
**         
** ȫ�ֱ���:  
** ����ģ��: ��
**
** ������:  LiJin
** �ա���:  2009��11��12��
** ��  ע:  
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** ��  ע: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static INT8U SendEventN(INT32U msg,INT32U param,INT32U lParam)
{
	CScreenBase *pScreen = NULL;
	CScreenMgr *pMgr = &g_ScreenLib;

	pScreen = GetCurrentScreenPtr(pMgr);

	if (pScreen == NULL)
		return FALSE;

	return SendScreenEvnent( pScreen ,msg,param ,lParam);
}
//�������ͣ��͸���ǰ��Ĵ���
static INT8U PostEventN(INT32U msg,INT32U param)
{
	CScreenBase *pScreen = NULL;
	CScreenMgr *pMgr = &g_ScreenLib;

	pScreen = GetCurrentScreenPtr(pMgr);

	if (pScreen == NULL)
		return FALSE;

	return PostScreenEvnent( pScreen ,msg,param,NULL,TRUE);
}

static void HandleCurScrEvent(void)
{
	CScreenBase *pScreen = NULL;
	CScreenMgr *pMgr = &g_ScreenLib;

	pScreen = GetCurrentScreenPtr(pMgr);

	if (pScreen == NULL)
		return;

	HandleScreenEvent(  pScreen  )	;
}

#define  ID_CLOSE_LCD  100
INT8U OnTaskTimer(INT32U nEventID,INT32U Param)
{
	if (nEventID == ID_CLOSE_LCD)
	{
		TurnOffLcd();
		KillGuiTimer (NULL,ID_CLOSE_LCD);
		return TRUE;
	}

	return FALSE;
}


static void GuiTask(void *pdata)
{
	INT16U  key = 0;
	INT8U  bRet = FALSE;
//	INT32U  nTimerID = 0;
	GuiEvent TaskEvent;

	pdata = pdata;

	//����Һ��
	TurnOnLcd();
	//5���ر�Һ��
//	nTimerID = SetTimer(nTimerID,5000,NULL,NULL);
	SetGuiTimer(NULL,ID_CLOSE_LCD,5000);

	while(TRUE)
	{
		HandleCurScrEvent( );
		//��ȡ����
		key = 0;
		if (KeyHit())
		{
			//TODO: ��������취�����ع� GetKey �ӿ�,���л�ȡ���һ����ֵ,
			key = GetKey( 30 );
			if (key != KEY_NONE)
			{//�а���������LCD�ĵ�
				TurnOnLcd();
				SetGuiTimer(NULL,ID_CLOSE_LCD,5000);

				//1.�������Ƿ��������ȹ��ܼ�

				//2.������Ϣ
				SendEventN(GUI_EVENT_KEYDOWN,NULL,NULL);
			 	//PostEventN(GUI_EVENT_KEYDOWN, key);

			}
			else
			{//û�а���
				//�رյ�

			}
		}
		//�����Ƿ������������͹�������Ϣ
		bRet = GuiTaskEventRecv( &TaskEvent );
		if(bRet)
		{
			HandleTaskEvent(&TaskEvent);
		}

	}

}

//���������Ķ�ջ����
#define  GUI_TASK_STK_SIZE      512
#define  PRIO_GUI_TASK          30

static unsigned char  GuiTaskStk [GUI_TASK_STK_SIZE];
struct rt_thread Gui_Thread;

void InitGuiTask( void  )
{
	InitLcd();
	//��ʼ�����ڼ�������Ϣ����
	InitGuiEventMgr();

	InitGuiTimerMgr();
	// �����ʼ��
	InitScreenLib();
	//��������
	rt_thread_init(&Gui_Thread,"Gui_Thread",GuiTask, RT_NULL, &GuiTaskStk[0], sizeof(GuiTaskStk),
			PRIO_GUI_TASK, 10);

	rt_thread_startup(&Gui_Thread);
}