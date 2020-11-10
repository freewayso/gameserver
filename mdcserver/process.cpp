#include "pch.h"
#include "process.h"
#include "ptcregister.h"
#include "config.h"


static bool IsProcessRunning = true;

bool Process_Setup(const char *configfile)
{
	SINGLETON_CREATE_INIT_ARG1(DcmConfig, configfile)
	CProtocolRegister::Regiter();
	return true;
}


void Process_Cleanup()
{
	SINGLETON_DESTORY_UNINIT(DcmConfig);
}


bool Process_Update()
{
	DcmConfig::Instance()->ProcessNetMessage();
	return IsProcessRunning;
}


void Process_Stop()
{
	IsProcessRunning = false;
}

