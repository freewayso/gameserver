#include "pch.h"
#include "userregtask.h"
#include "SQLStmt.h"
#include "timeutil.h"
#include "crpc.h"


UserRegTask::UserRegTask(UINT32 rpcid)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	m_rpcid = rpcid;
}

UserRegTask::~UserRegTask()
{

}

int UserRegTask::Execute(MYSQL *mysql)
{
	std::string name = m_arg.name();
	{
		const char query[] = "INSERT INTO gameuser(AccountName, ChannelID,PermitPwd,IsBind,BindAccountID,BindTime,DeviceId,IP,RegTime,STATE) VALUES\
							  (?,?,?,?,?,FROM_UNIXTIME(?),?,?,FROM_UNIXTIME(?),0)";
		SQLStmt<9, 0> stmt(mysql);
		stmt.BindIn(&name[0], name.size());
		int channel = m_arg.channnel();
		stmt.BindIn(&channel);
		std::string passward =  m_arg.password();
		stmt.BindIn(&passward[0],passward.size());
		int isbind =0;
		stmt.BindIn(&isbind);
		int bindaccountid =0;
		stmt.BindIn(&bindaccountid);
		int bindtime =1597237197;
		stmt.BindIn(&bindtime);
		std::string deviceid =  m_arg.deviceid();
		stmt.BindIn(&deviceid[0],deviceid.size());
		std::string ip =  m_arg.ip();
		stmt.BindIn(&ip[0],ip.size());
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
		m_result = 0;

	}


	{
		const char query[] = "select AccountID from gameuser where AccountName = ?";
		SQLStmt<1, 1> stmt(mysql);
		UINT64 accountid = 0;

		stmt.BindIn(&name[0],name.size());
		stmt.BindOut(&accountid);
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
			m_res.set_accountid(accountid);
			LogInfo("account name is sucess create");
			return 0;
		}
		
	}

	return 0;
}

void UserRegTask::FinishTask()
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

void UserRegTask::SetData(CBR::RegAccountArg &data)
{
	m_arg = data;
}
