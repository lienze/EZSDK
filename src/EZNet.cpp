#include "EZNet.h"

namespace EZ {

	bool InitNetForWin() {
#if (defined _WIN32) || (defined _WIN64)
		WSAData wd;
		WSAStartup(MAKEWORD(2, 2), &wd);
#endif
		return true;
	}


	EZUDP::EZUDP() {
		m_bInitSend = false;
		m_bInitRecv = false;
		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		m_sockaddrSend.sin_family = AF_INET;//IPv4
		m_sockaddrRecv.sin_family = AF_INET;
		memset(m_SendBuff, 0, sizeof(m_SendBuff));
		memset(m_RecvBuff, 0, sizeof(m_RecvBuff));
		m_SendDataQueue.clear();
		m_RecvDataQueue.clear();
	}
	EZUDP::~EZUDP() {}

	bool EZUDP::InitNetSend(const char * _pAddr
		, const unsigned short _usPort) {
		m_bInitSend = true;
		m_sockaddrSend.sin_port = htons(_usPort);
		m_sockaddrSend.sin_addr.s_addr = inet_addr(_pAddr);
		return true;
	}
	bool EZUDP::InitNetRecv(const unsigned short _usPort) {
		m_bInitRecv = true;
		m_sockaddrRecv.sin_port = htons(_usPort);
		m_sockaddrRecv.sin_addr.s_addr = htonl(INADDR_ANY);
		bind(m_sock, (struct sockaddr*)&m_sockaddrRecv, sizeof(m_sockaddrRecv));
		return true;
	}
	int EZUDP::SendTo(const char* pBuf) {
		if (!m_bInitSend)
			return eSendInfo_HasNotInit;
		assert(strlen(pBuf) <= g_iMaxSendLen);
		memset(m_SendBuff, 0, sizeof(m_SendBuff));
		//memcpy(m_SendBuff,pBuf,sizeof(m_SendBuff));
		strncpy(m_SendBuff, pBuf, strlen(pBuf));
		int ret = sendto(m_sock, m_SendBuff, sizeof(m_SendBuff), 0, (struct sockaddr *)&m_sockaddrSend, sizeof(m_sockaddrSend));
		if (ret == -1)
			return eSendInfo_sendtoErr;
		return eSendInfo_Success;
	}
	int EZUDP::RecvFrom() {
		if (!m_bInitRecv)
			return eRecvInfo_HasNotInit;
		//assert(strlen(pBuff)<=g_iMaxRecvLen);
		memset(m_RecvBuff, 0, sizeof(m_RecvBuff));
		socklen_t len = sizeof(m_sockaddrRecv);
		recvfrom(m_sock, m_RecvBuff, g_iMaxRecvLen, 0, (struct sockaddr *)&m_sockaddrRecv, &len);
#ifdef _DEBUG
		printf("recv:%s from_addr:%s:%d\n", m_RecvBuff, inet_ntoa(m_sockaddrRecv.sin_addr), m_sockaddrRecv.sin_port);
#endif
		//�ڶ�������һ��ռ�������Ž������ݣ�����������
		//TODO:��ʱʹ��ϵͳά���Ķѿռ䣨���ڶ��в�����Ƭ��
		//������Ҫ�޸�Ϊ�ڴ�صĹ���ʽ�������ֶ�����
		RecvPack *_pRPack = new RecvPack();
		if (_pRPack) {
			//�������ݳ�ʼ��
			_pRPack->m_iType = 0;
			_pRPack->iSize = 0;
			//TODO:�˴����ܻ�����⣬�������޸�
			strncpy(_pRPack->_RecvData, m_RecvBuff, strlen(m_RecvBuff) + 1);
			//�������н���ͳһ����
			m_RecvDataQueue.push_back(_pRPack);
			return eRecvInfo_Success;
		}
		return eRecvInfo_NewPackErr;
	}
	int EZUDP::GetSock() { return m_sock; }
	std::deque<RecvPack*> *EZUDP::GetRecvQueue() {
		return &m_RecvDataQueue;
	}



	EZSelector::EZSelector() {
		FD_ZERO(&m_rd);
		m_tv.tv_sec = 0;
		m_tv.tv_usec = 0;
		m_iMaxSock = 0;
	}
	EZSelector::~EZSelector() {}
	bool EZSelector::DoSelect(std::vector<EZNetBase*> &vec, long _tv_sec, long _tv_usec) {
		//��ʼ��select��ʱʱ��
		m_tv.tv_sec = _tv_sec;
		m_tv.tv_usec = _tv_usec;
		//��ʼ��readfds�б�
		FD_ZERO(&m_rd);
		m_iMaxSock = 0;
		for (auto& elem : vec) {
			if (elem->GetSock() > m_iMaxSock)
				m_iMaxSock = elem->GetSock();
			FD_SET(elem->GetSock(), &m_rd);
		}
		int rst = select(m_iMaxSock + 1, &m_rd, NULL, NULL, &m_tv);
		if (rst > 0)
		{
			printf("Data is available %d\n", rst);
			//�������ϲ���зַ�
			//TODO:������Ҫ�Էַ�ģʽ�����Ż�
			PutDataUp(vec);
			return true;
		}
		else if (rst == -1)
		{
			printf("select error %d\n", rst);
			return false;
		}
		else
		{
			//printf("None Data %d\n", rst);
			return false;
		}
	}
	void EZSelector::PutDataUp(std::vector<EZNetBase*> vec) {
		//�Դ��ں�̬��ȡ����readfds�б������ѯ
		for (auto elem : vec) {
			printf("current socket:%d\n", elem->GetSock());
			elem->RecvFrom();
		}
	}
	void EZSelector::SetMaxSock(int _MaxSock) { m_iMaxSock = _MaxSock; }
	int  EZSelector::GetMaxSock() { return m_iMaxSock; }

	EZNetMan::EZNetMan() {}
	EZNetMan::~EZNetMan() {}
	bool EZNetMan::AddUnit(EZNetBase* _p) {
		m_vecNetUnit.push_back(_p);
		return true;
	}
	bool EZNetMan::RemoveUnit(EZNetBase *_p) {
		return true;
	}
	bool EZNetMan::FindUnit(EZNetBase *_p) {
		for (auto it : m_vecNetUnit) {
			if (it == _p)
				return true;
		}
		return false;
	}
	int EZNetMan::FindUnitPos(EZNetBase *_p) {
		unsigned int _iPos = 0;
		for (auto it : m_vecNetUnit) {
			if (it == _p)
				return _iPos;
			_iPos++;
		}
		return -1;
	}
	//��ɽ��ղ����������͵��ϲ�ȴ�����
	bool EZNetMan::RecLogic() {
		return m_selector.DoSelect(m_vecNetUnit, 0, MICROSECONDS_4);
	}
	//���ϲ������ƽ������Ͷ��к���
	bool EZNetMan::SendLogic() {
		return true;
	}
	//���ÿ�����ӽ�����Ӧ���߼�����
	bool EZNetMan::Logic() {
		for (auto it : m_vecNetUnit) {
			if (it && it->GetRecvQueue() && !(it->GetRecvQueue()->empty())) {
				//TODO:Ŀǰ�����������ӽ��յ������ݽ��д�ӡ������Ҫ�������ϲ�����
				printf("front Data:%s\n", it->GetRecvQueue()->front()->_RecvData);
				//����ָ����ָ����ڶ��ڵĿռ�
				delete[] it->GetRecvQueue()->front();
				//���еĵ�һ������ָ�뵯��
				it->GetRecvQueue()->pop_front();
			}
		}
		return true;
	}

}//end namespace EZ

