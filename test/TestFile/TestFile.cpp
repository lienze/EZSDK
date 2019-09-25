#include <iostream>
#include "EZFile.h"

using namespace EZ;

void TestEZReadFile()
{
	EZReadFile fFile;
	int iRet = fFile.FileOpen("./test.txt");
	if (iRet == 1) {
		std::string szContent;
		while (!(fFile.GetFin())->eof()) {
			fFile.GetLine(szContent);
			std::cout << "1."+szContent << std::endl;
		}
		fFile.SetFilePtrToHead();
		while (!(fFile.GetFin())->eof()) {
			fFile.GetLine(szContent);
			std::cout << "2."+szContent << std::endl;
		}
	}
}

/*
void TestEZSaveFile(){
	EZSaveFile fFile;
	int iRet = fFile.FileOpen("./test.txt");
	if (iRet == 1)
	{
		std::string szContent = "Hello world";
		fFile.SaveLine(szContent);
		fFile.FileClose();
	}
}

void TestEZRemoveFile() {
	EZFileBase::CreateFile("./test.txt");
	EZFileBase::RemoveFile("./test.txt");
}

void TestEZCreateFile(){
	EZFileBase::CreateFile("./aaa.txt");
}

void TestEZDir() {
	if (DirExsit("./test") == false)
		CreateDir("./test");
}
*/

int main(int argc,char *argv[])
{
	TestEZReadFile();
	return 0;
}
