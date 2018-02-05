#pragma once

#include <ctime>

namespace EZ {

class EZTime
{
public:
	EZTime(){
		t_time = NULL;
	}
	~EZTime(){}
	bool UpdateTime(){
		time(&raw_time);
		t_time = localtime(&raw_time);
		return true;
	}
	std::tm *GetTime(){
		if(t_time==NULL){
			UpdateTime();
		}
		return t_time;
	}
	int GetYear(){
		return GetTime()->tm_year + 1900;
	}
	int GetMonth(){
		return GetTime()->tm_mon + 1;
	}
	int GetDay(){
		return GetTime()->tm_mday;
	}
	int GetWeekDay(){
		int iWeekDayTmp = 0;
		return (iWeekDayTmp = GetTime()->tm_wday)==0?7:iWeekDayTmp;
	}
	int GetHour(){
		return GetTime()->tm_hour;
	}
	int GetMinutes(){
		return GetTime()->tm_min;
	}
	int GetSeconds(){
		return GetTime()->tm_sec;
	}
private:
	time_t raw_time;
	struct std::tm *t_time;
};

}//end namespace EZ
