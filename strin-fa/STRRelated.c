#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "STRRelated.h"
#include "BasicRelated.h"


// 用于获得及补齐具体长度的序列（确保前后都会留下最大的查找区间MaxSTRBasicSize*MaxRepeatNum）
unsigned int FaSeqGet(unsigned char *CurrId,unsigned char *Seq,unsigned int MaxSeqSize,unsigned int MinSeqSize,unsigned int *SeqFrom,unsigned int *SeqTo,unsigned int *InitialBaseId,unsigned int AddNum)
{
	unsigned char BuffContent[200000];
	static unsigned char SId[1000] = "\0";
	unsigned int MaxBuffSize = 100000;
	static unsigned int BuffSize = 0;
	static unsigned int LineStart = 0;
	static unsigned int LineEnd = 0;
	unsigned int tFrom,tTo,i;
	
	// 当存在残留序列时，需要前移保留；
	if(*SeqFrom <= *SeqTo)
	{
		// 序列前移；
		tFrom = 1;
		if(*SeqFrom > MinSeqSize)
		{
			tFrom = *SeqFrom - MinSeqSize;
		}
		if(tFrom > 1)
		{
			StringShiftForward2(Seq,tFrom,*SeqTo);
		}
		*SeqTo = *SeqTo - tFrom + 1;
		*SeqFrom = *SeqFrom - tFrom + 1;
		*InitialBaseId += tFrom - 1;
	}
	
	// 假如上次碰到的是FaID，则覆盖即可；
	if(SId[0])
	{
		StringCopy2String(CurrId,SId);
		*InitialBaseId = 1;
		*SeqFrom = 1;
		*SeqTo = 0;
		Seq[0] = ' ';
	}
	
	while(MultiLineCap(BuffContent,MaxBuffSize,&BuffSize,&LineStart,&LineEnd))
	{
		if(BuffContent[LineStart] == '>')
		{
			if(*SeqFrom > *SeqTo)
			{
				// 上一条fa已经处理完毕；
				tFrom = LineStart + 1;
				tTo = LineEnd;
				if(BuffContent[LineEnd] == '\n')
				{
					tTo --;
				}
				StringCopy2StringInSpecRange(CurrId,BuffContent,tFrom,tTo);
				
				*InitialBaseId = 1;
				*SeqFrom = 1;
				*SeqTo = 0;
				Seq[0] = ' ';
			}
			else
			{
				// 上一条fa仍未处理完毕；
				tFrom = LineStart + 1;
				tTo = LineEnd;
				if(BuffContent[LineEnd] == '\n')
				{
					tTo --;
				}
				StringCopy2StringInSpecRange(SId,BuffContent,tFrom,tTo);
				break;
			}
		}
		else
		{
			// 假如fa标签为空
			if(! CurrId[0])
			{
				StringShiftForward(BuffContent,LineStart,LineEnd);
				printf("[ Error ] Seq without ID: %s\n",BuffContent);
				exit(1);
			}
			SId[0] = '\0';
			// 直接加在序列末尾即可。
			for(i = LineStart;i <= LineEnd;i ++)
			{
				if(BuffContent[i] == '\n' || BuffContent[i] == '\t' || BuffContent[i] == ' ')
				{
					continue;
				}
				// 小写转大写
				if(BuffContent[i] >= 97 && BuffContent[i] <= 122)
				{
					BuffContent[i] -= 32;
				}
				*SeqTo = *SeqTo + 1;
				Seq[*SeqTo] = BuffContent[i];
			}
			Seq[*SeqTo + 1] = '\0';
			// 假如超过了长度上限立马返回；
			if(*SeqTo > MaxSeqSize)
			{
				break;
			}
		}
	}
	
	// 假如没有数据了
	if(*SeqFrom > *SeqTo)
	{
		return 0;
	}
	
	// 序列长度不足时需要置空一定的长度范围，便于后续计算。
	if(*SeqTo < MaxSeqSize)
	{
		tTo = *SeqTo + 1;
		for(i = 0;i < AddNum;i ++)
		{
			tTo ++;
			Seq[tTo] = ' ';
		}
		Seq[tTo + 1] = '\0';
	}
	
	return 1;
}

