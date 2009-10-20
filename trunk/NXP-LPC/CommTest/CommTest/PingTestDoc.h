#pragma once

// CPingTestDoc �ĵ�
const unsigned int UPDATA_VIEW_PING_NORMAL = 1;
//const unsigned int UPDATA_VIEW_CLIENT_PACKET = 2;

class CPingTestDoc : public CDocument
{
	DECLARE_DYNCREATE(CPingTestDoc)

public:
	CPingTestDoc();
	virtual ~CPingTestDoc();

public:
	DWORD m_dwDestIp;
	UINT m_nTaskCnt;
	UINT m_nPingDataSize;
	UINT m_nIntTime;
	BOOL m_bAutoDelay;

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
