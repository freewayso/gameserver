#include "coder.h"
#include <string.h>
#include "util.h"
#include "LogInit.h"
#include "baseconfig.h"

#define COMPRESS_THREDHOLD Config::GetConfig()->GetCompressThredhold()
#define SEND_TEMP_BUFFER_SIZE  (1024 * 1024 * 10)

static MemoryStream stream(SEND_TEMP_BUFFER_SIZE);

MemoryStream::MemoryStream(int size)
{
	m_Buffer = new char[size];
	m_position = 0;
	m_length = 0;
	m_capcity = size;
	m_isCompressed = false;
}

MemoryStream::~MemoryStream()
{
	delete[] m_Buffer;
}

char * MemoryStream::GetBuffer()
{
	return m_Buffer;
}

int MemoryStream::GetLength()
{
	return m_length;
}

int MemoryStream::GetCapcity()
{
	return m_capcity;
}

int MemoryStream::GetSpace()
{
	return m_capcity - m_position;
}

void MemoryStream::Seek(int position)
{
	m_position = position;
}

void MemoryStream::Advance(int len)
{
	m_position += len;
	if (m_position > m_length)
		m_length = m_position;
}

bool MemoryStream::Write(const char *data, int len)
{
	if (m_position + len >= m_capcity)
	{
		LogWarn("protocol serialize buffer overflow!\n");
		return false;
	}

	memcpy(m_Buffer+m_position, data, len);
	Advance(len);
	return true;
}

MemoryStream * MemoryStream::Get(bool autoclear)
{
	if (autoclear)
	{
		stream.Clear();
	}

	return &stream;
}

void MemoryStream::Clear()
{
	m_isCompressed = false;
	m_position = 0;
	m_length = 0;
}

char * MemoryStream::GetWritePosition()
{
	return m_Buffer + m_position;
}

bool MemoryStream::Compress(int headsize)
{
	if (m_length - headsize < COMPRESS_THREDHOLD)
	{
		m_isCompressed = false;
		return true;
	}

	ZlibTool *pZlib = ZlibTool::Get();
	if (!pZlib->Compress(m_Buffer+headsize, m_length - headsize))
	{
		m_isCompressed = false;
		return false;
	}

	memcpy(m_Buffer+headsize, pZlib->GetBuffer(), pZlib->GetLength());
	m_position = headsize + pZlib->GetLength();
	m_length = m_position;
	m_isCompressed = true;
	return true;
}

static ZlibTool zlibtool(SEND_TEMP_BUFFER_SIZE);

ZlibTool::ZlibTool(int size)
{
	m_Buffer = new char[size];
	m_Len = 0;
	m_Capcity = size;
}

ZlibTool::~ZlibTool()
{
	delete []m_Buffer;
}

bool ZlibTool::Compress(const char *buf, int len)
{
	m_Len = ::Compress(buf, len, m_Buffer, m_Capcity);
	return m_Len > -1;
}

bool ZlibTool::DeCompress(const char *buf, int len)
{
	m_Len = ::DeCompress(buf, len, m_Buffer, m_Capcity);
	return m_Len > -1;
}

ZlibTool * ZlibTool::Get()
{
	return &zlibtool;
}

