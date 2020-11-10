#include "pch.h"
#include "game/rpch2d_updateactordata.h"

// generate by ProtoGen at date: 2020/9/9 15:09:21

RPC_CLIENT_IMPLEMETION(RpcH2D_UpdateActorData, UpdateActorDataArg, UpdateActorDataRes)

void RpcH2D_UpdateActorData::OnReply(const UpdateActorDataArg &roArg, const UpdateActorDataRes &roRes, const CUserData &roUserData)
{
	if (roRes.error()==CBR::ERR_FAILED)
	{
		LogError("actor id[%llu] save failed!!",roArg.act().app().actid());
	}
}

void RpcH2D_UpdateActorData::OnTimeout(const UpdateActorDataArg &roArg, const CUserData &roUserData)
{
}
