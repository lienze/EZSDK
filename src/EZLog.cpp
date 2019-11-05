#include "EZLog.h"

namespace EZ {

bool EZLog::InitLog(std::string _szDir, std::string _szLogName) {
	std::string _szPath = _szDir + _szLogName;
	fLog.open(_szPath, std::ios::app);
	if (fLog.fail()) {
		printf("can not open %s\n", _szPath.c_str());
		return false;
	}
	return true;
}

bool EZLog::WriteData(std::string _szTmp) {
	if (fLog.is_open()) {
		//Get Current Time
		time(&raw_time);
		t_time = localtime(&raw_time);
		char _time[128];
		memset(_time, 0, 128);
		sprintf(_time, "%d/%02d/%02d %02d:%02d:%02d ",
			t_time->tm_year + 1900,
			t_time->tm_mon + 1,
			t_time->tm_mday,
			t_time->tm_hour,
			t_time->tm_min,
			t_time->tm_sec);
		fLog << _time << _szTmp << std::endl;
		return true;
	}
	return false;
}

void EZLog::CloseLog() {
	fLog.clear();
	fLog.close();
}

}//end namespace EZ
