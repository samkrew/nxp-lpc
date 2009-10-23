/****************************************Copyright (c)**************************************************
**                                江苏科技股份有限公司
**                                     事业部
**                                         
**
**                                 http://www.xxx.com.cn
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 2 
**描        述:  
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本: v1.00
** 日　期:  
** 描　述: 原始版本
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:  
** 日　期:  
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "stdafx.h"
#include "SvrCommMsg.h"

CUEPacket::CUEPacket()
{
	Init();
}

CUEPacket::~CUEPacket()
{

}


void CUEPacket::Init()
{
	m_nUsed=0;
	m_nOperation=-1;
 	ZeroMemory(&m_ol, sizeof(OVERLAPPED));
	ZeroMemory(&m_Buffer,sizeof(m_Buffer));
}
// Setup Setup the buffer for a Read. 
void CUEPacket::SetupRead()
{
	if (m_nUsed == 0)
	{
		m_wsabuf.buf = reinterpret_cast<char*>(m_Buffer);
		m_wsabuf.len = MAX_PACKAGE_SIZE; 
	}
	else // We have received some of the data but not all .. 
	{
		m_wsabuf.buf = reinterpret_cast<char*>(m_Buffer) + m_nUsed;
		m_wsabuf.len = MAX_PACKAGE_SIZE - m_nUsed; 
	}
}

// Setup the buffer for a Write
void CUEPacket::SetupWrite()
{
	m_wsabuf.buf = reinterpret_cast<char*>(m_Buffer);
	m_wsabuf.len = m_nUsed;
}

// cheks if the Buffer is valid. 
BOOL CUEPacket::IsValid()
{
	return TRUE;
}

UINT CUEPacket::GetUsed()
{
	return m_nUsed;
}

// Used to indicate that we did have  a successfull Receive 
UINT CUEPacket::Use(UINT nSize)
{
	m_nUsed+=nSize;
	return m_nUsed;
}

// Empty A used structure. 
void CUEPacket::EmptyUsed()
{
	m_nUsed=0;
}


// removes nSize byte from the Buffer. 
BOOL CUEPacket::Flush(UINT nBytesToRemove)
{
	if ((nBytesToRemove > MAX_PACKAGE_SIZE) || (nBytesToRemove > m_nUsed) ) 
	{
		TRACE("ERROR BOOL CUEPacket::Flush(UINT nBytesToRemove)");
		return FALSE;
	}
	m_nUsed-=nBytesToRemove;
	memmove(m_Buffer, m_Buffer + nBytesToRemove, m_nUsed);
	return TRUE;
}


BOOL CUEPacket::AddData(BYTE data)
{
	return AddData(&data, 1);
}

BOOL CUEPacket::AddData(UINT data)
{
	return AddData(reinterpret_cast<const BYTE*>(&data), sizeof(UINT));
}

BOOL CUEPacket::AddData(unsigned short data)
{
	return AddData(reinterpret_cast<const BYTE*>(&data), sizeof(unsigned short));
}

/*
* Adds a stream of char to the buffer. 
*/
BOOL CUEPacket::AddData(const char *const pData, UINT nSize)
{ 
	return AddData(reinterpret_cast<const BYTE*>(pData),nSize);
}

/*
* Adds a stream of BYTES to the buffer. 
*/
BOOL CUEPacket::AddData(const BYTE *const pData, UINT nSize)
{
	if ( nSize > MAX_PACKAGE_SIZE-m_nUsed )
		return FALSE;
	else
	{
		memcpy(m_Buffer + m_nUsed, pData, nSize);
		m_nUsed += nSize;
		return TRUE;
	}
}
// Setup the buffer for a ZeroByteRead. 
void CUEPacket::SetupZeroByteRead()
{
	m_wsabuf.buf =(char*)m_Buffer;
	m_wsabuf.len = 0; 
}
