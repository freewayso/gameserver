#include "pch.h"
#include "LogInit.h"
#include "actor.h"
#include "timeutil.h"
#include "dblink.h"
#include "game/rpch2d_updateactordata.h"


Actor::Actor()
{
	m_qwPrevSecTick = TimeUtil::GetFrameTime();
	m_bSaveFlag     =  false;
}



Actor::~Actor()
{
}

void Actor::Init(const CBR::ActorBase &acBase,UINT64 sessionID)
{
	m_sessionID  = sessionID;
	m_qwAccountID  = acBase.accountid(); 
	m_dwActorType = acBase.app().actortype();
	m_szActorName  = acBase.app().actorname();
	m_qwID  = acBase.app().actid();
	m_qwExp = acBase.att().actorexp();
	m_dwResId = acBase.app().resid();
	m_dwSex = acBase.app().sex();
	m_dwVigor = acBase.att().vigor();
	m_dwCreateTime = acBase.app().createtime();
	m_dwHairStyle = acBase.app().hairstyle();
	m_dwFire = acBase.att().fire();
	m_dwScore = acBase.att().score();

}


void Actor::Uninit()
{

}

void Actor::OnLogin()
{
	
}

int Actor::GetActorType()
{
	return m_dwActorType;
}

void Actor::SetActorType(int prof)
{
	m_dwActorType = prof;
}


void Actor::Update(float delta)
{
	UINT64 qwTickNow = TimeUtil::GetFrameTime();
	if(qwTickNow > m_qwPrevSecTick && qwTickNow - m_qwPrevSecTick >= 1000)
	{
		UpdatePerSecond();
		m_qwPrevSecTick = qwTickNow;
	}
}



void Actor::OnLoginOut()
{
	LogInfo("actor id=[%llu] name=[%s] login out,and save actor data",m_qwID,m_szActorName.c_str());
	Save();
}


void Actor::UpdatePerSecond()
{
	if (m_bSaveFlag)
	{
		Save();
	}
	m_bSaveFlag = false;
	//存储标志
}

void Actor::AddExp(UINT64 exp, bool isnewexp)
{
	m_qwExp += exp;
}

void Actor::AddRealExp(UINT64 exp)
{

}

void Actor::Levelup()
{

}

void Actor::Save()
{
	//修改存储标签
	RpcH2D_UpdateActorData  *pRpc = RpcH2D_UpdateActorData::CreateRpc();
	CBR::ActorBase* pActBase = pRpc->m_oArg.mutable_act();
	CBR::appear* pAppear = pActBase->mutable_app();
	pAppear->set_actid(m_qwID);
	pAppear->set_resid(m_dwResId);
	pAppear->set_sex(m_dwSex);
	pAppear->set_hairstyle(m_dwHairStyle);
	CBR::AttriBase* pAttrBase = pActBase->mutable_att();
	pAttrBase->set_score(m_dwScore);
	pAttrBase->set_actorexp(m_qwExp);
	pAttrBase->set_vigor(m_dwVigor);
	pAttrBase->set_fire(m_dwFire);
	pAttrBase->set_fighting(m_dwFighting);
	pAttrBase->set_viplev(m_dwVipLevel);
	DBLink::Instance()->SendTo(*pRpc);

}

UINT64 Actor::GetID() 
{
	return m_qwID;
}
