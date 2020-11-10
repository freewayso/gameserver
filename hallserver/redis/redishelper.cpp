#include "redishelper.h"
#include <util.h>
#include "config.h"
#include "LogInit.h"

RedisResult* CRedisHelper::GetRedisString(RedisConn* poRedisConn, const char* table, const char* key)
{
	redisReply* reply = NULL;
	if (poRedisConn->m_poRedisConn)
	{
		reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, "get %s:%s:%s", table, key, HallConfig::Instance()->GetKeySuffix().c_str());
	}
	if (!CheckReply(reply))
	{
		if(poRedisConn->Reconnect())
			reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, "get %s:%s:%s", table, key, HallConfig::Instance()->GetKeySuffix().c_str());
	}
	RedisResult* result = new RedisResult(reply);
	return result;
}

RedisResult* CRedisHelper::MGetRedisString(RedisConn* poRedisConn, const char* table, std::vector<std::string>& key)
{
	std::stringstream ss;
	ss << "mget";
	for (int i = 0; i < key.size(); ++i)
	{
		ss << " "<< table << ":" << key[i].c_str() <<":" << HallConfig::Instance()->GetKeySuffix().c_str();
	}
	redisReply* reply = NULL;
	if (poRedisConn->m_poRedisConn)
	{
		reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, ss.str().c_str());
	}
	if (!CheckReply(reply))
	{
		if(poRedisConn->Reconnect())
			reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, ss.str().c_str());
	}
	RedisResult* result = new RedisResult(reply);
	return result;
}

//expireat key unixtime
RedisResult* CRedisHelper::SetRedisTimeLiness(RedisConn* poRedisConn, const char* table, const char* key, UINT32 time)
{
	redisReply* reply = NULL;
	if (poRedisConn->m_poRedisConn)
	{
		reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, "expireat %s:%s:%s %u", table, key,HallConfig::Instance()->GetKeySuffix().c_str(),  time);
	}
	if (!CheckReply(reply))
	{
		if(poRedisConn->Reconnect())
			reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, "expireat %s:%s:%s %u",  table, key,HallConfig::Instance()->GetKeySuffix().c_str(), time);
	}
	RedisResult* result = new RedisResult(reply);
	return result;
}



RedisResult* CRedisHelper::SetRedisString(RedisConn* poRedisConn, const char* table, const char* key, const std::string& value)
{
	//std::stringstream ss;
	redisReply* reply = NULL;
	if (poRedisConn->m_poRedisConn)
	{
		reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, "set %s:%s:%s %b", table, key, HallConfig::Instance()->GetKeySuffix().c_str(), value.c_str(), value.length());
	}
	if (!CheckReply(reply))
	{
		if(poRedisConn->Reconnect())
			reply = (redisReply*)redisCommand(poRedisConn->m_poRedisConn, "set %s:%s:%s %b", table, key, HallConfig::Instance()->GetKeySuffix().c_str(), value.c_str(), value.length());
	}
	RedisResult* result = new RedisResult(reply);
	return result;
}

bool CRedisHelper::CheckReply(redisReply* reply)
{
	if (!reply || reply->type == REDIS_REPLY_ERROR)
	{
		return false;
	}

	return true;
}



RedisConn::RedisConn()
{
	m_poRedisConn = NULL;
	m_defaultError = "default error";
	m_defaultErrNo = 0;
}

RedisConn::~RedisConn()
{
	if(m_poRedisConn != NULL)
	{
		redisFree(m_poRedisConn);
		m_poRedisConn = NULL;
	}
}

