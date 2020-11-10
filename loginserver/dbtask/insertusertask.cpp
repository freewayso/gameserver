#include "pch.h"
#include "insertusertask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"
#include "timeutil.h"


InsertUserTask::InsertUserTask():CMysqlTask("InsertUserTask"),m_rpcId(0),m_id(0)
{
}

InsertUserTask::~InsertUserTask()
{ 
}

void InsertUserTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	SqlJoin columns;
	columns.addPair("AccountID", m_data.accountid());
	columns.addPair("AccountName", m_data.accountname());
	columns.addPair("ChannelID", m_data.channel());
	columns.addPair("PermitPwd", m_data.passward());
	columns.addPair("IsBind", m_data.isbind());
	columns.addPair("BindAccountID", m_data.bindaccountid());
	columns.addPair("BindTime", TimeUtil::GetDateTimeStr(m_data.bindtime()).c_str());
	columns.addPair("DeviceId", m_data.deviceid());
	columns.addPair("IP", m_data.ip());
	columns.addPair("RegTime", TimeUtil::GetDateTimeStr(m_data.regtime()).c_str());
	columns.addPair("STATE", m_data.state());
	LogInfo("userid =[%llu] accountname=[%s] is created sucessful",m_data.accountid(),m_data.accountname().c_str());
	std::string sql;
	SqlBuilder::BuildInsertSql(poMysqlConn, sql, "gameuser", columns);
	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void InsertUserTask::OnReply()
{
	CBR::ErrorCode nErrCode = CBR::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = CBR::ERR_FAILED;
		LogError("Insert gameuser failed[%llu], errMsg: %s", m_id, GetErrStream().str().c_str());
	}else
	{
		LogInfo("user[%llu] info is sucessfull created",m_data.accountid());
	}

}
