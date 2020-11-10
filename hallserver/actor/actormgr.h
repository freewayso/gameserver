#ifndef _ACTORMGR_H
#define _ACTORMGR_H

#include "multiindex_map.h"
#include "actor.h"

class Actor;
class ActorMgr
{
	struct SessionIndex
	{
		typedef SessionType key;
		key Key(Actor *r){ return r->GetSessionID(); }
	};

	struct ActorIDIndex
	{
		typedef UINT64 key;
		key Key(Actor *r){ return r->GetID(); }
	};


public:
	ActorMgr();
	~ActorMgr();
	DECLARE_SINGLETON(ActorMgr)
	bool Init();
	void Uninit();
	void AddActor(Actor *pActor);
	void RemoveActor(Actor *pActor);
	Actor *GetActorByID(UINT64 actorid);
	Actor * GetBySession(SessionType sessionID);
	Actor *Login(const CBR::ActorBase *pAcBase, UINT64 sessionID);
	void LoginOut(UINT64 sessionID);
	void RemoveBySession(SessionType sessionID);
	void Update(float delta);
private:
	mutliindex_map_ptr<Actor, SessionIndex, ActorIDIndex> m_RoleDatas;
	UINT32 m_dwTime;

};




#endif 

