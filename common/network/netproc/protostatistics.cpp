#include "define.h"
#include "protostatistics.h"
#include <sstream>
#include "protocol.h"
#include "timeutil.h"

static ProtoStatistics *gInstance = NULL;

ProtoStatistics::ProtoStatistics()
{
	m_Enable = false;
}

ProtoStatistics::~ProtoStatistics()
{

}

ProtoStatistics * ProtoStatistics::Instance()
{
	if (gInstance == NULL)
	{
		gInstance = new ProtoStatistics();
	}

	return gInstance;
}

void ProtoStatistics::Destory()
{
	if (gInstance != NULL)
	{
		delete gInstance;
		gInstance = NULL;
	}
}

void ProtoStatistics::SetEnable(bool flag)
{
	m_Enable = flag;
}

void ProtoStatistics::AddProtoRecvBytes(UINT32 proto, UINT32 bytes, UINT64 ProcessTime)
{
	if (m_Enable == false) return;

	auto i = m_ProtoDetails.find(proto);
	if (i == m_ProtoDetails.end())
	{
		ProtoDetail detail;
		detail.RecvBytes = bytes;
		detail.SendBytes = 0;
		detail.ProcessTime = ProcessTime;
		detail.RecvCount = 1;
		detail.SendCount = 0;
		detail.RecvBytesInPeriod = bytes;
		detail.RecvCountInPeriod = 1;
		m_ProtoDetails.insert(std::make_pair(proto, detail));
	}
	else
	{
		i->second.RecvBytes += bytes;
		i->second.ProcessTime += ProcessTime;
		i->second.RecvCount += 1;
		i->second.RecvBytesInPeriod += bytes;
		i->second.RecvCountInPeriod += 1;
	}
}

void ProtoStatistics::AddProtoSendBytes(UINT32 proto, UINT32 bytes, UINT64 ProcessTime)
{
	if (m_Enable == false) return;

	auto i = m_ProtoDetails.find(proto);
	if (i == m_ProtoDetails.end())
	{
		ProtoDetail detail;
		detail.RecvBytes = 0;
		detail.SendBytes = bytes;
		detail.RecvCount = 0;
		detail.SendCount = 1;
		detail.ProcessTime = 0;
		detail.SendBytesInPeriod = bytes;
		detail.SendCountInPeriod = 1;
		m_ProtoDetails.insert(std::make_pair(proto, detail));
	}
	else
	{
		i->second.SendBytes += bytes;
		i->second.SendCount += 1;
		i->second.SendBytesInPeriod += bytes;
		i->second.SendCountInPeriod += 1;
		//i->second.ProcessTime += ProcessTime;
	}
}

UINT64 ProtoStatistics::GetProtoRecvBytes(UINT32 proto)
{
	auto i = m_ProtoDetails.find(proto);
	return (i == m_ProtoDetails.end()) ? 0 : i->second.RecvBytes;
}

UINT64 ProtoStatistics::GetProtoSendBytes(UINT32 proto)
{
	auto i = m_ProtoDetails.find(proto);
	return (i == m_ProtoDetails.end()) ? 0 : i->second.SendBytes;
}

ProtoDetail * ProtoStatistics::GetProtoDetail(UINT32 proto)
{
	auto i = m_ProtoDetails.find(proto);
	return (i == m_ProtoDetails.end()) ? 0 : &i->second;
}

std::string ProtoStatistics::DebugInfo()
{
	std::stringstream ss;

	ss << "Protocol\tRecvBytes\tRecvCount\tSendBytes\tSendCount\tProcessTime\n";
	for (auto i = m_ProtoDetails.begin(); i != m_ProtoDetails.end(); ++i)
	{
		ProtoDetail &detail = i->second;
		ss << i->first << "\t" << detail.RecvBytes << "\t" << detail.RecvCount
			<< "\t" << detail.SendBytes << "\t" << detail.SendCount << "\t" << detail.ProcessTime << "\n";
	}

	return ss.str();
}

