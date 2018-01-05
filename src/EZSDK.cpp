#include <iostream>
#include "EZFile.h"


int main()
{
	/*EZ::EZReadFile fFile;
	int iRet = fFile.FileOpen("./test.txt");
	if(iRet == 1)
	{
		std::string szContent;
		while(!(fFile.GetFin())->eof())
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
	}*/

	EZ::EZSaveFile fFile;
	int iRet = fFile.FileOpen("./test.txt");
	if (iRet == 1)
	{
		std::string szContent = "Hello world";
		fFile.SaveLine(szContent);
		fFile.FileClose();
	}
	return 0;
}
