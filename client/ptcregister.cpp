#include "pch.h"
#include "forward.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2020/9/15 10:08:33

#include "game/ptct2c_heartbeat.h"
#include "login/rpcc2l_loginreq.h"
#include "game/rpcc2t_entergametest.h"
#include "game/rpcc2t_createactorcli2gate.h"
#include "game/rpcc2t_clienttokenauth.h"
#include "game/rpcc2h_startmatch.h"


static BaseLink *FindLink(const char *linkName)
{
	return NULL;
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcT2C_Heartbeat());
	CRpc::RegistRpc(RpcC2L_LoginReq::CreateRpc());
	CRpc::RegistRpc(RpcC2T_EnterGameTest::CreateRpc());
	CRpc::RegistRpc(RpcC2T_CreateActorCli2Gate::CreateRpc());
	CRpc::RegistRpc(RpcC2T_ClientTokenAuth::CreateRpc());
	CRpc::RegistRpc(RpcC2H_StartMatch::CreateRpc());


	CProtocol::RegistProtoName(PTCT2C_HEARTBEAT_TYPE, "PTCT2C_HEARTBEAT");
	CProtocol::RegistProtoName(RPCC2L_LOGINREQ_TYPE, "RPCC2L_LOGINREQ");
	CProtocol::RegistProtoName(RPCC2T_ENTERGAMETEST_TYPE, "RPCC2T_ENTERGAMETEST");
	CProtocol::RegistProtoName(RPCC2T_CREATEACTORCLI2GATE_TYPE, "RPCC2T_CREATEACTORCLI2GATE");
	CProtocol::RegistProtoName(RPCC2T_CLIENTTOKENAUTH_TYPE, "RPCC2T_CLIENTTOKENAUTH");
	CProtocol::RegistProtoName(RPCC2H_STARTMATCH_TYPE, "RPCC2H_STARTMATCH");

}