#ifndef _Redis_Mgr_H__
#define _Redis_Mgr_H__

class CRedisThread;
class CRedisTask;
struct redisContext;
class CRedisMgr
{
	CRedisMgr();
	~CRedisMgr();
	DECLARE_SINGLETON(CRedisMgr)

public:
	bool Init();
	void Uninit();

	void Process();
	void PushTask(CRedisTask* poTask, UINT32 dwThrdIndex=0);
	CRedisThread* GetThread(UINT32 dwIndex);
private:
	redisContext*		m_poRedisConn;
	typedef std::vector<CRedisThread*> CRedisThreadList;
	CRedisThreadList	m_oThreadList;
};

#endif