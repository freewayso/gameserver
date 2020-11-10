#include "pch.h"
#include "actormgr.h"
#include "util.h"



INSTANCE_SINGLETON(ActorMgr)

ActorMgr::ActorMgr()
{
	m_dwTime = 0;
}

ActorMgr::~ActorMgr()
{

}

bool ActorMgr::Init()
{
	return true;
}

void ActorMgr::Uninit()
{

}

void ActorMgr::AddActor(Actor *pActor)
{
	if(GetActorByID(pActor->GetID()) != NULL)
	{
		LogError("Actor id [%llu], session [%llu] already exist", pActor->GetID(), pActor->GetSessionID());
	}
	m_RoleDatas.insert(pActor);	
}


void ActorMgr::RemoveActor(Actor *pActor)
{
	m_RoleDatas.erase<SessionIndex>(pActor->GetSessionID());
}


Actor * ActorMgr::GetActorByID(SessionType sessionID)
{
	return m_RoleDatas.find<SessionIndex>(sessionID);
}

Actor * ActorMgr::GetBySession(SessionType sessionID)
{
	return m_RoleDatas.find<SessionIndex>(sessionID);
}

Actor * ActorMgr::Login(const CBR::ActorBase *pAcBase, UINT64 sessionID)
{
	Actor *pActor = new Actor;
	pActor->Init(*pAcBase, sessionID);
	AddActor(pActor);
	LogInfo("Actor enter hall {sessionid=%llu account=%llu actorid=%llu, name=%s} Login",sessionID ,pActor->GetAccount(), pActor->GetID(), pActor->GetName().c_str());
	pActor->OnLogin();
	return pActor;
}

void ActorMgr::Update(float delta)
{
	static float elapse = 0;
	elapse += delta;
	if (elapse > 60)
	{
		elapse -= 60;
	}
	ForeachCall(m_RoleDatas, Update, delta);
}

void ActorMgr::LoginOut(UINT64 sessionID)
{
	Actor *pActor = GetBySession(sessionID);
	if (pActor)
	{
		pActor->OnLoginOut();
		RemoveBySession(sessionID);
	}else
	{
		LogError("cannot find the actor session=[%llu]!!",sessionID);
	}
	LogInfo("actor login out the session is %llu",sessionID);
}

void ActorMgr::RemoveBySession(SessionType sessionID)
{
	Actor *pActor = GetBySession(sessionID);
	if (pActor != NULL)
	{
		LogDebug("Remove role, id [%llu], session [%llu]", pActor->GetID(), pActor->GetSessionID());
		RemoveActor(pActor);
		pActor->Uninit();
		delete pActor;
	}
	else
	{
		LogWarn("Remove role by session [%llu], but not found", sessionID);
	}
}
