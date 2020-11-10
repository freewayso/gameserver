#ifndef _READDB_MGR_H__
#define _READDB_MGR_H__

#include "pb/project.pb.h"




enum state
{
	eUntreated 		= 0, //初始化
	ePostRedis 		= 1, //投递给REDIS
	eRedisResponse 	= 2, //REDIS返回
	ePostMysql		= 3, //投递到MYSQL
	eMysqlReponse   = 4, //MYSQL respone返回
};

struct DbReadEvent
{
	UINT64 qwUid;		//事件id
	UINT64 qwActorId;	//投递过来角色id
	UINT64 qwPostTime;	//投递过来事件
	state dwState;
	UINT32 dwRpcId;
	CBR::ActorDataArg oArg;
	DbReadEvent()
	{
		qwUid			= 0;
		qwActorId		= 0;
		qwPostTime		= 0;
		dwState			= (state)0;
		dwRpcId = 0;
	}
};

class DbEventConvMgr
{
	DbEventConvMgr();
	~DbEventConvMgr();
	DECLARE_SINGLETON(DbEventConvMgr)
public:
	bool Init();
	void Uninit();
	UINT64 CreateUuid();
	void AddEvent(DbReadEvent *pEvent,const CBR::ActorDataArg &oData);
	void OnMysqlEvent(UINT64 qwEventId,const CUserData &userData);
	void OnRedisEvent(UINT64 qwEventId,const CUserData &userData);
private:
	UINT32 m_dwId;
	std::map<UINT64, DbReadEvent *> m_mapEvents;
};


#endif