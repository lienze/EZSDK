#include <iostream>
#include <fstream>
#include <string>

namespace EZ{
class CFile
{
public:
	CFile(){}
	~CFile(){}
	enum FileRetType{
		CAN_NOT_OPEN_READ_FILE = -1,
		CAN_NOT_OPEN_SAVE_FILE = -2,
		OPEN_FILE_SUCCEED      =  1,
	};
	virtual FileRetType FileOpen(std::string _path) = 0;
	virtual bool FileClose() = 0;
};

class CReadFile:public CFile
{
public:
	CReadFile() {};
	~CReadFile() {};
	FileRetType FileOpen(std::string _path){
		fin.open(_path, std::ios::in);
		if (fin.fail())
			return CAN_NOT_OPEN_READ_FILE;
		else
			return OPEN_FILE_SUCCEED;
	}
	bool FileClose(){
		fin.clear();
		fin.close();
		return true;
	}
	void GetLine(std::string &_szTmp){
		if (fin.is_open())
			getline(fin, _szTmp);
	}
	bool SetFilePtrToHead(){
		if (fin.is_open())
			fin.seekg(0, std::ios::beg);
		return true;
	}
	bool SetFilePtrToEnd(){
		if (fin.is_open())
			fin.seekg(0, std::ios::end);
		return true;
	}
	std::ifstream *GetFin() { return &fin; }
private:
	std::ifstream fin;
};

class CSaveFile:public CFile
{
public:
	CSaveFile() {};
	~CSaveFile() {};
	FileRetType FileOpen(std::string _path);
	bool FileClose();
private:
	std::fstream fout;
};
}//end namespace EZ
