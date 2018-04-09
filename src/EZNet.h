#pragma once

//处理不同平台头文件差异
#if (defined __APPLE__) || (defined __linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //for select module
#elif (defined _WIN32) || (defined _WIN64)
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#endif

//标准库
#include <cstring>
#include <vector>
#include <deque>

//用于处理不同平台数据类型
#if (defined __APPLE__) || (defined __linux__)
#define byte unsigned char
#elif (defined _WIN32) || (defined _WIN64)

#endif

//宏定义
#define MICROSECONDS	1000000				//1s
#define MICROSECONDS_2	(MICROSECONDS/2)	//0.5s
#define MICROSECONDS_4	(MICROSECONDS/4)	//0.25s
#define MICROSECONDS_5	(MICROSECONDS/5)	//0.2s
#define MICROSECONDS_10	(MICROSECONDS/10)	//0.01s

namespace EZ {

const int g_iMaxSendLen = 1024;
const int g_iMaxRecvLen = 1024;
//const int g_iMaxNetUnit = 200;

bool InitNetForWin() {
#if (defined _WIN32) || (defined _WIN64)
	WSAData wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
#endif
	return true;
}

//所有数据包的基础结构
struct PackBase {
	byte m_iType;
};

//发送数据包结构
struct SendPack :public PackBase {
	int iSize;
	char _SendData[1];
};

//接收数据包结构
struct RecvPack :public PackBase {
	int iSize;
	char _RecvData[1];
};

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
		m_SendDataQueue.clear();
		m_RecvDataQueue.clear();
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
#ifdef _DEBUG
		printf("recv:%s from_addr:%s:%d\n",m_RecvBuff,inet_ntoa(m_sockaddrRecv.sin_addr),m_sockaddrRecv.sin_port);
#endif
		//在堆中申请一块空间用来存放接收数据，待后续处理
		//TODO:暂时使用系统维护的空间（易在堆中产生碎片）
		//后续需要修改为内存池的管理方式并进行手动管理
		RecvPack *_pRPack = new RecvPack();
		if (_pRPack) {
			//接收数据初始化
			_pRPack->m_iType = 0;
			_pRPack->iSize = 0;
			char *_pTmp = new char[strlen(m_RecvBuff)];
			strncpy(_pRPack->_RecvData, _pTmp, strlen(_pTmp));
			//交给队列进行统一管理
			m_RecvDataQueue.push_back(_pRPack);
			return true;
		}
		return false;
	}
	virtual int GetSock() { return m_sock; }
private:
	int m_sock;
	struct sockaddr_in m_sockaddrSend;
	struct sockaddr_in m_sockaddrRecv;
	char m_SendBuff[g_iMaxSendLen];
	char m_RecvBuff[g_iMaxRecvLen];
	std::deque<SendPack*> m_SendDataQueue;//存储指向发送数据的指针
	std::deque<RecvPack*> m_RecvDataQueue;//存储指向接收数据的指针
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
			//数据向上层进行分发
			//TODO:后续需要对分发模式进行优化
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
			printf("current socket:%d\n", elem->GetSock());
			elem->RecvFrom();
		}
	}
	void SetMaxSock(int _MaxSock) { m_iMaxSock = _MaxSock; }
	int  GetMaxSock() { return m_iMaxSock; }
private:
	fd_set m_rd;
	struct timeval m_tv;
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
	//完成接收并将数据推送到上层等待处理
	bool RecLogic(){
		return m_selector.DoSelect(m_vecNetUnit, 0, MICROSECONDS_4);
	}
	//将上层数据移交至发送队列后发送
	bool SendLogic() {
		return true;
	}
	//针对每个连接进行相应的逻辑处理
	bool Logic() {
		return true;
	}
private:
	std::vector<EZNetBase*> m_vecNetUnit;
	EZSelector m_selector;
};
}//end namespace EZ

