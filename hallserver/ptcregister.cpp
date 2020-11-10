#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2020/9/15 20:40:10

#include "login/ptct2h_loginoutntf.h"
#include "game/ptcc2h_cancelmatch.h"
#include "common/rpct2h_registergatetohall.h"
#include "common/rpch2d_registerhalltodb.h"
#include "game/rpch2d_updateactordata.h"
#include "game/rpct2h_getactors.h"
#include "game/rpch2d_queryactordb.h"
#include "game/rpct2h_createactorgate2hall.h"
#include "game/rpch2d_createactorhall2db.h"
#include "game/rpch2m_reghall2mdc.h"
#include "game/rpcc2h_startmatch.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcT2H_LoginOutNtf());
	CProtocol::Register(new PtcC2H_CancelMatch());
	CRpc::RegistRpc(RpcT2H_RegisterGateToHall::CreateRpc());
	CRpc::RegistRpc(RpcH2D_RegisterHallToDb::CreateRpc());
	CRpc::RegistRpc(RpcH2D_UpdateActorData::CreateRpc());
	CRpc::RegistRpc(RpcT2H_GetActors::CreateRpc());
	CRpc::RegistRpc(RpcH2D_QueryActorDb::CreateRpc());
	CRpc::RegistRpc(RpcT2H_CreateActorGate2Hall::CreateRpc());
	CRpc::RegistRpc(RpcH2D_CreateActorHall2Db::CreateRpc());
	CRpc::RegistRpc(RpcH2M_RegHall2Mdc::CreateRpc());
	CRpc::RegistRpc(RpcC2H_StartMatch::CreateRpc());


	CProtocol::RegistProtoName(PTCT2H_LOGINOUTNTF_TYPE, "PTCT2H_LOGINOUTNTF");
	CProtocol::RegistProtoName(PTCC2H_CANCELMATCH_TYPE, "PTCC2H_CANCELMATCH");
	CProtocol::RegistProtoName(RPCT2H_REGISTERGATETOHALL_TYPE, "RPCT2H_REGISTERGATETOHALL");
	CProtocol::RegistProtoName(RPCH2D_REGISTERHALLTODB_TYPE, "RPCH2D_REGISTERHALLTODB");
	CProtocol::RegistProtoName(RPCH2D_UPDATEACTORDATA_TYPE, "RPCH2D_UPDATEACTORDATA");
	CProtocol::RegistProtoName(RPCT2H_GETACTORS_TYPE, "RPCT2H_GETACTORS");
	CProtocol::RegistProtoName(RPCH2D_QUERYACTORDB_TYPE, "RPCH2D_QUERYACTORDB");
	CProtocol::RegistProtoName(RPCT2H_CREATEACTORGATE2HALL_TYPE, "RPCT2H_CREATEACTORGATE2HALL");
	CProtocol::RegistProtoName(RPCH2D_CREATEACTORHALL2DB_TYPE, "RPCH2D_CREATEACTORHALL2DB");
	CProtocol::RegistProtoName(RPCH2M_REGHALL2MDC_TYPE, "RPCH2M_REGHALL2MDC");
	CProtocol::RegistProtoName(RPCC2H_STARTMATCH_TYPE, "RPCC2H_STARTMATCH");

}