#include "pch.h"
#include "createactortask.h"
#include "SQLStmt.h"
#include "crpc.h"
#include "timeutil.h"

CreateActorTask::CreateActorTask(UINT32 rpcid)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	m_rpcid = rpcid;
}

CreateActorTask::~CreateActorTask()
{

}



int CreateActorTask::Execute(MYSQL *mysql)
{

	{
		//查看用户名是否重复
		const char query[] = "INSERT INTO gameactor(AccountID,SeqIndex,ActorName,ActorType,ResID,HairStyle,Sex,CreateTime) VALUES\
							 (?,?,?,?,?,?,?,?)";
		SQLStmt<8, 0> stmt(mysql);
		UINT64 AccountID = m_arg.accountid();
		stmt.BindIn(&AccountID);
		UINT32 SeqIndex = m_arg.seqindex();
		stmt.BindIn(&SeqIndex);
		std::string name = m_arg.name();
		stmt.BindIn(const_cast<char*>(name.c_str()), name.size());
		UINT32  actorType = m_arg.actortype();
		stmt.BindIn(&actorType);
		UINT32  resID = m_arg.resid();
		stmt.BindIn(&resID);
		UINT32  hairStyle = m_arg.hairid();
		stmt.BindIn(&hairStyle);
		UINT32  sex = m_arg.sex();
		stmt.BindIn(&sex);
		UINT32  ntime = TimeUtil::GetTime();
		stmt.BindIn(&ntime);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}
	
	}

	{
		const char query[] = "update gameuser set State=? where AccountID =?";
		SQLStmt<2, 0> stmt(mysql);
		int userid = 0;
		UINT64 state = 2;
		stmt.BindIn(&state);
		UINT64 accountID = m_arg.accountid();
		stmt.BindIn(&accountID);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		m_result = 0;
	}

	return 0;
}


void CreateActorTask::FinishTask()
{
	if (m_result != 0)
	{
		LogError("sql error[%d]", m_result);
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_rpcid, oUserData);
		return;
	}

	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 200)
	{
		LogWarn("user reg task finish use time %llu ms", qwTimeUsed);
	}
	CUserData oUserData;
	oUserData.m_dwUserData = 0;
	oUserData.m_pUserPtr = &m_arg;
	CRpc::ReplyDelayRpc(m_rpcid, oUserData);
}

void CreateActorTask::SetData(CBR::CreateActorReq &data)
{
	m_arg = data;
}

