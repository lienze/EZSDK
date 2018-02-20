#pragma once

#if (defined __APPLE__) || (defined __linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif (defined _WIN32) || (defined _WIN64)
#include <WinSock2.h>
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
	//virtual bool InitNet(const char * _pAddr,const unsigned short _usPort) = 0;
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
		printf("recv: %s\n",m_RecvBuff);
		return true;
	}
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
class EZNetMan	//Net Manager
{
public:
	EZNetMan(){}
	~EZNetMan(){}
	bool AddUnit(EZNetBase* _p){
		vecNetUnit.push_back(_p);
		return true;
	}
	bool RemoveUnit(EZNetBase *_p){
		return true;
	}
	bool FindUnit(EZNetBase *_p){
		auto it = vecNetUnit.begin();
 		for(;it!=vecNetUnit.end();it++){
			if(*it == _p)
				return true;	
		}
		return false;
	}
	unsigned int FindUnitPos(EZNetBase *_p){
		unsigned int _iPos = 0;
		auto it = vecNetUnit.begin();
		for(;it!=vecNetUnit.end();it++,_iPos++){
			if(*it == _p)
				return _iPos;
		}
		return -1;
	}
private:
	std::vector<EZNetBase*> vecNetUnit;
};
}//end namespace EZ
