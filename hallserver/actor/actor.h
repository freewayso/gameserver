#ifndef __ACTOR_H
#define __ACTOR_H

#include "pb/project.pb.h"

class Actor
{

public:
	Actor();
	~Actor();
	void			Init(const CBR::ActorBase &acBase,UINT64 sessionID);
	void			Uninit();
	void			OnLogin();
	void			AddExp(UINT64 exp, bool isnewexp=true);
	void			AddRealExp(UINT64 exp);
	void			Levelup();
	void			Save();
	UINT64			GetID();
	int				GetActorType();
	void			SetActorType(int prof);
	void			Update(float delta);
	void			OnLoginOut();
	void			SetSessionID(SessionType sessionID) { m_sessionID = sessionID; }
	SessionType		GetSessionID() {return m_sessionID; }
	void			SetAccount(UINT64 accountid)	 { m_qwAccountID = accountid; }
	const UINT64	GetAccount() const{ return m_qwAccountID; }
	const std::string &GetName() const	{ return m_szActorName; }
	void			SetName(const std::string& str){ m_szActorName = str; }
	UINT32			GetResId(){return m_dwResId;}
	void			SetResId(UINT32 dwResId) { m_dwResId = dwResId;}
	UINT32			GetVigor() {return m_dwVigor;}
	void			SetVigor(UINT32 dwVigor){m_dwVigor=dwVigor; }
	UINT64			GetTotalExp(){ return m_qwTotalExp;}
	void			SetTotalExp(UINT32 qwTotalExp) { m_qwTotalExp = qwTotalExp; }
	UINT32			GetCreateTime(){ return m_dwCreateTime;}
	void			SetCreateTime(UINT32 dwCreateTime) { m_dwCreateTime = dwCreateTime; }
	void			SetHairStyle(UINT32 dwHairStyle) {m_dwHairStyle = dwHairStyle;}
	UINT32			GetHairStyle() { return m_dwHairStyle;}
	void			SetSex(UINT32 dwSex){ m_dwSex = dwSex;}
	UINT32			GetSex(){ return m_dwSex;}
	UINT32			GetLevel(){ return m_dwLevel;}
	void			SetLevel(UINT32 dwLevel) {m_dwLevel = dwLevel;}
	UINT32			GetScore() { return m_dwScore;}
	void			SetScore(UINT32 dwScore) { m_dwScore = dwScore;}
	UINT32			GetFire() { return m_dwFire;}
	void			SetFire(UINT32 dwFire) { m_dwFire = dwFire;}
	UINT32			GetVipLevel(){return m_dwVipLevel;}

public:
	void			SetSaveFlag(bool bFlag){ m_bSaveFlag = bFlag;}
	void			UpdatePerSecond();
private:
	SessionType	m_sessionID;
	UINT64      m_qwID;
	UINT64      m_qwExp;
	UINT64		m_qwTotalExp;
	UINT32		m_dwActorType;
	std::string m_szActorName;
	UINT64		m_qwAccountID;
	UINT32		m_dwResId;
	UINT32		m_dwVigor;
	UINT32      m_dwCreateTime;
	UINT32		m_dwHairStyle;
	UINT32		m_dwSex;
	UINT32		m_dwLevel;
	UINT32		m_dwScore;
	UINT32		m_dwFire;
	UINT32		m_dwFighting;
	UINT32		m_dwVipLevel;
//´æµµ
	UINT64 m_qwPrevSecTick;
	bool m_bSaveFlag;

};


#endif