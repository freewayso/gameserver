#include "pch.h"
#include <time.h>
#include <stdlib.h>
#include "verifymgr.h"
#include "config.h"
#include "util/XRandom.h"


INSTANCE_SINGLETON(VerifyMgr)

VerifyMgr::VerifyMgr()
{
	srand((unsigned int)time(NULL));
	m_sqlRandIndex = 0;
	m_dbTaskNum = 0;
}

VerifyMgr::~VerifyMgr()
{

}

bool VerifyMgr::Init()
{
	const DBInfo &db = LoginConfig::Instance()->GetDBInfo();
	for (int i = 0; i < SQLTHREAD_COUNT; ++i)
	{
		if (m_dbThread[i].Init(db))
		{
			m_dbThread[i].Start();
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

void VerifyMgr::Uninit()
{
	for (int i = 0; i < SQLTHREAD_COUNT; ++i)
	{
		m_dbThread[i].Cancel();
		m_dbThread[i].Join();
		m_dbThread[i].Uninit();
	}
}



void VerifyMgr::Update()
{
	for (int i = 0; i < SQLTHREAD_COUNT; ++i)
	{
		m_dbThread[i].FinishedTaskList();
	}

}



void VerifyMgr::AddDBTask(ISQLTask *pTask)
{
	m_sqlRandIndex = (m_sqlRandIndex + 1)%SQLTHREAD_COUNT;
	m_dbThread[m_sqlRandIndex].AddTask(pTask);
	m_dbTaskNum++;
}