bool RedisConn::Connect(const char* pszHost, UINT32 dwPort, const char* pszPwd)
{
	struct timeval timeout = {3, 500000};
	m_poRedisConn = redisConnectWithTimeout(pszHost, dwPort, timeout);
	if (!m_poRedisConn)
	{
		LogError("Can't connect to redis %s:%u, null", pszHost, dwPort);
		return false;
	}
	if (m_poRedisConn->err)
	{
		redisFree(m_poRedisConn);

		LogError("Can't connect to redis %s:%u, ErrDesc:%s", pszHost, dwPort, m_poRedisConn->errstr);

		return false;
	}

	redisReply* reply = (redisReply*)redisCommand(m_poRedisConn, "auth %s", pszPwd);
	if (!reply || reply->type == REDIS_REPLY_ERROR)
	{
		LogError("auth error redis %s:%u.", pszHost, dwPort);
		freeReplyObject(reply);
		redisFree(m_poRedisConn);
		return false;
	}
	freeReplyObject(reply);

	LogInfo("Connect to redis %s:%u succ.", pszHost, dwPort);

	timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = 100000;
	redisSetTimeout(m_poRedisConn, tm);

	return true;
}

UINT32 RedisConn::GetErrno()
{
	if (m_poRedisConn)
	{
		return m_poRedisConn->err;
	}
	else
	{
		return m_defaultErrNo;
	}
}

const char* RedisConn::GetError()
{
	if (m_poRedisConn)
	{
		return m_poRedisConn->errstr;
	}
	else
	{
		return m_defaultError.c_str();
	}
}

bool RedisConn::Reconnect()
{
	redisFree(m_poRedisConn);
	m_poRedisConn = NULL;

	const RedisInfo& redisInfo = HallConfig::Instance()->GetRedisInfo();
	struct timeval timeout = {3, 500000};
	m_poRedisConn = redisConnect(redisInfo.m_ip.c_str(), convert<UINT32>(redisInfo.m_port.c_str()));
	if (!m_poRedisConn || m_poRedisConn->err)
	{
		if (m_poRedisConn)
		{
			m_defaultErrNo = m_poRedisConn->err;
			m_defaultError = m_poRedisConn->errstr;
		}
		redisFree(m_poRedisConn);
		m_poRedisConn = NULL;
		return false;
	}

	redisReply* reply = (redisReply*)redisCommand(m_poRedisConn, "auth %s", redisInfo.m_password.c_str());
	if (!reply || reply->type == REDIS_REPLY_ERROR)
	{
		m_defaultErrNo = m_poRedisConn->err;
		m_defaultError = m_poRedisConn->errstr;
		freeReplyObject(reply);
		redisFree(m_poRedisConn);
		m_poRedisConn = NULL;
		return false;
	}
	freeReplyObject(reply);

	timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = 100000;
	redisSetTimeout(m_poRedisConn, tm);

	return true;
}

RedisResult::~RedisResult()
{

}

bool RedisResult::IsResultValid()
{
	return m_poRedisReply != NULL;
}

bool RedisResult::Free()
{
	if (IsResultValid())
	{
		freeReplyObject(m_poRedisReply);
		m_poRedisReply = NULL;
	}
	delete this;

	return true;
}

int RedisResult::GetResultType()
{
	if (!IsResultValid())
	{
		return 0;
	}

	return m_poRedisReply->type;
}

bool RedisResult::GetString(std::string& res)
{
	if (!IsResultValid())
	{
		return false;
	}

	if (m_poRedisReply->type == REDIS_REPLY_STRING && m_poRedisReply->str)
	{
		res = std::string(m_poRedisReply->str, m_poRedisReply->len);
		return true;
	}

	return false;
}

int RedisResult::GetElementsSize()
{
	if (!IsResultValid())
	{
		return 0;
	}
	if (m_poRedisReply->type == REDIS_REPLY_ARRAY)
	{
		return m_poRedisReply->elements;
	}

	return 0;
}

int RedisResult::GetElementType(UINT32 index)
{
	int size = GetElementsSize();
	if (!size || index >= size)
	{
		return 0;
	}

	if (m_poRedisReply->element[index])
	{
		return m_poRedisReply->element[index]->type;
	}

	return 0;
}

bool RedisResult::GetElement(UINT32 index, std::string& ele)
{
	if (index >= GetElementsSize())
	{
		return false;
	}

	redisReply* temp = m_poRedisReply->element[index];
	if (temp && temp->type == REDIS_REPLY_STRING && temp->str)
	{
		ele = std::string(temp->str, temp->len);
		return true;
	}

	return false;
}

