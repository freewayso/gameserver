#include "pch.h"
#include "game/rpcc2h_entergamereq.h"
#include "LogInit.h"
// generate by ProtoGen at date: 2020/8/20 19:20:36

RPC_CLIENT_IMPLEMETION(RpcC2H_EnterGameReq, EnterGameArg, EnterGameRes)

void RpcC2H_EnterGameReq::OnReply(const EnterGameArg &roArg, const EnterGameRes &roRes, const CUserData &roUserData)
{
	for (UINT32 i=0; i <roRes.actorlist().size(); i++ )
	{
		LogInfo("-----------enter game is sucessful----------------accountid=%llu actorname=%s actorid=%llu hairstyle=%u actorexp=%u",roRes.actorlist(i).accountid(),\
			roRes.actorlist(i).app().actorname().c_str(),roRes.actorlist(i).app().actid(),roRes.actorlist(i).app().hairstyle(),roRes.actorlist(i).att().actorexp());

	}
	
}

void RpcC2H_EnterGameReq::OnTimeout(const EnterGameArg &roArg, const CUserData &roUserData)
{
}
