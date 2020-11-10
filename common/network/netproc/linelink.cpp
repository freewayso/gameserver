#include "linelink.h"
#include "protocol.h"
#include "netproc.h"
#include "crpc.h"
#include "LogInit.h"


LineLink::LineLink()
{
}

LineLink::~LineLink()
{
}

void LineLink::OnClose(UINT32 dwConnID, int err)
{
	for(CMapLine2Conn::iterator it = m_oMapLine2Conn.begin(); it != m_oMapLine2Conn.end(); ++it)
	{
		if(it->second == dwConnID)
		{
			UINT32 dwLine = it->first;
			m_oMapLine2Conn.erase(it);
			OnLineClosed(dwLine);
			break;
		}
	}
}

bool LineLink::SendToLine(UINT32 dwLine, const CProtocol& roPtc)
{
	UINT32 dwConnID = GetConnID(dwLine);
	if(dwConnID == INVALID_CONN_ID)
	{
		LogError("invalid dwLine =%u",dwLine);
		return false;
	}

	return Send(dwConnID, roPtc);
}

bool LineLink::SendToLine(UINT32 dwLine, const CRpc& roRpc)
{
	UINT32 dwConnID = GetConnID(dwLine);
	if(dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(dwConnID, const_cast<CRpc&>(roRpc));
}

bool LineLink::SendToAllLine(const CProtocol& roPtc)
{
	for(CMapLine2Conn::iterator it = m_oMapLine2Conn.begin(); it != m_oMapLine2Conn.end(); ++it)
	{
		Send(it->second, roPtc);
	}
	return true;
}

bool LineLink::SendToAllLine(const CRpc& roRpc)
{
	for(CMapLine2Conn::iterator it = m_oMapLine2Conn.begin(); it != m_oMapLine2Conn.end(); ++it)
	{
		Send(it->second, const_cast<CRpc&>(roRpc));
	}
	return true;
}

bool LineLink::RegisterLine(UINT32 dwLine, UINT32 dwConnID)
{
	std::pair<CMapLine2Conn::iterator, bool> pr = m_oMapLine2Conn.insert(std::make_pair(dwLine, dwConnID));
	if(pr.second == false)
	{
		m_oMapLine2Conn[dwLine] = dwConnID;
		LogWarn("line[%u] already registered", dwLine);
	}
	OnLineConnected(dwLine);
	return pr.second;
}

bool LineLink::IsLineConnected(UINT32 dwLine) const
{
	return m_oMapLine2Conn.find(dwLine) != m_oMapLine2Conn.end(); 
}

UINT32 LineLink::GetConnID(UINT32 dwLine)
{
	CMapLine2Conn::iterator it = m_oMapLine2Conn.find(dwLine);
	return it == m_oMapLine2Conn.end() ? INVALID_CONN_ID : it->second;
}