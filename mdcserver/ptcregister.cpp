#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2020/9/14 21:14:34

#include "game/rpch2m_reghall2mdc.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CRpc::RegistRpc(RpcH2M_RegHall2Mdc::CreateRpc());


	CProtocol::RegistProtoName(RPCH2M_REGHALL2MDC_TYPE, "RPCH2M_REGHALL2MDC");

}