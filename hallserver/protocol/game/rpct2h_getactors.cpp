#include "pch.h"
#include "game/rpct2h_getactors.h"
#include "game/rpch2d_queryactordb.h"
#include "dblink.h"
#include "actor/actormgr.h"

// generate by ProtoGen at date: 2020/8/20 11:10:02

RPC_SERVER_IMPLEMETION(RpcT2H_GetActors, ActorDataArg, ActorDataRes)

void RpcT2H_GetActors::OnCall(const ActorDataArg &roArg, ActorDataRes &roRes)
{
	LogInfo("get actor params accountid[%llu]",roArg.accountid());
	RpcH2D_QueryActorDb  *pRpc = RpcH2D_QueryActorDb::CreateRpc();
	pRpc->m_oArg.set_actorid(roArg.actorid());
	pRpc->m_oArg.set_rpcid(DelayRpc());
	pRpc->m_oArg.set_accountid(roArg.accountid());
	pRpc->m_oArg.set_sessionid(roArg.sessionid());
	DBLink::Instance()->SendTo(*pRpc);
}

void RpcT2H_GetActors::OnDelayReplyRpc(const ActorDataArg &roArg, ActorDataRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == 0 && roUserData.m_pUserPtr)
	{
		const CBR::ActorDataRes* pInfo = (CBR::ActorDataRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);
		int actsize = pInfo->actorlist_size();
		if (actsize>=1)
		{
			ActorMgr::Instance()->Login(&pInfo->actorlist(0),roArg.sessionid());
		}
	}
}
