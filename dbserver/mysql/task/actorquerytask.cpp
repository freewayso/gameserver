#include "pch.h"
#include "actorquerytask.h"
#include "crpc.h"
#include "cache/dbeventconv.h"

CActorQueryTask::CActorQueryTask()
	:CMysqlTask("CActorQueryTask")
	,m_qwStoreKey(0), m_dwRpcDelayed(0)
{
	m_qwEventId = 0;
}

CActorQueryTask::~CActorQueryTask()
{
	
}

void CActorQueryTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::stringstream ss;
	ss << "select `ActorID`,`AccountID`,`SeqIndex`,`ActorName`,`ActorType`,`ResID`,`HairStyle`,`Sex`,`CreateTime` from gameactor where AccountID = " << m_qwStoreKey;
	IMysqlRecordSet* poRes = NULL;
	if (!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		m_oRes.set_error(CBR::ERR_FAILED);
		return;
	}
	if (0 == poRes->GetRowNum())
	{
		m_oRes.set_error(CBR::ERR_MYSQL_NODATA);
	}
	else
	{
		poRes->FetchRow();
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		CBR::ActorBase* pActor =  m_oRes.add_actorlist();
		if (!poRes->GetFieldValue(0, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'ActorID' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->mutable_app()->set_actid(convert<UINT64>(pVal));


		if (!poRes->GetFieldValue(1, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'AccountID' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->set_accountid(convert<UINT64>(pVal));

		if (!poRes->GetFieldValue(2, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'SeqIndex' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->mutable_app()->set_seqindex(convert<UINT32>(pVal));

		std::string strName;
		if (!poRes->GetFieldValue(3, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'ActorName' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		strName.assign(pVal, dwLen);
		pActor->mutable_app()->set_actorname(strName);


		if (!poRes->GetFieldValue(4, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'ActorType' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->mutable_app()->set_actortype(convert<UINT32>(pVal));


		if (!poRes->GetFieldValue(5, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'ResID' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->mutable_app()->set_resid(convert<UINT32>(pVal));

		if (!poRes->GetFieldValue(6, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'HairStyle' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->mutable_app()->set_hairstyle(convert<UINT32>(pVal));


		if (!poRes->GetFieldValue(7, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'Sex' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->mutable_app()->set_sex(convert<UINT32>(pVal));


		if (!poRes->GetFieldValue(8, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'CreateTime' failed";
			m_oRes.set_error(CBR::ERR_FAILED);
			poRes->Release();
			return;
		}
		pActor->mutable_app()->set_createtime(convert<UINT32>(pVal));
		m_oRes.set_error(CBR::ERR_SUCCESS);

	}
	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
		
}


void CActorQueryTask::OnReply()
{
	CBR::ErrorCode dwRet = CBR::ERR_SUCCESS;
	if (m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("CActorQueryTask failed, accountid: %llu, errMsg: %s", m_qwStoreKey, GetErrStream().str().c_str());
	}

	CUserData oUser;
	oUser.m_pUserPtr = (void*)&m_oRes;
	oUser.m_dwUserData = m_oRes.error();
	DbEventConvMgr::Instance()->OnMysqlEvent(m_qwEventId, oUser);
}