#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "BasicRelated.h"

// 日志
unsigned int TimeLog(time_t Start,unsigned char *String)
{
	time_t End,DurMin,DurSec,DurHour,DurDay;
	char *Currt;
	unsigned int i;
	
	time(&End);
	Currt = asctime(localtime(&End));
	i = 0;
	while(*(Currt + i))
	{
		i ++;
	}
	i = i - 1;
	for(i;i > 0;i --)
	{
		if(*(Currt + i) == ' ')
		{
			*(Currt + i) = 0;
			break;
		}
	}
	
	DurSec = End - Start;
	if(DurSec < 60)
	{
		printf("[ %s  %us ] %s.\n",Currt,DurSec,String);
	}
	else
	{
		DurMin = (int)(DurSec / 60);
		DurSec = (int)(DurSec % 60);
		if(DurMin < 60)
		{
			printf("[ %s  %umin%us ] %s.\n",Currt,DurMin,DurSec,String);
		}
		else
		{
			DurHour = (int)(DurMin / 60);
			DurMin = (int)(DurMin % 60);
			if(DurHour < 24)
			{
				printf("[ %s  %uh%umin ] %s.\n",Currt,DurHour,DurMin,String);
			}
			else
			{
				DurDay = (int)(DurHour / 24);
				DurHour = (int)(DurHour % 24);
				printf("[ %s  %ud%uh%umin ] %s.\n",Currt,DurDay,DurHour,DurMin,String);
			}
		}
	}
	fflush(stdout);
	
	return 1;
}

// 块读取
unsigned char MultiLineCap(unsigned char *BuffContent,unsigned int MaxBuffSize,unsigned int *BuffSize,unsigned int *LineStart,unsigned int *LineEnd)
{
	unsigned char BKBuffContent[200000];
	unsigned int Item;
	
	// 假如初始状态或者读入的数据已处理完，需要重新读入数据；
	if(*LineEnd == *BuffSize)
	{
		*BuffSize = fread(BuffContent,1,MaxBuffSize,stdin);
		BuffContent[*BuffSize] = '\0';
		if(*BuffSize == 0)
		{
			return 0;
		}
		*LineStart = 0;
		*LineEnd = 0;
	}
	else
	{
		*LineStart = *LineEnd + 1;
		*LineEnd = *LineStart;
	}
	
	Item = *LineEnd;
	for(Item;Item < *BuffSize;Item ++)
	{
		*LineEnd = Item;
		if(BuffContent[Item] == '\n')
		{
			return 1;
		}
	}
	
	// relocating;
	StringCopy2StringInSpecRange(BKBuffContent,BuffContent,*LineStart,*LineEnd);
	*BuffSize = fread(BuffContent,1,MaxBuffSize,stdin);
	BuffContent[*BuffSize] = '\0';
	if(*BuffSize == 0 && *LineStart > *LineEnd)
	{
		return 0;
	}
	StringAdd2String(BKBuffContent,BuffContent);
	*BuffSize = StringCopy2String(BuffContent,BKBuffContent);
	*LineStart = 0;
	*LineEnd = 0;
	Item = *LineEnd;
	for(Item;Item < *BuffSize;Item ++)
	{
		*LineEnd = Item;
		if(BuffContent[Item] == '\n')
		{
			return 1;
		}
	}
	if(*LineEnd + 1 == *BuffSize)
	{
		return 1;
	}
	
	return 0;
}

// 数字/字符转换
unsigned char String2Char(unsigned char *String)
{
	unsigned char i,Multi,Total;
	
	Multi = 10;
	Total = 0;
	i = 0;
	while(String[i])
	{
		Total = Total * Multi + String[i] - 48;
		i ++;
	}
	
	return Total;
}
unsigned int String2Int(unsigned char *String)
{
	unsigned int i,Multi,Total;
	
	Multi = 10;
	Total = 0;
	i = 0;
	while(String[i])
	{
		Total = Total * Multi + String[i] - 48;
		i ++;
	}
	
	return Total;
}
unsigned int Int2String(unsigned int Num, unsigned char *Char)
{
	unsigned int i,BitNum,tmpId;
	
	BitNum = 10;
	for(i = 1;i < 20;i ++)
	{
		if(Num < BitNum)
		{
			break;
		}
		BitNum = BitNum * 10;
	}
	BitNum = BitNum / 10;
	
	tmpId = 0;
	while(BitNum)
	{
		Char[tmpId] = (unsigned int)(Num / BitNum) % 10 + 48;
		BitNum = (unsigned int)(BitNum / 10);
		tmpId ++;
	}
	Char[tmpId] = '\0';
	
	return tmpId;
}
unsigned int StringCopy2String(unsigned char *StringTo, unsigned char *StringFrom)
{
	unsigned int i = 0;
	
	while(StringFrom[i])
	{
		StringTo[i] = StringFrom[i];
		i ++;
	}
	StringTo[i] = '\0';
	
	return i;
}
unsigned int StringCopy2StringInSpecRange(unsigned char *StringTo, unsigned char *StringFrom, unsigned int From, unsigned int To)
{
	unsigned int i,j;
	
	j = 0;
	for(i = From;i <= To;i ++)
	{
		StringTo[j] = StringFrom[i];
		j ++;
	}
	StringTo[j] = '\0';
	
	return j;
}
unsigned int CharAdd2String(unsigned char *StringMain, unsigned char Char)
{
	unsigned int i = 0;
	unsigned int j = 0;
	
	while(StringMain[i])
	{
		i ++;
	}
	StringMain[i] = Char;
	i ++;
	StringMain[i] = '\0';
	
	return i;
}
unsigned int StringAdd2String(unsigned char *StringMain, unsigned char *String)
{
	unsigned int i = 0;
	unsigned int j = 0;
	
	while(StringMain[i])
	{
		i ++;
	}
	while(String[j])
	{
		StringMain[i] = String[j];
		i ++;
		j ++;
	}
	StringMain[i] = '\0';
	
	return i;
}
unsigned int StringShiftForward(unsigned char *String,unsigned int From,unsigned int To)
{
	unsigned int i,j;
	
	j = 0;
	for(i = From;i <= To;i ++)
	{
		String[j] = String[i];
		j ++;
	}
	String[j] = '\0';
	
	return j;
}
unsigned int StringShiftForward2(unsigned char *String,unsigned int From,unsigned int To)
{
	unsigned int i,j;
	
	String[0] = ' ';
	j = 1;
	for(i = From;i <= To;i ++)
	{
		String[j] = String[i];
		j ++;
	}
	String[j] = '\0';
	
	return j;
}
