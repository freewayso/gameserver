#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2020/9/15 20:40:10

#include "login/ptcl2t_loginsession2gatentf.h"
#include "login/ptct2h_loginoutntf.h"
#include "game/ptct2c_heartbeat.h"
#include "game/ptcc2h_cancelmatch.h"
#include "common/rpct2h_registergatetohall.h"
#include "common/rpct2l_registergate2login.h"
#include "game/rpcc2t_entergametest.h"
#include "game/rpct2h_getactors.h"
#include "game/rpct2h_createactorgate2hall.h"
#include "game/rpcc2t_createactorcli2gate.h"
#include "game/rpcc2t_clienttokenauth.h"
#include "game/rpcc2h_startmatch.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcL2T_LoginSession2GateNtf());
	ProtocolForward::RegistForward(PTCC2H_CANCELMATCH_TYPE, FindLink("hslink"));
	CRpc::RegistRpc(RpcT2H_RegisterGateToHall::CreateRpc());
	CRpc::RegistRpc(RpcT2L_RegisterGate2Login::CreateRpc());
	CRpc::RegistRpc(RpcC2T_EnterGameTest::CreateRpc());
	CRpc::RegistRpc(RpcT2H_GetActors::CreateRpc());
	CRpc::RegistRpc(RpcT2H_CreateActorGate2Hall::CreateRpc());
	CRpc::RegistRpc(RpcC2T_CreateActorCli2Gate::CreateRpc());
	CRpc::RegistRpc(RpcC2T_ClientTokenAuth::CreateRpc());
	RpcForward::RegistForward(RPCC2H_STARTMATCH_TYPE, FindLink("hslink"), FindLink("clientlink"));


	CProtocol::RegistProtoName(PTCL2T_LOGINSESSION2GATENTF_TYPE, "PTCL2T_LOGINSESSION2GATENTF");
	CProtocol::RegistProtoName(PTCT2H_LOGINOUTNTF_TYPE, "PTCT2H_LOGINOUTNTF");
	CProtocol::RegistProtoName(PTCT2C_HEARTBEAT_TYPE, "PTCT2C_HEARTBEAT");
	CProtocol::RegistProtoName(PTCC2H_CANCELMATCH_TYPE, "PTCC2H_CANCELMATCH");
	CProtocol::RegistProtoName(RPCT2H_REGISTERGATETOHALL_TYPE, "RPCT2H_REGISTERGATETOHALL");
	CProtocol::RegistProtoName(RPCT2L_REGISTERGATE2LOGIN_TYPE, "RPCT2L_REGISTERGATE2LOGIN");
	CProtocol::RegistProtoName(RPCC2T_ENTERGAMETEST_TYPE, "RPCC2T_ENTERGAMETEST");
	CProtocol::RegistProtoName(RPCT2H_GETACTORS_TYPE, "RPCT2H_GETACTORS");
	CProtocol::RegistProtoName(RPCT2H_CREATEACTORGATE2HALL_TYPE, "RPCT2H_CREATEACTORGATE2HALL");
	CProtocol::RegistProtoName(RPCC2T_CREATEACTORCLI2GATE_TYPE, "RPCC2T_CREATEACTORCLI2GATE");
	CProtocol::RegistProtoName(RPCC2T_CLIENTTOKENAUTH_TYPE, "RPCC2T_CLIENTTOKENAUTH");
	CProtocol::RegistProtoName(RPCC2H_STARTMATCH_TYPE, "RPCC2H_STARTMATCH");

}