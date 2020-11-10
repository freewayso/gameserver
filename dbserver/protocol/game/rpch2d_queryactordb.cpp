#include "pch.h"
#include "game/rpch2d_queryactordb.h"
#include "cache/memorysyncdb.h"
#include "cache/dbeventconv.h"


RPC_SERVER_IMPLEMETION(RpcH2D_QueryActorDb, ActorDataArg, ActorDataRes)
void RpcH2D_QueryActorDb::OnCall(const ActorDataArg &roArg, ActorDataRes &roRes)
{
	CBR::ActorBase * pActor =  MemorySyncDb::Instance()->GetActorBase(roArg.actorid());
	if (pActor)
	{
		roRes.add_actorlist()->CopyFrom(*pActor);
		roRes.set_error(CBR::ERR_SUCCESS);
	}else
	{
		DbReadEvent *pInfo = new DbReadEvent;
		pInfo->dwRpcId = DelayRpc();
		pInfo->qwActorId = roArg.actorid();
		DbEventConvMgr::Instance()->AddEvent(pInfo,roArg);
	}
}

void RpcH2D_QueryActorDb::OnDelayReplyRpc(const ActorDataArg &roArg, ActorDataRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_pUserPtr)
	{
		ActorDataRes& res = *(ActorDataRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(res);
		roRes.set_error(ERR_SUCCESS);
	}else
	{

	}

}
