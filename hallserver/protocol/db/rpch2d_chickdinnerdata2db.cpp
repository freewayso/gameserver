#include "pch.h"
#include "db/rpch2d_chickdinnerdata2db.h"

// generate by ProtoGen at date: 2020/8/7 10:57:52

RPC_CLIENT_IMPLEMETION(RpcH2D_ChickDinnerData2DB, ChickDinnerSaveDataArg, ChickDinnerSaveDataRes)

void RpcH2D_ChickDinnerData2DB::OnReply(const ChickDinnerSaveDataArg &roArg, const ChickDinnerSaveDataRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() != CBR::ERR_SUCCESS)
	{
		LogError("chick dinner sava data failed 【%u】",(UINT32)roRes.result());
	
	}

}

void RpcH2D_ChickDinnerData2DB::OnTimeout(const ChickDinnerSaveDataArg &roArg, const CUserData &roUserData)
{
}
