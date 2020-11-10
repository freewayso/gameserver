#include "pch.h"
#include "login/ptct2h_loginoutntf.h"
#include "actor/actormgr.h"
#include "LogInit.h"
// generate by ProtoGen at date: 2020/8/25 15:08:26

void PtcT2H_LoginOutNtf::Process(UINT32 dwConnID)
{
	LogInfo("loginout session =%llu error=%u ",m_Data.session(),m_Data.error());
	ActorMgr::Instance()->LoginOut(m_Data.session());

}
