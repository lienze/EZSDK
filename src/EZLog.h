#pragma once

#include <fstream>
#include <string>

namespace EZ {
enum LOGTYPE
{
	SYSLOG = 1,
	CLIENTLOG = 2,
	ERRLOG = 3,
};
class EZLog
{
public:
	EZLog(){}
	~EZLog(){
		fLog.clear();
		fLog.close();
	}
	bool InitLog(std::string _szPath){
		fLog.open(_szPath,std::ios::app);
		if(fLog.fail()){
			printf("can not open %s\n",_szPath.c_str());
			return false;
		}
		else
			return true;
	}
	bool WriteData(std::string _szTmp){
		if(fLog.is_open()){
			fLog << _szTmp;
			return true;
		}
		return false;
	}
private:
	std::fstream fLog;
};//end class EZLog

class EZLogMan //EZLog Manager
{
public:
	EZLogMan(){}
	~EZLogMan(){}

};//end class EZLogMan
}//end namespace EZ