// 判断指定UnitSize条件下的重复次数，不能重新读取文件，否则不能判断其它Size的条件；
unsigned int RepeatNumCount(unsigned char *Seq,unsigned int SeqFrom,unsigned int UnitSize,unsigned int MaxRepeatNum,unsigned int *RepeatNum)
{
	unsigned int MatchFlag,PairFrom,Shift;
	
	// 逆向检查，假如前向能匹配，那么此前一定计算过。
	if(SeqFrom > 1)
	{
		if(Seq[SeqFrom - 1] == Seq[SeqFrom + UnitSize - 1])
		{
			return 1;
		}
	}
	
	// 是否重复2次；
	MatchFlag = 1;
	PairFrom = SeqFrom + UnitSize;
	for(Shift = 0;Shift < UnitSize;Shift ++)
	{
		if(Seq[SeqFrom + Shift] != Seq[PairFrom + Shift])
		{
			MatchFlag = 0;
			break;
		}
	}
	
	// 是否重复3~N次；
	if(MatchFlag)
	{
		// 检查重复的次数；
		*RepeatNum += 1;
		
		while(MatchFlag && *RepeatNum <= MaxRepeatNum)
		{
			PairFrom += UnitSize;
			for(Shift = 0;Shift < UnitSize;Shift ++)
			{
				if(Seq[SeqFrom + Shift] != Seq[PairFrom + Shift])
				{
					MatchFlag = 0;
					break;
				}
			}
			if(MatchFlag)
			{
				*RepeatNum += 1;
			}
		}
	}

	return 0;
}

// STR
unsigned int STRLocate(unsigned int MinSTRBasicSize,unsigned int MaxSTRBasicSize,unsigned int MinRepeatNum,unsigned int MaxRepeatNum,unsigned int Flag4OverLap)
{
	unsigned char CurrId[1000],Seq[600000],Flag4MinimalUnit[1000],UnitSeq[1000];
	unsigned int MaxSeqSize,MinSeqSize,CalLimit,SeqFrom,SeqTo,InitialBaseId,AddNum,UnitSize,RepeatNum,Flag4Forward,Pos,tId,tLimit,MatchFlag,PairFrom,i,j;
	
	// 保留前向MinSeqSize以便复核。
	MaxSeqSize = 500000;
	MinSeqSize = 100000;
	CalLimit = 400000;
	AddNum = MaxSTRBasicSize * MaxRepeatNum;
	// 初始需要设置From大于To
	SeqFrom = 1;
	SeqTo = 0;
	InitialBaseId = 1;
	while(FaSeqGet(CurrId,Seq,MaxSeqSize,MinSeqSize,&SeqFrom,&SeqTo,&InitialBaseId,AddNum))
	{
		// 只计算到固定位置，后向留够核实空间；
		tLimit = CalLimit;
		if(SeqTo <= CalLimit)
		{
			tLimit = SeqTo;
		}
		// 超出设定的阈值或者起始坐标大于结束坐标就结束；
		while(SeqFrom <= tLimit)
		{
			// 计算最小单位，2bp2次重复就要排除4bp，该4bp一定不是最小单位。
			memset(Flag4MinimalUnit,0,MaxSTRBasicSize * sizeof(unsigned char));
			for(UnitSize = MinSTRBasicSize;UnitSize <= MaxSTRBasicSize;UnitSize ++)
			{
				// 跳过非最小重复单元；
				if(Flag4MinimalUnit[UnitSize - 1])
				{
					continue;
				}
				RepeatNum = 1;
				Flag4Forward = RepeatNumCount(Seq,SeqFrom,UnitSize,MaxRepeatNum,&RepeatNum);
				// 最小重复单元；
				for(i = 2;i <= RepeatNum;i ++)
				{
					j = UnitSize * i - 1;
					if(j >= MaxSTRBasicSize)
					{
						break;
					}
					Flag4MinimalUnit[j] = 1;
				}
				// 不靠前或者重复次数不达标；
				if(RepeatNum < MinRepeatNum || RepeatNum > MaxRepeatNum || Flag4Forward)
				{
					continue;
				}
				
				// 满足条件时需要记录；
				tId = SeqFrom + UnitSize - 1;
				StringCopy2StringInSpecRange(UnitSeq,Seq,SeqFrom,tId);
				Pos = InitialBaseId + SeqFrom - 1;
				printf("%s\t%d\t%s\t%d\n",CurrId,Pos,UnitSeq,RepeatNum);
				// 是否允许多个STR重叠；
				if(! Flag4OverLap)
				{
					SeqFrom += UnitSize * RepeatNum - 1;
				}
				break;
			}
			
			SeqFrom ++;
		}
	}
	
	return 1;
}