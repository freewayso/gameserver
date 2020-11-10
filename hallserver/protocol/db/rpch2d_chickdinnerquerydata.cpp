#include "pch.h"
#include "db/rpch2d_chickdinnerquerydata.h"
#include "LogInit.h"

// generate by ProtoGen at date: 2020/8/11 10:50:46

RPC_CLIENT_IMPLEMETION(RpcH2D_ChickDinnerQueryData, ChickDinnerQueryDataArg, ChickDinnerQueryDataRes)

void RpcH2D_ChickDinnerQueryData::OnReply(const ChickDinnerQueryDataArg &roArg, const ChickDinnerQueryDataRes &roRes, const CUserData &roUserData)
{
	if (roRes.error() == CBR::ERR_SUCCESS)
	{
			LogInfo("RpcH2D_ChickDinnerQueryData....[%s] [%llu] [%u] [%u]",roRes.data().name().c_str(),roRes.data().roleid(),roRes.data().score(),roRes.data().killer());
	}else
	{
			LogWarn("RpcH2D_ChickDinnerQueryData...not found.uid=[%llu] errorcode[%u]",roRes.data().roleid(),roRes.error());
	}
}

void RpcH2D_ChickDinnerQueryData::OnTimeout(const ChickDinnerQueryDataArg &roArg, const CUserData &roUserData)
{

}
