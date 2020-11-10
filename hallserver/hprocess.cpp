#include "pch.h"
#include "hprocess.h"
#include "config.h"
#include "ptcregister.h"
#include "hall/hallmgr.h"
#include "actor/actormgr.h"
#include "redis/redismgr.h"


static bool IsProcessRunning = true;

bool Process_Setup(const char *configfile)
{
	SINGLETON_CREATE_INIT_ARG1(HallConfig, configfile);
	SINGLETON_CREATE_INIT(HallMgr);
	SINGLETON_CREATE_INIT(ActorMgr);
	SINGLETON_CREATE_INIT(CRedisMgr);
	CProtocolRegister::Regiter();
	return true;
}

void Process_Cleanup()
{
	SINGLETON_DESTORY_UNINIT(CRedisMgr);
	SINGLETON_DESTORY_UNINIT(ActorMgr);
	SINGLETON_DESTORY_UNINIT(HallMgr);
	SINGLETON_DESTORY_UNINIT(HallConfig);
		
}

bool Process_Update()
{
	HallConfig::Instance()->ProcessNetMessage();
	HallMgr::Instance()->Update();
	CRedisMgr::Instance()->Process();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

