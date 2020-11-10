#include "pch.h"
#include "login/rpcc2l_createactor.h"
#include "reconnetmgr.h"
#include "robot/robotmgr.h"
#include "timeutil.h"
#include "session/usermgr.h"

// generate by ProtoGen at date: 2020/8/21 11:09:13

RPC_CLIENT_IMPLEMETION(RpcC2L_CreateActor, CreateActorReq, CreateActorRes)

void RpcC2L_CreateActor::OnReply(const CreateActorReq &roArg, const CreateActorRes &roRes, const CUserData &roUserData)
{
	//LogInfo("-----------RpcC2L_CreateActor----------------accountid[%llu] token[%llu]",roRes.accoutid(),roRes.session());
	//std::string cli = "clientlink";
	//AccountSession *pSession = new AccountSession;
	//pSession->dwTime = TimeUtil::GetTime();
	//pSession->llAccountId = roRes.accoutid();
	//pSession->llToken  =  roRes.session();
	//pSession->dwState = 1;
	//pSession->llActorId = 0;
	//SessionMgr::Instance()->AddSession(pSession);
	//RobotMgr::Instance()->addClient("192.168.129.45",15010,pSession->llAccountId);
}

void RpcC2L_CreateActor::OnTimeout(const CreateActorReq &roArg, const CUserData &roUserData)
{
}
