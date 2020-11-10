#include "pch.h"
#include "login/rpcc2l_loginreq.h"
#include "login/rpcc2l_createactor.h"
#include "LogInit.h"
#include "reconnetmgr.h"
#include "session/usermgr.h"
#include "robot/robotmgr.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2020/8/14 14:45:42

RPC_CLIENT_IMPLEMETION(RpcC2L_LoginReq, LoginDataArg, LoginDataRes)

void RpcC2L_LoginReq::OnReply(const LoginDataArg &roArg, const LoginDataRes &roRes, const CUserData &roUserData)
{
	LogDebug("-------------RpcC2L_LoginReq------------------ip=[%s] port=[%u] session=[%llu] accountid=[%llu]",roRes.ip().c_str(),roRes.port(),roRes.session(),roRes.accoutid());
	AccountSession *session	= new AccountSession();
	session->qwAccountId = roRes.accoutid();
	session->qwToken  =  roRes.session();
	session->dwState = roRes.state();
	session->dwTime  = TimeUtil::GetTime();
	if (roRes.actorlist().size())
	{
		LogDebug("login sucess get actid=[%lu]",roRes.actorlist(0).id());
		session->qwActorId = roRes.actorlist(0).id();
	}
	LoginLink*  pLinker = RobotMgr::Instance()->FindLinker(m_dwConnID);
	if (pLinker)
	{
		RobotMgr::Instance()->DisConnect(m_dwConnID);
	}
	SessionMgr::Instance()->AddUserSession(session);
	RobotMgr::Instance()->addClient(roRes.ip(),roRes.port(),roRes.accoutid());

}

void RpcC2L_LoginReq::OnTimeout(const LoginDataArg &roArg, const CUserData &roUserData)
{


}
