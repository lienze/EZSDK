#pragma once

#include <string>
#include <cassert>

namespace EZ {
class EZStringList;

class EZString
{
public:
	EZString() {}
	~EZString(){}
	EZString(std::string &_szTmp){
		basic_str = _szTmp;
	}
	void operator=(const std::string &_szTmp){
		basic_str = _szTmp;
	}
	std::string content(){
		return basic_str;
	}

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
			pHead = new EZStringListNode;
			pHead->pPreNode = NULL;
			pHead->ez_str = _szTmp;
			pHead->pNextNode = NULL;
			pCurr = pHead;
			pTail = pHead;
			iNodeNum++;
		}else{
			pCurr->pNextNode = new EZStringListNode;
			pCurr->pNextNode->pPreNode = pCurr;
			pCurr = pCurr->pNextNode;
			pCurr->pNextNode = NULL;
			pTail = pCurr;
			pCurr->ez_str = _szTmp;
			iNodeNum++;
		}
		return true;
	}
	bool AddInHead(std::string &_szTmp){

		return true;
	}
private:
	EZStringListNode * pHead;
	EZStringListNode * pCurr;
	EZStringListNode * pTail;
	int iNodeNum;
};
}//end namespace EZ
