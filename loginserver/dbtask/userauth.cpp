#include "pch.h"
#include "userauth.h"
#include "SQLStmt.h"
#include "timeutil.h"
#include "crpc.h"
#include "session/sessionmgr.h"

 UINT64 UserAuthTask::SessionCounter = 0;

 //流程调整，根据机器码做标准...

UserAuthTask::UserAuthTask(UINT32 rpcid)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	m_rpcid = rpcid;
}

UserAuthTask::~UserAuthTask()
{

}

int UserAuthTask::Execute(MYSQL *mysql)
{
	UINT64 accountid = 0;
	bool flag = false;
	std::string deviceId = m_arg.deviceid();
	{
		const char query[] = "select AccountID,AccountName from gameuser where DeviceId = ?";
		SQLStmt<1, 2> stmt(mysql);
		stmt.BindIn(const_cast<char*>(m_arg.deviceid().c_str()), m_arg.deviceid().size());
		stmt.BindOut(&accountid);
		char accountname[128];
		stmt.BindOut(accountname,128);
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
		if (stmt.FetchResult())
		{
			LogInfo("autu sucess");
			m_res.set_accoutid(accountid);
			m_res.set_accountname(accountname);
			m_result = 0;
			return 0;
		}
	}

	{

		const char query[] = "INSERT delayed gameuser(AccountID,AccountName, ChannelID,PermitPwd,IsBind,BindAccountID,BindTime,DeviceId,IP,RegTime,STATE) VALUES\
							 (?,?,?,?,?,?,FROM_UNIXTIME(?),?,?,FROM_UNIXTIME(?),0)";
		SQLStmt<10, 0> stmt(mysql);
		accountid = SessionManager::Instance()->CreateUUID();
		stmt.BindIn(&accountid);
		stmt.BindIn(const_cast<char*>(m_arg.deviceid().c_str()), m_arg.deviceid().size());
		int channel = 0;
		stmt.BindIn(&channel);
		std::string passward = "";
		stmt.BindIn(const_cast<char*>(passward.c_str()), passward.size());
		int isbind =0;
		stmt.BindIn(&isbind);
		int bindaccountid =0;
		stmt.BindIn(&bindaccountid);
		int bindtime =0;
		stmt.BindIn(&bindtime);
		stmt.BindIn(&deviceId[0],deviceId.size());
		stmt.BindIn(const_cast<char*>(m_arg.ip().c_str()), m_arg.ip().size());
		int regtime = TimeUtil::GetTime();
		stmt.BindIn(&regtime);
		int state = 1;
		stmt.BindIn(&state);
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
		m_res.set_accoutid(accountid);
		m_res.set_accountname(deviceId);
		m_result = 0;
	
	}

	return 0;
}




void UserAuthTask::FinishTask()
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
		LogWarn("user login task finish use time %llu ms", qwTimeUsed);
	}

	CUserData oUserData;
	oUserData.m_dwUserData = 0;
	oUserData.m_pUserPtr = &m_res;
	CRpc::ReplyDelayRpc(m_rpcid, oUserData);



}

void UserAuthTask::SetData(CBR::LoginDataArg &data)
{
	m_arg = data;
}
