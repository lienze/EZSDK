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
	EZString(std::string &_szTmp);
	void operator=(const std::string &_szTmp);
	EZString operator[](const std::string &_szTmp);
	EZString operator+(EZString &_szForAdd);
	std::string content();
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
	EZStringList();
	~EZStringList(){}
	EZString * operator[](int _idx);
	bool AddInTail(std::string &_szTmp);
	bool AddInHead(std::string &_szTmp);
	bool InsertAt(int _iPos, std::string &_szTmp);
	bool PrintList();
	inline int GetSize() const{
		return iNodeNum;
	}
	bool DeleteAll();
private:
	EZStringListNode * pHead;
	EZStringListNode * pCurr;
	EZStringListNode * pTail;
	int iNodeNum;
private:
	bool createList(std::string &_szTmp);
};

}//end namespace EZ
