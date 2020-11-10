#ifndef __ROBOTMGR__H
#define __ROBOTMGR__H


#include <map>
#include <string>
#include <list>
#include "network.h"
#include "crpc.h"


enum eRobotTaskType
{
	eCONNECT	= 1,
	eCONNECTING = 2,
	eCONNECTED	= 3,
	eLOGINING		= 4,
	eLOGINIED		= 5,
	eEND        = 6,

};

struct stDBInfo
{
	std::string ip;
	UINT32 port;
	std::string name;
	std::string password;
	std::string database;
	stDBInfo()
	{
		ip			= "";
		port		= 0;
		name		="";
		password	= "";
		database		= "";
	}

};

class LoginLink;
class ClientLink;

struct stUser
{
	UINT64 id;
	std::string m_name;
	std::string m_passward;
	stUser()
	{
		id = 0;
		m_name = "";
		m_passward = "";
	}
};

struct PeerInfo;


class RobotMgr
{
	RobotMgr();
	~RobotMgr();
	DECLARE_SINGLETON(RobotMgr)
public:
	bool Init(const char *config);
	void Uninit();
	void Update();
	void CheckLinkState();
	void ProcessNetMessage();
	bool Send(UINT32 dwConnID, CRpc &roRpc);
	char * BuildActorName();
	char * BuildUserName();
	LoginLink* FindLinker(UINT32 dwConnID);
	bool SendTo(UINT32 dwConnID, const CProtocol& roPtc);
	void SendLogin();
	bool InitNetwork();
	bool LoadFile(const char *config);
	void DelLoginLinker(LoginLink* pLink);
	void ConnectLoginTask();
	void addClient(const std::string &szIP, UINT32 dwPort,UINT64 llUserId,UINT32 dwSendBuffSize = 1024*1024*10,UINT32 dwRecvBuffSize=1024*1024*10);
	void delClientLinker(ClientLink *pLink);
	ClientLink* getClientLink(UINT32 dwConnId);
	void DisConnect(UINT32 dwConnId);

public:
	UINT32 m_dwPressActor;
	std::string m_szFixName;
	std::string m_szDeviceId;
private:
	std::vector<LoginLink*> m_LoginHandler;
	std::list<ClientLink*> m_ClientHandler;
	INT32 m_dwMaxClient;
	UINT32 m_dwThread;
	INetWork            *m_Network;
	UINT32 m_dwTaskState;
	PeerInfo *m_pLoginCfg;
	UINT32 m_dwLoginBatchNum;;
	UINT32 m_dwLoginExcuteNum;
	UINT64 m_qwLoginExcuteTime;
	UINT32 m_dwNum;
	


};


#endif

