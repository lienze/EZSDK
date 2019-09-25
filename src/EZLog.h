#pragma once

#include <fstream>
#include <ctime>
#include <map>
#include <cstring>

#define ERRORLOG  EZ::LogType::ERR
#define CLIENTLOG EZ::LogType::CLIENT
#define SYSLOG    EZ::LogType::SYS

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
	bool InitLog(std::string _szDir, std::string _szLogName);
	bool WriteData(std::string _szTmp);
	void CloseLog();
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
