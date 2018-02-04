#pragma once

#if (defined __APPLE__) || (defined LINUX)
#include <sys/socket.h>
#include <netinet/in.h>
#endif

namespace EZ {
class EZNetBase
{
	virtual bool InitNet() = 0;
};
class EZUDP:public EZNetBase
{
public:
	EZUDP(){
		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		m_sockaddr.sin_family = AF_INET;//IPv4

	}
	~EZUDP(){}
public:
	bool InitNet(){
		//m_sockaddr.sin_port = htons();
		//m_sockaddr.sin_addr.s_addr = htonl();
		return true;
	}
private:
	int m_sock;
	struct sockaddr_in m_sockaddr;
};

class EZTCP:public EZNetBase
{
public:
	EZTCP(){}
	~EZTCP(){}
};
}//end namespace EZ
