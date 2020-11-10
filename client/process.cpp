#include "pch.h"
#include "process.h"
#include "ptcregister.h"
#include "robot/robotmgr.h"
#include "session/usermgr.h"

static bool IsProcessRunning = true;


bool Process_Setup(int argc, char** argv)
{
	SINGLETON_CREATE_INIT_ARG1(RobotMgr,argv[1]);
	SINGLETON_CREATE_INIT(SessionMgr);
	CProtocolRegister::Regiter();
	return true;
}

void Process_Cleanup()
{
	SINGLETON_DESTORY_UNINIT(SessionMgr);
	SINGLETON_DESTORY_UNINIT(RobotMgr);

}

bool Process_Update(int& mainRet)
{
	RobotMgr::Instance()->ProcessNetMessage();
	SessionMgr::Instance()->Update();
	RobotMgr::Instance()->Update();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

