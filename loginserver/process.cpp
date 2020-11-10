#include "pch.h"
#include "process.h"
#include "config.h"
#include "ptcregister.h"
#include "session/sessionmgr.h"
#include "login/mysqlmgr.h"
#include "login/verifymgr.h"


static bool IsProcessRunning = true;

bool Process_Setup(const char *configfile)
{
	CProtocolRegister::Regiter();

	SINGLETON_CREATE_INIT_ARG1(LoginConfig, configfile);
	SINGLETON_CREATE_INIT(SessionManager);
	SINGLETON_CREATE_INIT(VerifyMgr);
	SINGLETON_CREATE_INIT(CMysqlMgr);
	//CmdLine::Setup();

	return true;
}

void Process_Cleanup()
{
	//CmdLine::Stop();
	SINGLETON_DESTORY_UNINIT(CMysqlMgr);
	SINGLETON_DESTORY_UNINIT(VerifyMgr);
	SINGLETON_DESTORY_UNINIT(SessionManager);
	SINGLETON_DESTORY_UNINIT(LoginConfig);

}

bool Process_Update()
{
//	CmdLine::Run();

	LoginConfig::Instance()->ProcessNetMessage();
	VerifyMgr::Instance()->Update();
	CMysqlMgr::Instance()->Process();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

