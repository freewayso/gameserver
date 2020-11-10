#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2020/9/9 19:14:51

#include "login/ptcl2t_loginsession2gatentf.h"
#include "common/rpct2l_registergate2login.h"
#include "login/rpcc2l_loginreq.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CRpc::RegistRpc(RpcT2L_RegisterGate2Login::CreateRpc());
	CRpc::RegistRpc(RpcC2L_LoginReq::CreateRpc());


	CProtocol::RegistProtoName(PTCL2T_LOGINSESSION2GATENTF_TYPE, "PTCL2T_LOGINSESSION2GATENTF");
	CProtocol::RegistProtoName(RPCT2L_REGISTERGATE2LOGIN_TYPE, "RPCT2L_REGISTERGATE2LOGIN");
	CProtocol::RegistProtoName(RPCC2L_LOGINREQ_TYPE, "RPCC2L_LOGINREQ");

}