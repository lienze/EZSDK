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
	bool RecvFrom(){
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
		FD_SET(0, &m_rd);
		m_tv.tv_sec = 5;
		m_tv.tv_usec = 0;
		m_MaxSock = 0;
		m_SockNum = 0;
	}
	~EZSelector() {}
	bool Logic() {
		FD_ZERO(&m_rd);
		int rst = select(m_MaxSock+1, &m_rd, NULL, NULL, &m_tv);
		if (rst > 0)
		{
			printf("Data is available.%d\n", rst);
			return true;
		}
		else
		{
			printf("Data none %d\n", rst);
			return false;
		}
	}
	void SetMaxSock(int _MaxSock) { m_MaxSock = _MaxSock; }
	int  GetMaxSock() { return m_MaxSock; }
	void SetSockNum(int _SockNum) { m_SockNum = _SockNum; }
	int  GetSockNum() { return m_SockNum; }
private:
	fd_set m_rd;
	struct timeval m_tv;
	int m_iErr;
	int m_MaxSock;
	int m_SockNum;
};

class EZNetMan	//Net Manager
{
public:
	EZNetMan(){}
	~EZNetMan(){}
	bool AddUnit(EZNetBase* _p){
		m_vecNetUnit.push_back(_p);
		if(m_selector.GetMaxSock() < _p->GetSock())
			m_selector.SetMaxSock(_p->GetSock());
		m_selector.SetSockNum(m_selector.GetSockNum() + 1);
		return true;
	}
	bool RemoveUnit(EZNetBase *_p){
		return true;
	}
	bool FindUnit(EZNetBase *_p){
		auto it = m_vecNetUnit.begin();
 		for(;it!=m_vecNetUnit.end();it++){
			if(*it == _p)
				return true;	
		}
		return false;
	}
	unsigned int FindUnitPos(EZNetBase *_p){
		unsigned int _iPos = 0;
		auto it = m_vecNetUnit.begin();
		for(;it!=m_vecNetUnit.end();it++,_iPos++){
			if(*it == _p)
				return _iPos;
		}
		return -1;
	}
	bool Logic() {
		if (m_selector.Logic() == true)//存在数据
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
