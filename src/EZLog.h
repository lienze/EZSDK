#pragma once

#include <fstream>
#include <string>
#include <ctime>
#include <map>

namespace EZ {

typedef enum{
	SYS    = 1,
	CLIENT = 2,
	ERR    = 3,
}LogType;

class EZLog
{
public:
	EZLog(){}
	~EZLog(){}
	bool InitLog(std::string _szDir,std::string _szLogName){
		std::string _szPath = _szDir + _szLogName;
		fLog.open(_szPath,std::ios::app);
		if(fLog.fail()){
			printf("can not open %s\n",_szPath.c_str());
			return false;
		}
		return true;
	}
	bool WriteData(std::string _szTmp){
		if(fLog.is_open()){
			//Get Current Time
			time(&raw_time);
			t_time = localtime(&raw_time);
			char _time[128];
			memset(_time,0,128);
			sprintf(_time,"%d/%02d/%02d %02d:%02d:%02d ",
				t_time->tm_year+1900,
				t_time->tm_mon+1,
				t_time->tm_mday,
				t_time->tm_hour,
				t_time->tm_min,
				t_time->tm_sec);
			fLog << _time << _szTmp << std::endl;
			return true;
		}
		return false;
	}
	void CloseLog(){
		fLog.clear();
		fLog.close();
	}
private:
	std::fstream fLog;
	time_t raw_time;
	struct std::tm *t_time;
};//end class EZLog

class EZLogMan //EZLog Manager
{
public:
	EZLogMan(){}
	~EZLogMan(){}
	bool AddLogger(EZLog *_pLog,LogType _Type){
		LogMap.insert({_Type,_pLog});
		return true;
	}
	EZLog * GetLogger(LogType _Type){
		return LogMap[_Type];
	}
private:
	std::map<LogType,EZLog *> LogMap;
};//end class EZLogMan

#define ELOG(__TYPE,__STR)                        \
    do{                                           \
        auto pLogger = g_LogMan.GetLogger(__TYPE);\
        if(pLogger)                               \
            pLogger->WriteData(__STR);            \
    }while(0);

}//end namespace EZ
