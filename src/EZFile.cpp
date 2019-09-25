#include "EZFile.h"

namespace EZ {

bool EZFileBase::RemoveFile(std::string _path) {
	return remove(_path.c_str());
}

FileRetType EZFileBase::CreateFile(std::string _path) {
	std::ofstream ofile;
	ofile.open(_path, std::ios::out);
	if (ofile.fail()) {
		OFSTREAMCLOSE(ofile);
		return CAN_NOT_CREATE_FILE;
	}
	else {
		OFSTREAMCLOSE(ofile);
		return CREATE_FILE_SUCCEED;
	}
}

FileRetType EZReadFile::FileOpen(std::string _path) {
	fin.open(_path, std::ios::in);
	if (fin.fail())
		return CAN_NOT_OPEN_READ_FILE;
	else
		return OPEN_FILE_SUCCEED;
}

bool EZReadFile::FileClose() {
	fin.clear();
	fin.close();
	return true;
}

void EZReadFile::GetLine(std::string &_szTmp) {
	if (fin.is_open())
		getline(fin, _szTmp);
}

bool EZReadFile::SetFilePtrToHead() {
	if (fin.is_open()) {
		fin.clear();
		fin.seekg(0, std::ios::beg);
		return true;
	}
	return false;
}

bool EZReadFile::SetFilePtrToEnd() {
	if (fin.is_open()) {
		fin.clear();
		fin.seekg(0, std::ios::end);
		return true;
	}
	return false;
}

FileRetType EZSaveFile::FileOpen(std::string _path) {
	fout.open(_path, std::ios::app);
	if (fout.fail())
		return CAN_NOT_OPEN_READ_FILE;
	else
		return OPEN_FILE_SUCCEED;
}

bool EZSaveFile::FileClose() {
	fout.clear();
	fout.close();
	return true;
}

void EZSaveFile::SaveLine(std::string &_szTmp) {
	if (fout.is_open())
		fout << _szTmp;
}

// 文件夹操作
#if (defined __APPLE__) || (defined __linux__)
#include <sys/stat.h>
#include <unistd.h>
// 创建文件夹 for linux
bool CreateDir(std::string _dirName) {
	//TODO: 目前权限先设置为777，需要将权限修改为参数传入
	if (mkdir(_dirName.c_str(), 0777) == -1)
		return false;
	return true;
}

// 删除文件夹 for linux
bool DeleteDir(std::string _dirName) {
	//TODO: 由于linux权限问题，只允许删除空目录
	//所以此处应修改为递归删除目录下的所有文件后再
	//删除所要删除的文件夹
	if (rmdir(_dirName.c_str()) == -1)
		return false;
	return true;
}

// 判断文件夹是否存在 for linux
bool DirExsit(std::string _dirName)
{
	if (access(_dirName.c_str(), 0) == 0) {
		//printf("%s Exsit\n", _dirName.c_str());
		return true;
	}
	else {
		//printf("%s Not Exsit\n", _dirName.c_str());
		return false;
	}
}
#elif (defined _WIN32) || (defined _WIN64)
#include <io.h>
#include <direct.h>

// 创建文件夹 for win
bool CreateDir(std::string _dirName) {
	if (_mkdir(_dirName.c_str()) == -1)
		return false;
	return true;
}

// 删除文件夹 for win
bool DeleteDir(std::string _dirName) {
	if (_rmdir(_dirName.c_str()) == -1)
		return false;
	return true;
}

// 判断文件夹是否存在 for win
bool DirExsit(std::string _dirName)
{
	if (_access(_dirName.c_str(), 0) == 0) {
		//printf("%s Exsit\n", _dirName.c_str());
		return true;
	}
	else {
		//printf("%s Not Exsit\n", _dirName.c_str());
		return false;
	}
}
#endif

};// end of namespace EZ
