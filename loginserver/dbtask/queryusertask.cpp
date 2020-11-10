#include "pch.h"
#include "queryusertask.h"
#include "crpc.h"
#include "insertusertask.h"
#include "session/sessionmgr.h"
#include "login/mysqlmgr.h"
#include "timeutil.h"
#include "utildb/sqlbuilder.h"

QueryUserTask::QueryUserTask():CMysqlTask("LoginTask"),m_szStoreKey(""), m_dwRpcDelayed(0)
{
}

QueryUserTask::~QueryUserTask()
{
	
}


void QueryUserTask::GetActorByAccount(UINT64 qwAccountId,IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::stringstream ss;
	ss << "select `ActorID`,`ActorName` from gameactor where AccountID =";
	ss << qwAccountId;

	IMysqlRecordSet* poRes = NULL;
	if (!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		m_res.set_result(CBR::ERR_FAILED);
		return;
	}
	std::vector<UINT64> oExpired;
	while (poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		if (!poRes->GetFieldValue(0, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'ActorID' failed";
			poRes->Release();
			return;
		}
		CBR::ActorBrief *pBrief = m_res.add_actorlist();
		pBrief->set_id(convert<UINT64>(pVal));
	
		std::string actorname;
		if (!poRes->GetFieldValue(1, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'ActorName' failed";
			poRes->Release();
			return;
		}
		actorname.assign(pVal, dwLen);
		pBrief->set_name(actorname);
		LogInfo("actor list actorid=[%llu] name=[%s]",convert<UINT64>(pVal),actorname.c_str());
	}
	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}


void QueryUserTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::stringstream ss;
	ss << "select `AccountID`,`AccountName` from gameuser where DeviceId ='";
	if(-1 == poMysqlConn->EscapeString(m_szStoreKey.c_str(), (INT32)m_szStoreKey.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";

	IMysqlRecordSet* poRes = NULL;
	if (!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		m_res.set_result(CBR::ERR_FAILED);
		return;
	}
	if (0 == poRes->GetRowNum())
	{
		SetMysqlError(MYSQL_ERR_FAILED);
		SqlJoin columns;
		UINT64 accountid = SessionManager::Instance()->CreateUUID();
		columns.addPair("AccountID", accountid);
		columns.addPair("AccountName",m_szStoreKey);
		columns.addPair("ChannelID", 1);
		columns.addPair("PermitPwd", "");
		columns.addPair("IsBind", 0);
		columns.addPair("BindAccountID", 0);
		columns.addPair("BindTime", TimeUtil::GetDateTimeStr(TimeUtil::GetTime()).c_str());
		columns.addPair("DeviceId", m_szStoreKey);
		columns.addPair("IP", "");
		columns.addPair("RegTime", TimeUtil::GetDateTimeStr(TimeUtil::GetTime()).c_str());
		columns.addPair("STATE", 1);
		LogInfo("userid =[%llu] accountname=[%s] is created sucessful",accountid,m_szStoreKey.c_str());
		std::string sql;
		SqlBuilder::BuildInsertSql(poMysqlConn, sql, "gameuser", columns);
		if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
		{
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			return;
		}
		SetMysqlError(MYSQL_ERR_SUCESS);
		m_res.set_accoutid(accountid);
		m_res.set_accountname(m_szStoreKey);
		m_res.set_result(CBR::ERR_SUCCESS);
	}
	else
	{
		UINT64 qwAccountId = 0;
		SetMysqlError(MYSQL_ERR_FAILED);
		poRes->FetchRow();
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		if (!poRes->GetFieldValue(0, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'AccountID' failed";
			poRes->Release();
			return;
		}
		qwAccountId = convert<UINT64>(pVal);
		m_res.set_accoutid(qwAccountId);
		std::string accountname;
		if (!poRes->GetFieldValue(1, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'AccountName' failed";
			poRes->Release();
			return;
		}
		SetMysqlError(MYSQL_ERR_SUCESS);
		accountname.assign(pVal, dwLen);
		m_res.set_accountname(accountname);
		m_res.set_result(CBR::ERR_SUCCESS);
		GetActorByAccount(qwAccountId,poMysqlConn,pcBuffer,nLength);

	}
	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}


void QueryUserTask::OnReply()
{
	if (m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("LoginTask failed, deviceid: %s, errMsg: %s", m_szStoreKey.c_str(), GetErrStream().str().c_str());
	}
	CUserData oUser;
	oUser.m_pUserPtr = (void*)&m_res;
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);

}