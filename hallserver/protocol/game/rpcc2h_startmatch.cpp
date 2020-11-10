#include "pch.h"
#include "game/rpcc2h_startmatch.h"
#include "actor/actormgr.h"
#include "actor/actor.h"
// generate by ProtoGen at date: 2020/9/15 10:08:33

RPC_SERVER_IMPLEMETION(RpcC2H_StartMatch, StartMatchArg, StartMatchRes)

void RpcC2H_StartMatch::OnCall(const StartMatchArg &roArg, StartMatchRes &roRes)
{

	Actor  *pActor = 	ActorMgr::Instance()->GetBySession(m_sessionID);

}

void RpcC2H_StartMatch::OnDelayReplyRpc(const StartMatchArg &roArg, StartMatchRes &roRes, const CUserData &roUserData)
{
}
