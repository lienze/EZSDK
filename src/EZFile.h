#pragma once

#include <iostream>
#include <fstream>
#include <string>

#define OFSTREAMCLOSE(__FILE) \
    do                        \
    {                         \
        __FILE.clear();       \
        __FILE.close();       \
    } while(0)


#if (defined _WIN32) || (defined _WIN64)
//solve conflict with the define in <fileapi.h>
#define CreateFileA CreateFile
#endif

namespace EZ{

enum FileRetType {
	CAN_NOT_OPEN_READ_FILE = -1,
	CAN_NOT_OPEN_SAVE_FILE = -2,
	CAN_NOT_CREATE_FILE = -3,
	CREATE_FILE_SUCCEED = 1,
	OPEN_FILE_SUCCEED = 1,
};

class EZFileBase
{
public:
	EZFileBase(){}
	virtual ~EZFileBase(){}
	virtual FileRetType FileOpen(std::string _path) = 0;
	virtual bool FileClose() = 0;
	static bool RemoveFile(std::string _path);
	static FileRetType CreateFile(std::string _path);
};

class EZReadFile:public EZFileBase
{
public:
	EZReadFile(){}
	~EZReadFile(){}
	FileRetType FileOpen(std::string _path);
	bool FileClose();
	void GetLine(std::string &_szTmp);
	bool SetFilePtrToHead();
	bool SetFilePtrToEnd();
	std::ifstream *GetFin() { return &fin; }
private:
	std::ifstream fin;
};

class EZSaveFile:public EZFileBase
{
public:
	EZSaveFile(){}
	~EZSaveFile(){}
	FileRetType FileOpen(std::string _path);
	bool FileClose();
	void SaveLine(std::string &_szTmp);
private:
	std::fstream fout;
};

//文件夹操作
#if (defined __APPLE__) || (defined __linux__)
#include <sys/stat.h>
#include <unistd.h>
//创建文件夹 for linux
bool CreateDir(std::string _dirName);
//删除文件夹 for linux
bool DeleteDir(std::string _dirName);
//判断文件夹是否存在 for linux
bool DirExsit(std::string _dirName);
#elif (defined _WIN32) || (defined _WIN64)
#include <io.h>
#include <direct.h>
//创建文件夹 for win
bool CreateDir(std::string _dirName);
//删除文件夹 for win
bool DeleteDir(std::string _dirName);
//判断文件夹是否存在 for win
bool DirExsit(std::string _dirName);
#endif


}//end namespace EZ