const char *FormatNumber(UINT64 number, char *buf, int size)
{
	int a, b, c, d;
	a = (int)(number / 1000000000);
	number %= 1000000000;
	b = (int)(number / 1000000);
	number %= 1000000;
	c = (int)(number / 1000);
	number %= 1000;
	d = (int)(number);

#ifndef WIN32
#define _snprintf snprintf
#endif

	if (a > 0)
	{
		_snprintf(buf, size, "%d,%03d,%03d,%03d", a, b, c, d);
	}
	else if (b > 0)
	{
		_snprintf(buf, size, "%d,%03d,%03d", b, c, d);
	}
	else if (c > 0)
	{
		_snprintf(buf, size, "%d,%03d", c, d);
	}
	else
	{
		_snprintf(buf, size, "%d", d);
	}

	return buf;
}


std::string ProtoStatistics::HtmlInfo()
{
	std::stringstream ss;

	UINT64 tm = TimeUtil::GetMilliSecond() / 1000;
	UINT64 s = tm % 60;
	UINT64 m = tm / 60 % 60;
	UINT64 h = tm / 3600 % 24;
	UINT64 dy = tm / (3600 * 24);

	ss << "<html>\
		<meta http-equiv=\"refresh\" content=\"60\"> \
		<body>" 
	   << "<h2>Server has run " << dy << " day " << h << ":" << m << ":" << s
	   << "  ( " << tm << " s)" << "</h2>"
	   << "<table border=\"1\">\
		<tr>\
		<th>Protocol</th>\
		<th>TotalRecvBytes</th>\
		<th>TotalRecvCount</th>\
		<th>TotalSendBytes</th>\
		<th>TotalSendCount</th>\
		<th>ProcessTime(us)</th>\
		<th>ProcessTimePerProto(us)</th>\
		<th>MaxRecvBytesInPeriod</th>\
		<th>MaxRecvCountInPeriod</th>\
		<th>MaxSendBytesInPeriod</th>\
		<th>MaxSendCountInPeriod</th>\
		</tr>";

	char buf[10][128];
	for (auto i = m_ProtoDetails.begin(); i != m_ProtoDetails.end(); ++i)
	{
		ProtoDetail &detail = i->second;
		UINT64 temp = (detail.RecvCount == 0 ? 0 : detail.ProcessTime / detail.RecvCount);

		if(detail.SendBytesInPeriod > detail.MaxSendBytesInPeriod)
		{
			detail.MaxSendBytesInPeriod = detail.SendBytesInPeriod;
			detail.SendBytesInPeriod	= 0;
		}
		if(detail.SendCountInPeriod > detail.MaxSendCountInPeriod)
		{
			detail.MaxSendCountInPeriod = detail.SendCountInPeriod;
			detail.SendCountInPeriod	= 0;
		}
		if(detail.RecvBytesInPeriod > detail.MaxRecvBytesInPeriod)
		{
			detail.MaxRecvBytesInPeriod = detail.RecvBytesInPeriod;
			detail.RecvBytesInPeriod	= 0;
		}
		if(detail.RecvCountInPeriod > detail.MaxRecvCountInPeriod)
		{
			detail.MaxRecvCountInPeriod = detail.RecvCountInPeriod;
			detail.RecvCountInPeriod	= 0;
		}

		if (temp < 300)
		{
			ss << "<tr>";
		}
		else if (temp < 700)
		{
			ss << "<tr bgcolor=\"yellow\">";
		}
		else 
		{
			ss << "<tr bgcolor=\"pink\">";
		}

		ss << "<td>" << CProtocol::GetProtoName(i->first) << "</td><td>" 
			<< FormatNumber(detail.RecvBytes, buf[0], 128) << "</td><td>" 
			<< FormatNumber(detail.RecvCount, buf[1], 128) << "</td><td>" 
			<< FormatNumber(detail.SendBytes, buf[2], 128) << "</td><td>" 
			<< FormatNumber(detail.SendCount, buf[3], 128) << "</td><td>" 
			<< FormatNumber(detail.ProcessTime, buf[4], 128) << "</td><td>"
			<< FormatNumber(temp, buf[5], 128) << "</td><td>"
			<< FormatNumber(detail.MaxRecvBytesInPeriod, buf[6], 128) << "</td><td>"
			<< FormatNumber(detail.MaxRecvCountInPeriod, buf[7], 128) << "</td><td>"
			<< FormatNumber(detail.MaxSendBytesInPeriod, buf[8], 128) << "</td><td>"
			<< FormatNumber(detail.MaxSendCountInPeriod, buf[9], 128) << "</td></tr>";
	}

	ss << "</table></body></html>";

	return ss.str();
}

void ProtoStatistics::Reset()
{
	m_ProtoDetails.clear();
}
