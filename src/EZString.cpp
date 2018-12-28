#include "EZString.h"

namespace EZ {

	EZString::EZString(std::string &_szTmp) {
		basic_str = _szTmp;
	}
	void EZString::operator=(const std::string &_szTmp) {
		basic_str = _szTmp;
	}
	EZString EZString::operator+(EZString &_szForAdd) {
		EZString __tmpReturnStr;
		__tmpReturnStr = basic_str + _szForAdd.content().c_str();
		return __tmpReturnStr;
	}
	EZString EZString::operator[](const std::string &_szTmp) {
		int _iFrom = 0, _iDis = 0, _iPos = 0;
		EZString __tmpReturnStr;
		if ((_iPos = _szTmp.find(":")) == std::string::npos)
			return __tmpReturnStr;
		_iDis = _iPos - _iFrom;
		std::string _szFromPos = _szTmp.substr(_iFrom, _iDis);
		std::string _szToPos = _szTmp.substr(_iPos + 1);
		_iFrom = atoi(_szFromPos.c_str());
		_iPos = atoi(_szToPos.c_str());
		_iDis = _iPos - _iFrom + 1;
		__tmpReturnStr = basic_str.substr(_iFrom, _iDis);
		return __tmpReturnStr;
	}
	std::string EZString::content() {
		return basic_str;
	}

	EZStringList::EZStringList() {
		pHead = pCurr = pTail = NULL;
		iNodeNum = 0;
	}
	EZString * EZStringList::operator[](int _idx) {
		assert(_idx < iNodeNum);
		EZStringListNode *pCurrTmp = pHead;
		assert(pCurrTmp != NULL);
		for (int _i = 0; _i < _idx; _i++) {
			if (pCurrTmp) {
				pCurrTmp = pCurrTmp->pNextNode;
			}
			else {
				return NULL;
			}
		}
		return &(pCurrTmp->ez_str);
	}
	bool EZStringList::AddInTail(std::string &_szTmp) {
		if (pHead == NULL) {
			createList(_szTmp);
		}
		else {
			pTail->pNextNode = new EZStringListNode;
			pCurr = pTail->pNextNode;
			pCurr->pPreNode = pTail;
			pCurr->pNextNode = NULL;
			pTail = pCurr;
			pTail->ez_str = _szTmp;
			iNodeNum++;
		}
		return true;
	}
	bool EZStringList::AddInHead(std::string &_szTmp) {
		if (pHead == NULL) {
			return createList(_szTmp);
		}
		else {
			pHead->pPreNode = new EZStringListNode;
			pCurr = pHead->pPreNode;
			pCurr->pPreNode = NULL;
			pCurr->pNextNode = pHead;
			pHead = pCurr;
			pHead->ez_str = _szTmp;
			iNodeNum++;
		}
		return true;
	}
	bool EZStringList::InsertAt(int _iPos, std::string &_szTmp) {
		if (pHead == NULL) {
			return createList(_szTmp);
		}
		else {
			if (_iPos<0 || _iPos>iNodeNum)return false;
			int _i = 0;
			pCurr = pHead;
			while (_i < _iPos) {
				if (pCurr && pCurr->pNextNode) {
					pCurr = pCurr->pNextNode;
					_i++;
				}
				else {
					return false;
				}
			}
			assert(pCurr != NULL);
			EZStringListNode * _pNodeTmp = new EZStringListNode(_szTmp);
			if (_i == 0) {
				if (pCurr->pPreNode == NULL) {//Head Node
					pCurr->pPreNode = _pNodeTmp;
					pHead = _pNodeTmp;
					_pNodeTmp->pNextNode = pCurr;
					iNodeNum++;
				}
				else {
					delete _pNodeTmp;
					return false;
				}
			}
			else {
				if (pCurr->pPreNode != NULL) {
					pCurr->pPreNode->pNextNode = _pNodeTmp;
					_pNodeTmp->pPreNode = pCurr->pPreNode;
					_pNodeTmp->pNextNode = pCurr;
					pCurr->pPreNode = _pNodeTmp;
					iNodeNum++;
				}
				else {
					delete _pNodeTmp;
					return false;
				}
			}
			return true;
		}
	}
	bool EZStringList::PrintList() {
		if (pHead == NULL) {
			printf("List is NULL\n");
			return false;
		}
		else {
			pCurr = pHead;
			while (pCurr) {
				printf("%s ", pCurr->ez_str.content().c_str());
				pCurr = pCurr->pNextNode;
			}
			printf("\n");
			return true;
		}
	}
	bool EZStringList::DeleteAll() {
		while (pHead != pTail) {
			pCurr = pTail->pPreNode;
			delete pTail;
			pTail = pCurr;
		}
		delete pHead;
		pHead = pCurr = pTail = NULL;
		iNodeNum = 0;
		return true;
	}
	bool EZStringList::createList(std::string &_szTmp) {
		pHead = new EZStringListNode;
		pHead->pPreNode = NULL;
		pHead->ez_str = _szTmp;
		pHead->pNextNode = NULL;
		pCurr = pHead;
		pTail = pHead;
		iNodeNum++;
		return true;
	}

	//Class implementation for EZString::split function
	//in order to solve recursive call
	EZStringList EZString::split(const char &_cTmp) {
		size_t _iFrom = 0, _iDis = 0, _iPos = 0;
		EZStringList _tmpList;// = new EZStringList();
		while ((_iPos = basic_str.find(_cTmp, _iFrom)) != std::string::npos) {
			_iDis = _iPos - _iFrom;
			std::string tmp = basic_str.substr(_iFrom, _iDis);
			_tmpList.AddInTail(tmp);
			_iFrom = _iPos + 1;
		}
		if (_iFrom <= basic_str.length()) {
			std::string tmp = basic_str.substr(_iFrom);
			_tmpList.AddInTail(tmp);
		}
		return _tmpList;
	}
}//end namespace EZ
