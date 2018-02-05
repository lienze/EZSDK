#pragma once

#if (defined __APPLE__) || (defined LINUX)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif (defined _WIN32) || (defined _WIN64)
#include <WinSock2.h>
#endif


namespace EZ {
class EZNetBase
{
	virtual bool InitNet(unsigned short _usPort,const char * _pAddr) = 0;
};
class EZUDP:public EZNetBase
{
public:
	EZUDP(){
		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		m_sockaddr.sin_family = AF_INET;//IPv4
		memset(m_SendBuff,0,sizeof(m_SendBuff));
	}
	~EZUDP(){}
public:
	bool InitNet(unsigned short _usPort,const char * _pAddr){
		m_sockaddr.sin_port = htons(_usPort);
		m_sockaddr.sin_addr.s_addr = inet_addr(_pAddr);
		return true;
	}
	bool SendTo(){
		int ret = sendto(m_sock,m_SendBuff,sizeof(m_SendBuff),0,(struct sockaddr *)&m_sockaddr,sizeof(m_sockaddr));
		if(ret == -1)
			return false;
		return true;
	}
private:
	int m_sock;
	struct sockaddr_in m_sockaddr;
	char m_SendBuff[1024];
};

class EZTCP:public EZNetBase
{
public:
	EZTCP(){}
	~EZTCP(){}
};
}//end namespace EZ
