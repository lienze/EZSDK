#include <iostream>
#include "EZFile.h"
#include "EZString.h"

void TestEZReadFile() {
	EZ::EZReadFile fFile;
	int iRet = fFile.FileOpen("./test.txt");
	if (iRet == 1)
	{
		std::string szContent;
		while (!(fFile.GetFin())->eof())
		{
			fFile.GetLine(szContent);
			std::cout << szContent << std::endl;
		}
		fFile.SetFilePtrToHead();
		while (!(fFile.GetFin())->eof())
		{
			fFile.GetLine(szContent);
			std::cout << szContent << std::endl;
		}
	}
}

void TestEZSaveFile(){
	EZ::EZSaveFile fFile;
	int iRet = fFile.FileOpen("./test.txt");
	if (iRet == 1)
	{
		std::string szContent = "Hello world";
		fFile.SaveLine(szContent);
		fFile.FileClose();
	}
}

void TestEZString(){
	EZ::EZString szStr;
	szStr = "Hello";
	std::cout << szStr.content() << std::endl;

	EZ::EZStringList szStrList;
	std::string szText = "Hello";
	szStrList.AddInTail(szText);
	std::string szText1 = "Hello1";
	szStrList.AddInTail(szText1);
	printf("%s", szStrList[1]->content().c_str());
}


int main()
{
	TestEZString();
	return 0;
}
