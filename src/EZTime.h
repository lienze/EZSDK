#pragma once

#include <ctime>

namespace EZ {

class EZTime
{
public:
	EZTime();
	~EZTime();
	bool UpdateTime();
	std::tm *GetTime();
	int GetYear();
	int GetMonth();
	int GetDay();
	int GetWeekDay();
	int GetHour();
	int GetMinutes();
	int GetSeconds();
private:
	time_t raw_time;
	struct std::tm *t_time;
};

}//end namespace EZ
