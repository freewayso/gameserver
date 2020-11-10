#include "pch.h"
#include "actorinserttask.h"

#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"
#include "timeutil.h"


ActorInsertTask::ActorInsertTask():CMysqlTask("ActorInsertTask"),m_rpcId(0),m_id(0)
{
}

ActorInsertTask::~ActorInsertTask()
{ 
}

void ActorInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	SqlJoin columns;
	columns.addPair("AccountID", m_data.accountid());
	columns.addPair("SeqIndex", m_data.seqindex());
	columns.addPair("ActorName", m_data.name());
	columns.addPair("ActorType", m_data.actortype());
	columns.addPair("ResID", m_data.resid());
	columns.addPair("HairStyle", m_data.hairid());
	columns.addPair("Sex", m_data.sex());
	columns.addPair("CreateTime", TimeUtil::GetTime());

	std::string sql;
	SqlBuilder::BuildInsertSql(poMysqlConn, sql, "gameactor", columns);

	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	UINT64 qwInsertID = 0;
	if(!poMysqlConn->GetLastInsertID(qwInsertID)) return;
	m_id = qwInsertID;

	m_info.set_actorid(m_id);
	m_info.set_result(CBR::ERR_SUCCESS);
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void ActorInsertTask::OnReply()
{
	CBR::ErrorCode nErrCode = CBR::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = CBR::ERR_FAILED;
		LogError("Insert actor failed[%llu], errMsg: %s", m_id, GetErrStream().str().c_str());
	}
	CUserData oUser((UINT32)nErrCode, (void*)&m_info);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
