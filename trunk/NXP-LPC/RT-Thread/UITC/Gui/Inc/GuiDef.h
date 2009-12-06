 /****************************************Copyright (c)**************************************************
**							     
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:
**创   建   人:
**最后修改日期:
**描        述:   
**备        注 :
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本:  
** 日　期:  
** 描　述: 
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#pragma once

#include "applib.h"

//坐标位置
#ifndef LCD_POS
#	define LCD_POS INT16U
#endif

struct GUI_POINT
{
	INT16U x, y;
};
typedef struct GUI_POINT CGuiPoint;

struct GUI_RECT
{
	INT16U left, top, right, bottom;
};
typedef struct GUI_RECT CGuiRect;

typedef INT32U GUI_HANDLE;


#define GUI_RECT_WIDTH(r)	((r).right - (r).left)
#define GUI_RECT_HEIGHT(r)	((r).bottom - (r).top)


__inline void NormalizeGuiRect(CGuiRect *pRect)
{
	LCD_POS nTemp;
	if (pRect)
	{
		if (pRect->left > pRect->right)
		{
			nTemp = pRect->left;
			pRect->left = pRect->right;
			pRect->right = nTemp;
		}
		if (pRect->top > pRect->bottom)
		{
			nTemp = pRect->top;
			pRect->top = pRect->bottom;
			pRect->bottom = nTemp;
		}
	}
}

__inline void SetGuiRect(CGuiRect *pRect, LCD_POS l, LCD_POS t, LCD_POS r, LCD_POS b)
{
	pRect->left = l; pRect->top = t; pRect->right = r; pRect->bottom = b;
}

__inline void SetGuiRectEmpty(CGuiRect * pRect)
{
	pRect->left=0;pRect->right=0;pRect->top=0;pRect->bottom=0;
}

__inline void CopyGuiRect(CGuiRect* pdrc, const CGuiRect* psrc)
{
    memcpy(pdrc, psrc, sizeof(CGuiRect));
}

__inline INT8U IsGuiRectEmpty(const CGuiRect* prc)
{
    if( prc->left == prc->right ) return TRUE;
    if( prc->top == prc->bottom ) return TRUE;
    return FALSE;
}

__inline INT8U EqualGuiRect(const CGuiRect* prc1, const CGuiRect* prc2)
{
    if(prc1->left != prc2->left) return FALSE;
    if(prc1->top != prc2->top) return FALSE;
    if(prc1->right != prc2->right) return FALSE;
    if(prc1->bottom != prc2->bottom) return FALSE;

    return TRUE;
}

__inline INT8U IsGuiRectCovered(const CGuiRect* prc1, const CGuiRect* prc2)
{
    if (prc1->left < prc2->left
            || prc1->top < prc2->top
            || prc1->right > prc2->right
            || prc1->bottom > prc2->bottom)
        return FALSE;

    return TRUE;
}

__inline INT8U IntersectGuiRect(CGuiRect* pdrc, const CGuiRect* psrc1, const CGuiRect* psrc2)
{
    pdrc->left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
    pdrc->top  = (psrc1->top > psrc2->top) ? psrc1->top : psrc2->top;
    pdrc->right = (psrc1->right < psrc2->right) ? psrc1->right : psrc2->right;
    pdrc->bottom = (psrc1->bottom < psrc2->bottom)
                   ? psrc1->bottom : psrc2->bottom;

    if(pdrc->left >= pdrc->right || pdrc->top >= pdrc->bottom)
        return FALSE;

    return TRUE;
}

__inline INT8U DoesIntersect (const CGuiRect* psrc1, const CGuiRect* psrc2)
{
    INT16U left, top, right, bottom;

    left = (psrc1->left > psrc2->left) ? psrc1->left : psrc2->left;
    top  = (psrc1->top > psrc2->top) ? psrc1->top : psrc2->top;
    right = (psrc1->right < psrc2->right) ? psrc1->right : psrc2->right;
    bottom = (psrc1->bottom < psrc2->bottom)
                   ? psrc1->bottom : psrc2->bottom;

    if(left >= right || top >= bottom)
        return FALSE;

    return TRUE;
}

__inline INT8U UnionGuiRect(CGuiRect* pdrc, const CGuiRect* psrc1, const CGuiRect* psrc2)
{
	CGuiRect src1, src2;
    memcpy(&src1, psrc1, sizeof(CGuiRect));
    memcpy(&src2, psrc2, sizeof(CGuiRect));

    NormalizeGuiRect(&src1);
	NormalizeGuiRect(&src2);

	if (src1.left == src2.left && src1.right == src2.right)
	{
		if (src1.top <= src2.top && src2.top <= src1.bottom)
		{
			pdrc->left = src1.left;
			pdrc->right = src1.right;
			pdrc->top = src1.top;
			pdrc->bottom = MAX(src1.bottom, src2.bottom);

			return TRUE;
		}
		else if (src1.top >= src2.top && src2.bottom >= src1.top)
        {
            pdrc->left = src1.left;
            pdrc->right = src1.right;
            pdrc->top = src2.top;
			pdrc->bottom = MAX(src1.bottom, src2.bottom);

			return TRUE;
		}
		return FALSE;
	}

	if (src1.top == src2.top && src1.bottom == src2.bottom)
	{
		if (src1.left <= src2.left && src2.left <= src1.right)
		{
			pdrc->top = src1.top;
			pdrc->bottom = src1.bottom;
			pdrc->left = src1.left;
			pdrc->right = MAX(src1.right, src2.right);

			return TRUE;
		}
		else if (src1.left >= src2.left && src2.right >= src1.left)
		{
			pdrc->top = src1.top;
			pdrc->bottom = src1.bottom;
			pdrc->left = src2.left;
			pdrc->right = MAX(src1.right, src2.right);

			return TRUE;
		}
		return FALSE;
	}

    return FALSE;
}

__inline void GetBoundGuiRect(CGuiRect * pdrc,  const CGuiRect* psrc1, const CGuiRect* psrc2)
{
	CGuiRect src1, src2;
    memcpy(&src1, psrc1, sizeof(CGuiRect));
    memcpy(&src2, psrc2, sizeof(CGuiRect));

    NormalizeGuiRect(&src1);
    NormalizeGuiRect(&src2);

    pdrc->left = (src1.left < src2.left) ? src1.left : src2.left;
    pdrc->top  = (src1.top < src2.top) ? src1.top : src2.top;
    pdrc->right = (src1.right > src2.right) ? src1.right : src2.right;
    pdrc->bottom = (src1.bottom > src2.bottom)
                   ? src1.bottom : src2.bottom;
}

__inline int SubtractGuiRect(CGuiRect* rc, const CGuiRect* psrc1, const CGuiRect* psrc2)
{
	CGuiRect src, rcExpect, *prcExpect;
    INT16U nCount = 0;

    src = *psrc1;
    rcExpect = *psrc2;
    prcExpect = &rcExpect;

    if (!DoesIntersect (&src, prcExpect))
    {
        nCount = 1;
        rc[0] = src;
    }
    else
    {
        if(prcExpect->top > src.top)
        {
            rc[nCount].left  = src.left;
            rc[nCount].top   = src.top;
            rc[nCount].right = src.right;
            rc[nCount].bottom = prcExpect->top;
            nCount++;
            src.top = prcExpect->top;
        }
        if(prcExpect->bottom < src.bottom)
        {
            rc[nCount].top  = prcExpect->bottom;
            rc[nCount].left   = src.left;
            rc[nCount].right = src.right;
            rc[nCount].bottom = src.bottom;
            nCount++;
            src.bottom = prcExpect->bottom;
        }
        if(prcExpect->left > src.left)
        {
            rc[nCount].left  = src.left;
            rc[nCount].top   = src.top;
            rc[nCount].right = prcExpect->left;
            rc[nCount].bottom = src.bottom;
            nCount++;
        }
        if(prcExpect->right < src.right)
        {
            rc[nCount].left  = prcExpect->right;
            rc[nCount].top   = src.top;
            rc[nCount].right = src.right;
            rc[nCount].bottom = src.bottom;
            nCount++;
        }
    }

    return nCount;
}

__inline void  OffsetGuiRect(CGuiRect* prc, INT16U x, INT16U y)
{
    prc->left += x;
    prc->top += y;
    prc->right += x;
    prc->bottom += y;
}

__inline void InflateGuiRect(CGuiRect* prc, INT16U cx, INT16U cy)
{
    prc->left -= cx;
    prc->top -= cy;
    prc->right += cx;
    prc->bottom += cy;
}

__inline void InflateGuiRectToPt (CGuiRect* prc, INT16U x, INT16U y)
{
    if (x < prc->left) prc->left = x;
    if (y < prc->top) prc->top = y;
    if (x > prc->right) prc->right = x;
    if (y > prc->bottom) prc->bottom = y;
}

__inline INT8U PtInGuiRect(const CGuiRect* prc, INT16U x, INT16U y)
{
    if (   x >= prc->left
        && x < prc->right
        && y >= prc->top
        && y < prc->bottom)
        return TRUE;

    return FALSE;
}



struct GUI_SIZE
{
    /**
     * the extent in x coordinate of a 2D object.
     */
	INT16U cx;
    /**
     * the extent in y coordinate of a 2D object.
     */
	INT16U cy;
} ;

typedef struct GUI_SIZE CGuiSize;

//定义字符串最大长度
#define STRING_MAXLEN 256

//汉字库索引启示地址
#define CNZI_FIRST_INDEX 10000


