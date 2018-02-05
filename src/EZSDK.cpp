#include <iostream>
#include "EZFile.h"
#include "EZString.h"
#include "EZTime.h"
#include "EZNet.h"

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

void TestEZRemoveFile() {
	EZ::EZFileBase::CreateFile("./test.txt");
	EZ::EZFileBase::RemoveFile("./test.txt");
}

void TestEZCreateFile(){
	EZ::EZFileBase::CreateFile("./aaa.txt");
}

void TestEZString(){
	EZ::EZString szStr;
	szStr = "Hello";
	std::cout << szStr.content() << std::endl;

	EZ::EZStringList szStrList;
	std::string szText = "Hello";
	szStrList.AddInTail(szText);
	std::string szText1 = "Hello1";
	szStrList.AddInHead(szText1);
	std::string szText2 = "Hello2";
	szStrList.AddInHead(szText2);
	printf("%s %s %s\n", 
		szStrList[0]->content().c_str(),
		szStrList[1]->content().c_str(),
		szStrList[2]->content().c_str());
}

void TestEZStringWithOperator(){
	EZ::EZString szStr;
	szStr = "Hello";
	printf("%s\n",szStr["0:3"].content().c_str());
}

void TestEZStringWithOperatorPlus(){
	EZ::EZString szStr1,szStr2;
	szStr1 = "Good";
	szStr2 = "Well";
	EZ::EZString res = szStr1 + szStr2;
	printf("%s\n",res.content().c_str());
}

void TestEZStringSplitFunc(){
	std::string szText = "abc,1,2,999";
	EZ::EZString ez_string = szText;
	EZ::EZStringList ez_str_list = ez_string.split(',');
	//std::cout << ez_str_list[0]->content();
	printf("%s %s %s %s\n",
		ez_str_list[0]->content().c_str(),
		ez_str_list[1]->content().c_str(),
		ez_str_list[2]->content().c_str(),
		ez_str_list[3]->content().c_str());
}

void TestEZStringListInsert(){
	std::string szText = "abc,1,2";
	EZ::EZString ez_string = szText;
	EZ::EZStringList ez_str_list = ez_string.split(',');
	std::string ss = "aaa";
	ez_str_list.InsertAt(0,ss);
	ez_str_list.PrintList();
}

void TestEZTime(){
	EZ::EZTime _time;
	std::cout << _time.GetYear() << std::endl;
	std::cout << _time.GetMonth() << std::endl;
	std::cout << _time.GetDay() << std::endl;
	std::cout << _time.GetWeekDay() << std::endl;
	std::cout << _time.GetHour() << std::endl;
	std::cout << _time.GetMinutes() << std::endl;
	std::cout << _time.GetSeconds() << std::endl;
}

void TestEZNet(){
	EZ::EZUDP aUDP;
	aUDP.InitNet(8888,"192.168.199.103");
}

int main()
{
	TestEZNet();
	return 0;
}
