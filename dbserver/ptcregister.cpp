#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2020/9/10 16:36:07

#include "common/rpch2d_registerhalltodb.h"
#include "game/rpch2d_updateactordata.h"
#include "game/rpch2d_queryactordb.h"
#include "game/rpch2d_createactorhall2db.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CRpc::RegistRpc(RpcH2D_RegisterHallToDb::CreateRpc());
	CRpc::RegistRpc(RpcH2D_UpdateActorData::CreateRpc());
	CRpc::RegistRpc(RpcH2D_QueryActorDb::CreateRpc());
	CRpc::RegistRpc(RpcH2D_CreateActorHall2Db::CreateRpc());


	CProtocol::RegistProtoName(RPCH2D_REGISTERHALLTODB_TYPE, "RPCH2D_REGISTERHALLTODB");
	CProtocol::RegistProtoName(RPCH2D_UPDATEACTORDATA_TYPE, "RPCH2D_UPDATEACTORDATA");
	CProtocol::RegistProtoName(RPCH2D_QUERYACTORDB_TYPE, "RPCH2D_QUERYACTORDB");
	CProtocol::RegistProtoName(RPCH2D_CREATEACTORHALL2DB_TYPE, "RPCH2D_CREATEACTORHALL2DB");

}