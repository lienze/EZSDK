#pragma once

#if (defined __APPLE__) || (defined __linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif (defined _WIN32) || (defined _WIN64)
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#endif

#include <cstring>
#include <vector>

#include "EZDef.h"

namespace EZ {

const int g_iMaxSendLen = 1024;
const int g_iMaxRecvLen = 1024;
//const int g_iMaxNetUnit = 200;

class EZNetBase
{
public:
	EZNetBase(){}
	virtual ~EZNetBase(){}
	virtual int GetSock() = 0;
	virtual bool RecvFrom() = 0;
};
class EZUDP:public EZNetBase
{
public:
	EZUDP(){
		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		m_sockaddrSend.sin_family = AF_INET;//IPv4
		m_sockaddrRecv.sin_family = AF_INET;
		memset(m_SendBuff,0,sizeof(m_SendBuff));
		memset(m_RecvBuff,0,sizeof(m_RecvBuff));
	}
	~EZUDP(){}
public:
	bool InitNetSend(const char * _pAddr
					,const unsigned short _usPort){
		m_sockaddrSend.sin_port = htons(_usPort);
		m_sockaddrSend.sin_addr.s_addr = inet_addr(_pAddr);
		return true;
	}
	bool InitNetRecv(const unsigned short _usPort){
		m_sockaddrRecv.sin_port = htons(_usPort);
		m_sockaddrRecv.sin_addr.s_addr = htonl(INADDR_ANY);
		bind(m_sock,(struct sockaddr*)&m_sockaddrRecv,sizeof(m_sockaddrRecv));
		return true;
	}
	bool SendTo(const char* pBuf){
		assert(strlen(pBuf)<=g_iMaxSendLen);
		memset(m_SendBuff,0,sizeof(m_SendBuff));
		//memcpy(m_SendBuff,pBuf,sizeof(m_SendBuff));
		strncpy(m_SendBuff,pBuf,strlen(pBuf));
		int ret = sendto(m_sock,m_SendBuff,sizeof(m_SendBuff),0,(struct sockaddr *)&m_sockaddrSend,sizeof(m_sockaddrSend));
		if(ret == -1)
			return false;
		return true;
	}
	virtual bool RecvFrom(){
		//assert(strlen(pBuff)<=g_iMaxRecvLen);
		memset(m_RecvBuff,0,sizeof(m_RecvBuff));
		socklen_t len = sizeof(m_sockaddrRecv);
		recvfrom(m_sock,m_RecvBuff,g_iMaxRecvLen,0,(struct sockaddr *)&m_sockaddrRecv,&len);
		printf("recv:%s from_addr:%s:%d\n",m_RecvBuff,inet_ntoa(m_sockaddrRecv.sin_addr),m_sockaddrRecv.sin_port);
		return true;
	}
	virtual int GetSock() { return m_sock; }
private:
	int m_sock;
	struct sockaddr_in m_sockaddrSend;
	struct sockaddr_in m_sockaddrRecv;
	char m_SendBuff[g_iMaxSendLen];
	char m_RecvBuff[g_iMaxRecvLen];
};

class EZTCP:public EZNetBase
{
public:
	EZTCP(){}
	~EZTCP(){}
};

class EZSelector
{
public:
	EZSelector() {
		FD_ZERO(&m_rd);
		m_tv.tv_sec = 0;
		m_tv.tv_usec = 0;
		m_iMaxSock = 0;
	}
	~EZSelector() {}
	bool DoSelect(std::vector<EZNetBase*> &vec,long _tv_sec,long _tv_usec) {
		int iTest = FD_SETSIZE;
		//初始化select超时时间
		m_tv.tv_sec = _tv_sec;
		m_tv.tv_usec = _tv_usec;
		//初始化readfds列表
		FD_ZERO(&m_rd);
		m_iMaxSock = 0;
		for (auto& elem : vec) {
			if (elem->GetSock() > m_iMaxSock)
				m_iMaxSock = elem->GetSock();
			FD_SET(elem->GetSock(), &m_rd);
		}
		int rst = select(m_iMaxSock+1, &m_rd, NULL, NULL, &m_tv);
		if (rst > 0)
		{
			printf("Data is available %d\n", rst);
			PutDataUp(vec);
			return true;
		}
		else if (rst == -1)
		{
			printf("select error %d\n",rst);
			return false;
		}
		else
		{
			//printf("None Data %d\n", rst);
			return false;
		}
	}
	void PutDataUp(std::vector<EZNetBase*> vec) {
		//对从内核态获取到的readfds列表进行轮询
		for (auto elem : vec) {
			printf("%d\n", elem->GetSock());
			elem->RecvFrom();
		}
	}
	void SetMaxSock(int _MaxSock) { m_iMaxSock = _MaxSock; }
	int  GetMaxSock() { return m_iMaxSock; }
private:
	fd_set m_rd;
	struct timeval m_tv;
	int m_iErr;
	int m_iMaxSock;
};

class EZNetMan	//Net Manager
{
public:
	EZNetMan(){}
	~EZNetMan(){}
	bool AddUnit(EZNetBase* _p){
		m_vecNetUnit.push_back(_p);
		return true;
	}
	bool RemoveUnit(EZNetBase *_p){
		return true;
	}
	bool FindUnit(EZNetBase *_p){
		for (auto it : m_vecNetUnit) {
			if (it == _p)
				return true;
		}
		return false;
	}
	unsigned int FindUnitPos(EZNetBase *_p){
		unsigned int _iPos = 0;
		for (auto it : m_vecNetUnit) {
			if (it == _p)
				return _iPos;
			_iPos++;
		}
		return -1;
	}
	bool Logic() 
	{
		if (m_selector.DoSelect(m_vecNetUnit,0, MICROSECONDS_4) == true)//存在数据
		{
			//TODO 进行接收，推送至上层逻辑中进行处理

		}
		return true;
	}
private:
	std::vector<EZNetBase*> m_vecNetUnit;
	EZSelector m_selector;
};
}//end namespace EZ

