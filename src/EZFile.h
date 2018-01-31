#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace EZ{
class EZFileBase
{
public:
	EZFileBase(){}
	~EZFileBase(){}
	enum FileRetType {
		CAN_NOT_OPEN_READ_FILE = -1,
		CAN_NOT_OPEN_SAVE_FILE = -2,
		CAN_NOT_CREATE_FILE    = -3,
		CREATE_FILE_SUCCEED    =  1,
		OPEN_FILE_SUCCEED      =  1,
	};
	virtual FileRetType FileOpen(std::string _path) = 0;
	virtual bool FileClose() = 0;
	static bool RemoveFile(std::string _path){
		return remove(_path.c_str());
	}
	static FileRetType CreateFile(std::string _path){
		std::ofstream ofile;
		ofile.open(_path, std::ios::out);
		if (ofile.fail()) {
			ofile.clear();
			ofile.close();
			return CAN_NOT_CREATE_FILE;
		}
		else {
			ofile.clear();
			ofile.close();
			return CREATE_FILE_SUCCEED;
		}
	}
};

class EZReadFile:public EZFileBase
{
public:
	EZReadFile() {};
	~EZReadFile() {};
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

class EZSaveFile:public EZFileBase
{
public:
	EZSaveFile() {};
	~EZSaveFile() {};
	FileRetType FileOpen(std::string _path){
		fout.open(_path, std::ios::app);
		if (fout.fail())
			return CAN_NOT_OPEN_READ_FILE;
		else
			return OPEN_FILE_SUCCEED;
	}
	bool FileClose(){
		fout.clear();
		fout.close();
		return true;
	}
	void SaveLine(std::string &_szTmp) {
		if (fout.is_open())
			fout << _szTmp;
	}
private:
	std::fstream fout;
};
}//end namespace EZ
