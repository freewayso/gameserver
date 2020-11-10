#include "pch.h"
#include "dbprocess.h"
#include "config.h"
#include "mysql/mysqlmgr.h"
#include "redis/redismgr.h"
#include "ptcregister.h"
#include "cache/dbeventconv.h"
#include "cache/memorysyncdb.h"

#include "profile/dbprofiler.h"

static bool IsProcessRunning = true;



bool Process_Setup(const char *configfile)
{
	SINGLETON_CREATE_INIT_ARG1(DBConfig, configfile)
	SINGLETON_CREATE_INIT(CRedisMgr);
	SINGLETON_CREATE_INIT(CMysqlMgr);
	SINGLETON_CREATE_INIT(CDbProfiler)
	SINGLETON_CREATE_INIT(DbEventConvMgr)
	SINGLETON_CREATE_INIT(MemorySyncDb)

	//CmdLine::RegistCmd("debug", CmdLineSetDebugFlag);
	//CmdLine::Setup();
	CProtocolRegister::Regiter();

	return true;
}

void Process_Cleanup()
{	
	LogWarn("db stop...");
	//CmdLine::Stop();
		
	SINGLETON_DESTORY_UNINIT(CDbProfiler)
	SINGLETON_DESTORY_UNINIT(DbEventConvMgr)
	SINGLETON_DESTORY_UNINIT(MemorySyncDb)
	SINGLETON_DESTORY_UNINIT(CMysqlMgr);
	SINGLETON_DESTORY_UNINIT(CRedisMgr);
	SINGLETON_DESTORY_UNINIT(DBConfig);
}

bool Process_Update()
{
	DBConfig::Instance()->ProcessNetMessage();
	CMysqlMgr::Instance()->Process();
	CRedisMgr::Instance()->Process();
	MemorySyncDb::Instance()->Update();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

