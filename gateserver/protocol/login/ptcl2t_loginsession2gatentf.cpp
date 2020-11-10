#include "pch.h"
#include "login/ptcl2t_loginsession2gatentf.h"
#include "session/sessionmanager.h"

// generate by ProtoGen at date: 2020/8/17 11:52:44

void PtcL2T_LoginSession2GateNtf::Process(UINT32 dwConnID)
{
	LogInfo("login server notify token %llu",m_Data.session());
	SessionManager::Instance()->AddToken(m_Data);

}
