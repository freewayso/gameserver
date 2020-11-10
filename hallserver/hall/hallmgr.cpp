#include "pch.h"
#include "LogInit.h"
#include "hallmgr.h"
#include "timeutil.h"
#include "tinyxml2.h"
#include "util.h"
#include "crpc.h"
#include "config.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "actor/actormgr.h"



INSTANCE_SINGLETON(HallMgr);


HallMgr::HallMgr()
{

}


HallMgr::~HallMgr()
{
}

bool HallMgr::Init()
{ 
	return true;
}


void HallMgr::Uninit()
{

}

void HallMgr::Update()
{
	UINT32 dwTime = GameTime::GetTime();
	UINT64 qwMilli = TimeUtil::GetMilliSecond();
	ActorMgr::Instance()->Update(10);

}
