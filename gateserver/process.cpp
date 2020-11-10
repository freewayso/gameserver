#include "pch.h"
#include "process.h"
#include "ptcregister.h"
#include "config.h"
#include "protocolstat.h"
#include "protocolcoder.h"
#include "session/sessionmanager.h"


static bool IsProcessRunning = true;


bool Process_Setup(const char *file)
{
	SINGLETON_CREATE_INIT_ARG1(GateConfig, file)
	SINGLETON_CREATE_INIT(CProtocolStat);
	SINGLETON_CREATE_INIT(CProtocolCoder);
	SINGLETON_CREATE_INIT(SessionManager);
	CProtocolRegister::Regiter();
	//CmdLine::Setup();
	return true;
}

void Process_Cleanup()
{
	//CmdLine::Stop();
	SINGLETON_DESTORY_UNINIT(SessionManager);
	SINGLETON_DESTORY_UNINIT(CProtocolCoder);
	SINGLETON_DESTORY_UNINIT(CProtocolStat);
	SINGLETON_DESTORY_UNINIT(GateConfig)
}

bool Process_Update()
{
	//CmdLine::Run();
	GateConfig::Instance()->ProcessNetMessage();
	SessionManager::Instance()->Update();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

