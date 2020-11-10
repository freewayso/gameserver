#ifndef _H_Protocol_Coder_H__
#define _H_Protocol_Coder_H__

#include "baselink.h"


class CProtocolCoder
{
	CProtocolCoder();
	~CProtocolCoder();
	DECLARE_SINGLETON(CProtocolCoder)
public:
	bool Init();
	void Uninit();

	void Encode(const char* pcData, UINT32 dwDataLen, DataBlob& roOutData, UINT32 dwPtcType, UINT32& rdwPtcFlag);

	//������false
	bool Decode(const char* pcData, UINT32 dwDataLen, DataBlob& roOutData, UINT32 dwPtcType, UINT32& rdwPtcFlag);
private:
};

#endif