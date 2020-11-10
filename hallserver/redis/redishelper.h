#ifndef _Redis_Helper_H__
#define _Redis_Helper_H__

#include "redis/hiredis.h"
#include <vector>

class RedisResult
{
public:
	RedisResult(redisReply* reply){m_poRedisReply = reply;}
	~RedisResult();
	bool IsResultValid();
	bool Free();

	int GetResultType();
	bool GetString(std::string& res);

	int GetElementsSize();
	int GetElementType(UINT32 index);
	bool GetElement(UINT32 index, std::string& ele);
private:
	redisReply* m_poRedisReply;
};

class RedisConn;
class CRedisHelper
{
	CRedisHelper(){}
	~CRedisHelper(){}

public:

	static RedisResult* GetRedisString(RedisConn* poRedisConn, const char* table, const char* key);
	static RedisResult* MGetRedisString(RedisConn* poRedisConn, const char* table, /*const char** key*/std::vector<std::string>& key);
	static RedisResult* SetRedisTimeLiness(RedisConn* poRedisConn, const char* table, const char* key, UINT32 nTime);
	static RedisResult* SetRedisString(RedisConn* poRedisConn, const char* table, const char* key, const std::string& value);
	static bool CheckReply(redisReply* reply);
};


class RedisConn
{
	friend class CRedisHelper;
public:
	RedisConn();

	virtual ~RedisConn();

	virtual bool Connect(const char* pszHost, UINT32 dwPort, const char* pszPwd);

	// 关闭连接
	//virtual bool Close(){}

	//virtual bool Query(const char* pszSQL, UINT32 dwLen, UINT32* pdwAffectedRows);

	// 获取redis错误码
	virtual UINT32 GetErrno();

	// 获取redis错误描述信息
	virtual const char* GetError();


	//virtual void Release();
private:
	bool Reconnect();

private:
	redisContext*	m_poRedisConn;
	std::string m_defaultError;
	int m_defaultErrNo;
	//redisReply* m_poRedisReply;
};


//IMysqlConn* CreateMysqlConn();

#endif
