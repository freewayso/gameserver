#include "pch.h"
#include "game/rpcc2t_clienttokenauth.h"
#include "session/sessionmanager.h"

// generate by ProtoGen at date: 2020/9/9 19:17:44

RPC_SERVER_IMPLEMETION(RpcC2T_ClientTokenAuth, ClientAuthArg, ClientAuthRes)

void RpcC2T_ClientTokenAuth::OnCall(const ClientAuthArg &roArg, ClientAuthRes &roRes)
{
	CBR::ErrorCode ret =  SessionManager::Instance()->AuthToken(m_dwConnID,roArg.token());
	roRes.set_ret(ret);

}

void RpcC2T_ClientTokenAuth::OnDelayReplyRpc(const ClientAuthArg &roArg, ClientAuthRes &roRes, const CUserData &roUserData)
{
}
