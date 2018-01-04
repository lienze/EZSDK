#include <iostream>
#include "EZFile.h"


int main()
{
	EZ::CReadFile fFile;
	int iRet = fFile.FileOpen("./test.txt");
	if(iRet == 1)
	{
		while(!(fFile.GetFin())->eof())
		{
			std::string szContent;
			fFile.GetLine(szContent);
			std::cout << szContent << std::endl;
		}
	}
	return 0;
}
