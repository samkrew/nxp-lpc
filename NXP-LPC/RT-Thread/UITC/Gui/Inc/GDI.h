 /****************************************Copyright (c)**************************************************
**						��ʾӲ������������ֻ֧��1����ʾ�ӿ�	     
**                                      
**                                      
**
**                           
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��:   LcdDriver.h
**��   ��   ��:   Author: admin
**����޸�����: Created on: 2009-11-15
**��        ��:   
**��        ע :    
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

#pragma once 

#include "applib.h" 
#include "GuiDef.h" 


typedef struct GUI_CLIPRECT CGuiClipRect;

struct GUI_CLIPRECT
{
	CGuiRect Rect;

	CGuiClipRect *pPrev;
	CGuiClipRect *pNext; 
};

struct GUI_BLOCK_HEAP;
struct GUI_CLIPRGN
{
	CGuiRect RectBound; // The bounding rect of the region.

	CGuiClipRect *pHead;
	CGuiClipRect *pTail;

	struct GUI_BLOCK_HEAP *pHeap;
};

typedef struct GUI_CLIPRGN CGuiClipRgn;


#define GUI_REGION_NULL      0x00
#define GUI_REGION_SIMPLE    0x01
#define GUI_REGION_COMPLEX   0x02