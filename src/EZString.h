#pragma once

#include <string>
#include <cassert>

namespace EZ {
class EZStringList;

class EZString
{
public:
	EZString(){}
	~EZString(){}
	EZString(std::string &_szTmp){
		basic_str = _szTmp;
	}
	void operator=(const std::string &_szTmp){
		basic_str = _szTmp;
	}
	EZString operator[](const std::string &_szTmp){
		int _iFrom = 0, _iDis = 0, _iPos = 0;
		EZString tmpReturnStr;
		if((_iPos=_szTmp.find(":"))==std::string::npos)
			return tmpReturnStr;
		_iDis = _iPos - _iFrom;
		std::string _szFromPos = _szTmp.substr(_iFrom,_iDis);
		std::string _szToPos   = _szTmp.substr(_iPos+1);
		_iFrom = atoi(_szFromPos.c_str());
		_iPos  = atoi(_szToPos.c_str());
		_iDis  = _iPos - _iFrom + 1;
		tmpReturnStr = basic_str.substr(_iFrom,_iDis);
		return tmpReturnStr;
	}
	std::string content(){
		return basic_str;
	}
	EZStringList split(const char &_cTmp);

private:
	std::string basic_str;
};

struct EZStringListNode
{
	EZStringListNode *pPreNode;
	EZString ez_str;
	EZStringListNode *pNextNode;
};

class EZStringList
{
public:
	EZStringList(){
		pHead = pCurr = pTail = NULL;
		iNodeNum = 0;
	}
	~EZStringList(){}
	EZString * operator[](int _idx) {
		assert(_idx < iNodeNum);
		EZStringListNode *pCurrTmp = pHead;
		assert(pCurrTmp != NULL);
		for(int _i = 0; _i < _idx;_i++){
			if(pCurrTmp){
				pCurrTmp = pCurrTmp->pNextNode;
			}else{
				return NULL;
			}
		}
		return &(pCurrTmp->ez_str);
	}
	bool AddInTail(std::string &_szTmp){
		if(pHead==NULL){
			createList(_szTmp);
		}else{
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
	bool AddInHead(std::string &_szTmp){
		if (pHead == NULL) {
			createList(_szTmp);
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
	inline int GetSize() const{
		return iNodeNum;
	}
	bool DeleteAll(){
		while(pHead!=pTail){
			pCurr = pTail->pPreNode;
			delete pTail;
			pTail = pCurr;
		}
		delete pHead;
		pHead = pCurr = pTail = NULL;
		iNodeNum = 0;
	}
private:
	EZStringListNode * pHead;
	EZStringListNode * pCurr;
	EZStringListNode * pTail;
	int iNodeNum;
private:
	bool createList(std::string &_szTmp){
		pHead = new EZStringListNode;
		pHead->pPreNode = NULL;
		pHead->ez_str = _szTmp;
		pHead->pNextNode = NULL;
		pCurr = pHead;
		pTail = pHead;
		iNodeNum++;
		return true;
	}
};

//Class implementation for EZString::split function
//in order to solve recursive call
EZStringList EZString::split(const char &_cTmp){
	int _iFrom = 0, _iDis = 0, _iPos = 0;
	EZStringList _tmpList;// = new EZStringList();
	while((_iPos=basic_str.find(_cTmp,_iFrom))!=std::string::npos){
		_iDis = _iPos - _iFrom;
		std::string tmp = basic_str.substr(_iFrom,_iDis);
		_tmpList.AddInTail(tmp);
		_iFrom = _iPos + 1;
	}
	if(_iFrom < basic_str.length()-1){
		std::string tmp = basic_str.substr(_iFrom);
		_tmpList.AddInTail(tmp);
	}
	return _tmpList;
}
}//end namespace EZ
