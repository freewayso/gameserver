#ifndef __HALL_MGR_H__
#define __HALL_MGR_H__


#include <map>
#include <string>
#include "define.h"
#include "singleton.h"


class HallMgr
{
	HallMgr();
	~HallMgr();
	DECLARE_SINGLETON(HallMgr);
public:
	bool Init();
	void Uninit();
	void Update();	
public:

private:
	UINT32 m_nTime;
	UINT64 m_nId;
	UINT64 m_nmnTime;
};

#endif