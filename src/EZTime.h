#pragma once

#include <ctime>

namespace EZ {

class EZTime
{
public:
	EZTime(){}
	~EZTime(){}
	std::tm *GetCurrentTime(){
		time(&raw_time);
		t_time = localtime(&raw_time);
		return t_time;
	}
	int GetCurrentYear(){
		return GetCurrentTime()->tm_year + 1900;
	}
	int GetCurrentMonth(){
		return GetCurrentTime()->tm_mon + 1;
	}
	int GetCurrentDay(){
		return GetCurrentTime()->tm_mday;
	}
private:
	time_t raw_time;
	struct std::tm *t_time;
};

}//end namespace EZ
