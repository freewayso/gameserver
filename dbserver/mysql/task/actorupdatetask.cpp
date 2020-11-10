#include "pch.h"
#include "actorupdatetask.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"


ActorUpdateTask::ActorUpdateTask()
	:CMysqlTask("ActorUpdateTask")
	,m_dwStoreKey(0), m_dwRpcDelayed(0)
{
}

ActorUpdateTask::~ActorUpdateTask()
{
}

void ActorUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	IMysqlRecordSet* poRes = NULL;
	SqlJoin columns;

	if (m_oReq.has_app())
	{
		if (m_oReq.app().has_hairstyle())
		{
			columns.addPair("HairStyle",m_oReq.app().hairstyle());
		}
		if (m_oReq.app().has_resid())
		{
			columns.addPair("ResID",m_oReq.app().resid());
		}
		if (m_oReq.app().has_sex())
		{
			columns.addPair("Sex",m_oReq.app().sex());
		}
		if (m_oReq.app().has_actorname())
		{
			columns.addPair("ActorName",m_oReq.app().actorname());
		}
		if (m_oReq.app().has_createtime())
		{
			columns.addPair("CreateTime",m_oReq.app().createtime());
		}
	}

	if (m_oReq.has_att())
	{
		if (m_oReq.att().has_actorlevel())
		{
			columns.addPair("ActorLevel",m_oReq.att().actorlevel());
		}
		if (m_oReq.att().has_actorexp())
		{
			columns.addPair("ActorExp",m_oReq.att().actorexp());
		}
		if (m_oReq.att().has_vigor())
		{
			columns.addPair("Vigor",m_oReq.att().vigor());
		}
		if (m_oReq.att().has_score())
		{
			columns.addPair("Vigor",m_oReq.att().score());
		}
		if (m_oReq.att().has_fire())
		{
			columns.addPair("Fire",m_oReq.att().fire());
		}
		if (m_oReq.att().has_viplev())
		{
			columns.addPair("VipLev",m_oReq.att().vigor());
		}
	}
	SqlJoin cond;
	cond.addPair("ActorID", m_oReq.app().actid());

	std::string sql;
	SqlBuilder::BuildUpdateSql(poMysqlConn, sql, "gameactor", columns, cond);

	LogInfo("update actor sql=[%s]",sql.c_str());
	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void ActorUpdateTask::OnReply()
{
	CBR::ErrorCode nErrCode = CBR::ERR_SUCCESS;
	if (m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = CBR::ERR_FAILED;
		LogError("ActorUpdateTask failed, roleid: %llu, errMsg: %s", m_dwStoreKey, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	//这里存储成功把对应的在cache的数据清理掉，不过这里要小心.
	//CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}