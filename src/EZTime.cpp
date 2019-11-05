#include "EZTime.h"

namespace EZ {

EZTime::EZTime() {
	t_time = NULL;
}
EZTime::~EZTime() {}
bool EZTime::UpdateTime() {
	time(&raw_time);
	t_time = localtime(&raw_time);
	return true;
}
std::tm *EZTime::GetTime() {
	if (t_time == NULL) {
		UpdateTime();
	}
	return t_time;
}
int EZTime::GetYear() {
	return GetTime()->tm_year + 1900;
}
int EZTime::GetMonth() {
	return GetTime()->tm_mon + 1;
}
int EZTime::GetDay() {
	return GetTime()->tm_mday;
}
int EZTime::GetWeekDay() {
	int iWeekDayTmp = 0;
	return (iWeekDayTmp = GetTime()->tm_wday) == 0 ? 7 : iWeekDayTmp;
}
int EZTime::GetHour() {
	return GetTime()->tm_hour;
}
int EZTime::GetMinutes() {
	return GetTime()->tm_min;
}
int EZTime::GetSeconds() {
	return GetTime()->tm_sec;
}

}//end namespace EZ
