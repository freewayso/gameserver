#include "pch.h"
#include "game/rpcc2t_entergametest.h"

// generate by ProtoGen at date: 2020/8/20 19:21:01

RPC_CLIENT_IMPLEMETION(RpcC2T_EnterGameTest, EnterGameArg, EnterGameRes)

void RpcC2T_EnterGameTest::OnReply(const EnterGameArg &roArg, const EnterGameRes &roRes, const CUserData &roUserData)
{
	//进入游戏大厅rpc
	LogInfo("------------------------------------------accountid=%llu enter game-------------------- ",roArg.accountid());
		for (UINT32 i=0; i <roRes.actorlist().size(); i++ )
		{
			LogInfo("-----------enter game is sucessful----------------accountid=%llu actorname=%s actorid=%llu hairstyle=%u actorexp=%u",roRes.actorlist(i).accountid(),\
				roRes.actorlist(i).app().actorname().c_str(),roRes.actorlist(i).app().actid(),roRes.actorlist(i).app().hairstyle(),roRes.actorlist(i).att().actorexp());

		}
}

void RpcC2T_EnterGameTest::OnTimeout(const EnterGameArg &roArg, const CUserData &roUserData)
{
}
