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
	if (fin.is_open())
		fin.seekg(0, std::ios::beg);
	return true;
}
bool EZReadFile::SetFilePtrToEnd() {
	if (fin.is_open())
		fin.seekg(0, std::ios::end);
	return true;
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


//�ļ��в���
#if (defined __APPLE__) || (defined __linux__)
#include <sys/stat.h>
#include <unistd.h>
//�����ļ��� for linux
bool CreateDir(std::string _dirName) {
	//TODO: ĿǰȨ��������Ϊ777����Ҫ��Ȩ���޸�Ϊ��������
	if (mkdir(_dirName.c_str(), 0777) == -1)
		return false;
	return true;
}

//ɾ���ļ��� for linux
bool DeleteDir(std::string _dirName) {
	//TODO: ����linuxȨ�����⣬ֻ����ɾ����Ŀ¼
	//���Դ˴�Ӧ�޸�Ϊ�ݹ�ɾ��Ŀ¼�µ������ļ�����
	//ɾ����Ҫɾ�����ļ���
	if (rmdir(_dirName.c_str()) == -1)
		return false;
	return true;
}

//�ж��ļ����Ƿ���� for linux
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

//�����ļ��� for win
bool CreateDir(std::string _dirName) {
	if (_mkdir(_dirName.c_str()) == -1)
		return false;
	return true;
}

//ɾ���ļ��� for win
bool DeleteDir(std::string _dirName) {
	if (_rmdir(_dirName.c_str()) == -1)
		return false;
	return true;
}

//�ж��ļ����Ƿ���� for win
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
