#pragma once

//标准库
#include <cstring>
#include <vector>
#include <deque>
#include <cassert>

namespace EZ {

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

const int g_iMaxSendLen = 1024;
const int g_iMaxRecvLen = 1024;
//const int g_iMaxNetUnit = 200;

bool InitNetForWin();

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
	virtual int RecvFrom() = 0;
	virtual std::deque<RecvPack*> *GetRecvQueue() = 0;
};
class EZUDP:public EZNetBase
{
public:
	EZUDP();
	~EZUDP();
public:
	enum eSendInfo{
		eSendInfo_Success    =  0,
		eSendInfo_HasNotInit = -1,
		eSendInfo_sendtoErr  = -2,
	};
	bool InitNetSend(const char * _pAddr
		, const unsigned short _usPort);
	enum eRecvInfo{
		eRecvInfo_Success    =  0,
		eRecvInfo_HasNotInit = -1,
		eRecvInfo_NewPackErr  = -2,
	};
	bool InitNetRecv(const unsigned short _usPort);
	int SendTo(const char* pBuf);
	virtual int RecvFrom();
	virtual int GetSock();
	virtual std::deque<RecvPack*> *GetRecvQueue();
private:
	int m_sock;
	struct sockaddr_in m_sockaddrSend;
	struct sockaddr_in m_sockaddrRecv;
	bool m_bInitSend;//标识是否进行过发送初始化操作
	bool m_bInitRecv;//标识是否进行过接收初始化操作
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
	EZSelector();
	~EZSelector();
	bool DoSelect(std::vector<EZNetBase*> &vec, long _tv_sec, long _tv_usec);
	void PutDataUp(std::vector<EZNetBase*> vec);
	void SetMaxSock(int _MaxSock);
	int  GetMaxSock();
private:
	fd_set m_rd;
	struct timeval m_tv;
	int m_iMaxSock;
};

class EZNetMan	//网络连接管理器
{
public:
	EZNetMan();
	~EZNetMan();
	bool AddUnit(EZNetBase* _p);
	bool RemoveUnit(EZNetBase *_p);
	bool FindUnit(EZNetBase *_p);
	int FindUnitPos(EZNetBase *_p);
	//完成接收并将数据推送到上层等待处理
	bool RecLogic();
	//将上层数据移交至发送队列后发送
	bool SendLogic();
	//针对每个连接进行相应的逻辑处理
	bool Logic();
private:
	std::vector<EZNetBase*> m_vecNetUnit;
	EZSelector m_selector;
};
}//end namespace EZ

