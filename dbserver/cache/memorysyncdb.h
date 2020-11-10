#ifndef _MEMORYSYNCDB_MGR_H__
#define _MEMORYSYNCDB_MGR_H__

#include "pb/project.pb.h"

enum eCacheState
{
	DATA_DEFAULT        = 0,//默认初始化状态 
	DATA_UPDATE			= 1,//数据已经变更了
	DATA_DELAY_SUBMIT	= 2,//数据delay提交mysql
	DATA_NOW_SUBMIT	    = 3,//数据里面提交mysql
	DATA_FINISH_SUBMIT  = 4,//数据提交完成了
};


struct stActorData
{
	CBR::ActorBase data;
	eCacheState state;
	UINT32 dwTime;			//数据变更的事件
	UINT32 dwCommitTime;	// 数据提交mysql事件
	stActorData()
	{
		state		 = DATA_DEFAULT;
		dwTime		 = 0;
		dwCommitTime = 0;
	}
};

class MemorySyncDb
{
public:
	MemorySyncDb();
	~MemorySyncDb();
	DECLARE_SINGLETON(MemorySyncDb)
public:
	bool Init();
	void Uninit();
	void Update();
	void OnLogin(const CBR::ActorBase &data);
	void OnLoginOut(UINT64 qwActorId);
	void AddData(const CBR::ActorBase &data,eCacheState state = DATA_DELAY_SUBMIT);
	bool SyncData2Redis(const CBR::ActorBase &data,bool flag=true);
	CBR::ActorBase *GetActorBase(UINT64 qwActorId);
private:
	UINT32 m_dwTime;		//下一次保存时间
	UINT32 m_dwVersion;		//version默认都是0开始自增
	std::unordered_map<UINT64, stActorData> m_mapActors;
	

};


#endif