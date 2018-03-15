#include <iostream>
#include "EZFile.h"
#include "EZString.h"
#include "EZTime.h"
#include "EZNet.h"
#include "EZLog.h"
#include "EZDef.h"

using namespace EZ;

EZLogMan g_LogMan;

void TestEZReadFile() {
	EZReadFile fFile;
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

void TestEZString(){
	EZString szStr;
	szStr = "Hello";
	std::cout << szStr.content() << std::endl;

	EZStringList szStrList;
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
	EZString szStr;
	szStr = "Hello";
	printf("%s\n",szStr["0:3"].content().c_str());
}

void TestEZStringWithOperatorPlus(){
	EZString szStr1,szStr2;
	szStr1 = "Good";
	szStr2 = "Well";
	EZString res = szStr1 + szStr2;
	printf("%s\n",res.content().c_str());
}

void TestEZStringSplitFunc(){
	std::string szText = "abc,1,2,999";
	EZString ez_string = szText;
	EZStringList ez_str_list = ez_string.split(',');
	//std::cout << ez_str_list[0]->content();
	printf("%s %s %s %s\n",
		ez_str_list[0]->content().c_str(),
		ez_str_list[1]->content().c_str(),
		ez_str_list[2]->content().c_str(),
		ez_str_list[3]->content().c_str());
}

void TestEZStringListInsert(){
	std::string szText = "abc,1,2";
	EZString ez_string = szText;
	EZStringList ez_str_list = ez_string.split(',');
	std::string ss = "aaa";
	ez_str_list.InsertAt(0,ss);
	ez_str_list.PrintList();
}

void TestEZTime(){
	EZTime _time;
	std::cout << _time.GetYear() << std::endl;
	std::cout << _time.GetMonth() << std::endl;
	std::cout << _time.GetDay() << std::endl;
	std::cout << _time.GetWeekDay() << std::endl;
	std::cout << _time.GetHour() << std::endl;
	std::cout << _time.GetMinutes() << std::endl;
	std::cout << _time.GetSeconds() << std::endl;
}
//////////////////////////////////////////////////
// Funcionts for Client
//////////////////////////////////////////////////
void TestEZNetClient(){
	EZUDP aUDP;
	EZUDP bUDP;
	aUDP.InitNetSend("192.168.199.101",8888);
	aUDP.SendTo("Hello");
	EZNetMan g_NetMan;
	g_NetMan.AddUnit(&aUDP);
	g_NetMan.AddUnit(&bUDP);
	int _pos=0;
	_pos=g_NetMan.FindUnitPos(&bUDP);
	printf("g_NetMan == %d\n",_pos);
}

//////////////////////////////////////////////////
// Funcionts for Server
//////////////////////////////////////////////////
void Test_InitLogForServer(){
	EZLog *sysLog = new EZLog;
	if(sysLog)
		sysLog->InitLog("./Log/sys.txt");
	EZLog *clientLog = new EZLog;
	if(clientLog)
		clientLog->InitLog("./Log/client.txt");
	EZLog *errLog = new EZLog;
	if(errLog)
		errLog->InitLog("./Log/err.txt");
	g_LogMan.AddLogger(sysLog,SYSLOG);
	g_LogMan.AddLogger(clientLog,CLIENTLOG);
	g_LogMan.AddLogger(errLog,ERRORLOG);
}

void TestEZNetServer(){
	EZUDP aUDP;
	aUDP.InitNetRecv(8888);
	Test_InitLogForServer();
	ELOG(ERRORLOG,"Hello World!");
	//while(1){
	//	aUDP.RecvFrom();
	//}
}

int main(int argc,char *argv[])
{
	if(argc > 1){
		if(argc==2){//one parameter
			if(strcmp(argv[1],"-client")==0){
				printf("client running...\n");
				TestEZNetClient();
			}
			else if(strcmp(argv[1],"-server")==0){
				printf("server running...\n");
				TestEZNetServer();
			}
		}
	}
	return 0;
}
