#include "pch.h"
#include "game/rpch2m_reghall2mdc.h"
#include "network/halllink.h"

// generate by ProtoGen at date: 2020/9/14 21:14:34

RPC_SERVER_IMPLEMETION(RpcH2M_RegHall2Mdc, RegisterHallToDcmArg, RegisterHallToDcmRes)

void RpcH2M_RegHall2Mdc::OnCall(const RegisterHallToDcmArg &roArg, RegisterHallToDcmRes &roRes)
{
	HallLink::Instance()->AddHallServer(m_dwConnID,roArg.serverid());
}

void RpcH2M_RegHall2Mdc::OnDelayReplyRpc(const RegisterHallToDcmArg &roArg, RegisterHallToDcmRes &roRes, const CUserData &roUserData)
{
}
