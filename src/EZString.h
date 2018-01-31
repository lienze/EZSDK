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
		EZString __tmpReturnStr;
		if((_iPos=_szTmp.find(":"))==std::string::npos)
			return __tmpReturnStr;
		_iDis = _iPos - _iFrom;
		std::string _szFromPos = _szTmp.substr(_iFrom,_iDis);
		std::string _szToPos   = _szTmp.substr(_iPos+1);
		_iFrom = atoi(_szFromPos.c_str());
		_iPos  = atoi(_szToPos.c_str());
		_iDis  = _iPos - _iFrom + 1;
		__tmpReturnStr = basic_str.substr(_iFrom,_iDis);
		return __tmpReturnStr;
	}
	EZString operator+(EZString &_szForAdd){
		EZString __tmpReturnStr;
		__tmpReturnStr = basic_str + _szForAdd.content().c_str();
		return __tmpReturnStr;
	}
	std::string content(){
		return basic_str;
	}
	EZStringList split(const char &_cTmp);
	inline int GetLength() const{
		return basic_str.length();
	}
private:
	std::string basic_str;
};

struct EZStringListNode
{
	EZStringListNode *pPreNode;
	EZString ez_str;
	EZStringListNode *pNextNode;
	EZStringListNode(){
		pPreNode = NULL;
		pNextNode = NULL;
	}
	EZStringListNode(std::string &_szTmp){
		pPreNode = NULL;
		ez_str = _szTmp;
		pNextNode = NULL;
	}
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
	bool InsertAt(int _iPos,std::string &_szTmp){
		if(pHead == NULL){
			return createList(_szTmp);
		}else{
			if(_iPos<0 || _iPos>iNodeNum)return false;
			int _i=0;
			pCurr = pHead;
			while(_i<_iPos){
				if(pCurr && pCurr->pNextNode){
					pCurr = pCurr->pNextNode;
					_i++;
				}else{
					return false;
				}
			}
			assert(pCurr!=NULL);
			EZStringListNode * _pNodeTmp = new EZStringListNode(_szTmp);
			if(_i == 0){
				if(pCurr->pPreNode == NULL){//Head Node
					pCurr->pPreNode = _pNodeTmp;
					pHead = _pNodeTmp;
					_pNodeTmp->pNextNode = pCurr;
					iNodeNum++;
				}else{
					delete _pNodeTmp;
					return false;
				}
			}else{
				if(pCurr->pPreNode != NULL){
					pCurr->pPreNode->pNextNode = _pNodeTmp;
					_pNodeTmp->pPreNode = pCurr->pPreNode;
					_pNodeTmp->pNextNode = pCurr;
					pCurr->pPreNode = _pNodeTmp;
					iNodeNum++;
				}else{
					delete _pNodeTmp;
					return false;
				}
			}
			return true;
		}
	}
	bool PrintList(){
		if (pHead == NULL) {
			printf("List is NULL\n");
			return false;
		}
		else{
			pCurr = pHead;
			while(pCurr){
				printf("%s ", pCurr->ez_str.content().c_str());
				pCurr = pCurr->pNextNode;
			}
			printf("\n");
			return true;
		}
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
		return true;
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
	if(_iFrom <= basic_str.length()){
		std::string tmp = basic_str.substr(_iFrom);
		_tmpList.AddInTail(tmp);
	}
	return _tmpList;
}
}//end namespace EZ
