#pragma once

#include <string>

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

class EZStringList
{
public:
	EZStringList(){}
	~EZStringList(){}

};
}//end namespace EZ
