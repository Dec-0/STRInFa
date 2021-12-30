#include <stdio.h>
#include <stdlib.h>  // for file handle;
#include <getopt.h> // for argument;
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "BasicRelated.h"
#include "STRRelated.h"

time_t Start;
unsigned int MinSTRBasicSize,MaxSTRBasicSize,MinRepeatNum,MaxRepeatNum;
unsigned int Flag4OverLap = 0;


// ———————————————
// 命令行参数输入;
unsigned int OptGet(int argc, char *argv[])
{
	unsigned char SMinSTRBasicSize[10] = "minsize";
	unsigned char SMaxSTRBasicSize[10] = "maxsize";
	unsigned char SMinRepeatNum[10] = "mindup";
	unsigned char SMaxRepeatNum[10] = "maxdup";
	unsigned char SFlag4OverLap[10] = "overlap";
	unsigned char Help[5] = "help";
	unsigned char OptHash[10];
	unsigned char Info[5000] = "\n\n\
 STRInFa_v1.0   For Short Tandem Repeat locate in fasta sequences.\n\
 用于定位Fasta序列中的短串联重复片段\n\n\
 Last revised date: 2021.12.30\n\
 Contact:zhangdong_xie@foxmail.com\n\n\
 -minsize     ( Optional ) Minimal size of STR basic unit (Default: 1, Minimal: 1).\n\
                           STR最小单元的最小长度（默认值：1，最小值：1）。\n\
 -maxsize     ( Optional ) Maximal size of STR basic unit (Default: 6, Maximal: 50).\n\
                           STR最小单元的最大长度（默认值：6，最大值：50）。\n\n\
 -mindup      ( Optional ) Minimal repeat number for STR sequence (Default: 5, Minimal: 2).\n\
                           STR基础序列的最少重复次数（默认值：5，最小值：2）。\n\
 -maxdup      ( Optional ) Maximal repeat number for STR sequence (Default: 50, Maximal: 1000).\n\
                           STR基础序列的最多重复次数（默认值：50，最大值1000）。\n\n\
 -overlap     ( Optional ) If allow overlap of two or more STRs, like 'TGAGAGAGAGAGAAAAAATCG' (Default: Disabled).\n\
                           假如允许多个STRs序列有重叠，比如GA和A。\n\n\
 -h/--help    ( Optional ) Help info.\n\n\
 \n";
	unsigned int i,j,k,Flag4Match,tId;
	
	// 逐一检查所有输入的参数;
	// OptHash用于记录对应序号的参数是否有指定;
	memset(OptHash,0,10*sizeof(unsigned char));
	for(i = 1;i < argc;i ++)
	{
		if(argv[i][0] == '-')
		{
			j = 1;
			// 以防参数命令以“--”开头;
			if(argv[i][j] == '-')
			{
				j ++;
			}
			
			// 逐个核实并确定对应的参数;
			Flag4Match = 0;
			if(argv[i][j + 6] && ! argv[i][j + 7])
			{
				if(! Flag4Match && ! OptHash[0])
				{
					Flag4Match = 1;
					tId = 0;
					for(k = j;k < j + 7;k ++)
					{
						if(argv[i][k] != SMinSTRBasicSize[tId])
						{
							Flag4Match = 0;
						}
						tId ++;
					}
					if(Flag4Match)
					{
						i ++;
						MinSTRBasicSize = String2Int(argv[i]);
						OptHash[0] = 1;
					}
				}
				
				if(! Flag4Match && ! OptHash[1])
				{
					Flag4Match = 1;
					tId = 0;
					for(k = j;k < j + 7;k ++)
					{
						if(argv[i][k] != SMaxSTRBasicSize[tId])
						{
							Flag4Match = 0;
						}
						tId ++;
					}
					if(Flag4Match)
					{
						i ++;
						MaxSTRBasicSize = String2Int(argv[i]);
						OptHash[1] = 1;
					}
				}
				
				if(! Flag4Match && ! OptHash[4])
				{
					Flag4Match = 1;
					tId = 0;
					for(k = j;k < j + 7;k ++)
					{
						if(argv[i][k] != SFlag4OverLap[tId])
						{
							Flag4Match = 0;
						}
						tId ++;
					}
					if(Flag4Match)
					{
						Flag4OverLap = 1;
						OptHash[4] = 1;
					}
				}
			}
			else if(argv[i][j + 5] && ! argv[i][j + 6])
			{
				if(! Flag4Match && ! OptHash[2])
				{
					Flag4Match = 1;
					tId = 0;
					for(k = j;k < j + 6;k ++)
					{
						if(argv[i][k] != SMinRepeatNum[tId])
						{
							Flag4Match = 0;
						}
						tId ++;
					}
					if(Flag4Match)
					{
						i ++;
						MinRepeatNum = String2Int(argv[i]);
						OptHash[2] = 1;
					}
				}
				
				if(! Flag4Match && ! OptHash[3])
				{
					Flag4Match = 1;
					tId = 0;
					for(k = j;k < j + 6;k ++)
					{
						if(argv[i][k] != SMaxRepeatNum[tId])
						{
							Flag4Match = 0;
						}
						tId ++;
					}
					if(Flag4Match)
					{
						i ++;
						MaxRepeatNum = String2Int(argv[i]);
						OptHash[3] = 1;
					}
				}
			}
			else if(argv[i][j] == Help[0])
			{
				if(argv[i][j + 1])
				{
					if(argv[i][j + 4] == 0)
					{
						tId = 1;
						for(k = j + 1;k < j + 4;k ++)
						{
							if(argv[i][k] != Help[tId])
							{
								printf("[ Error ] Wrong format for -help.%s",Info);
								exit(1);
							}
							tId ++;
						}
					}
					else
					{
						printf("[ Error ] Wrong format for --help.%s",Info);
						exit(1);
					}
				}
				
				printf("%s",Info);
				exit(1);
			}
			else
			{
				printf("[ Error ] Unknown argument: %s.%s",argv[i],Info);
				exit(1);
			}
			
			if(! Flag4Match)
			{
				printf("[ Error ] Unknown argument: %s.%s",argv[i],Info);
				exit(1);
			}
		}
		else
		{
			printf("[ Error ] Unknown argument: %s.%s",argv[i],Info);
			exit(1);
		}
	}
	
	// 默认值；
	if(! OptHash[0])
	{
		MinSTRBasicSize = 1;
	}
	if(! OptHash[1])
	{
		MaxSTRBasicSize = 6;
	}
	if(! OptHash[2])
	{
		MinRepeatNum = 5;
	}
	if(! OptHash[3])
	{
		MaxRepeatNum = 50;
	}
	
	// Min不能大于Max
	if(MinSTRBasicSize > MaxSTRBasicSize)
	{
		printf("[ Error ] MinSTRBasicSize > MaxSTRBasicSize.%s",Info);
		exit(1);
	}
	if(MinRepeatNum > MaxRepeatNum)
	{
		printf("[ Error ] MinRepeatNum > MaxRepeatNum.%s",Info);
		exit(1);
	}
	
	// 不能超出最大范围
	if(MinSTRBasicSize < 1)
	{
		printf("[ Error ] MinSTRBasicSize exceed limit 1.%s",Info);
		exit(1);
	}
	if(MaxSTRBasicSize > 50)
	{
		printf("[ Error ] MaxSTRBasicSize exceed limit 50.%s",Info);
		exit(1);
	}
	if(MinRepeatNum < 2)
	{
		printf("[ Error ] MinRepeatNum exceed limit 2.%s",Info);
		exit(1);
	}
	if(MaxRepeatNum > 1000)
	{
		printf("[ Error ] MaxRepeatNum exceed limit 1000.%s",Info);
		exit(1);
	}
	
	return 1;
}


// ——————————
// Main Part；
int main(int argc, char *argv[])
{
	unsigned char Cmd[10000],Other[10000];
	
	// 初始化;
	time(&Start);
	OptGet(argc,argv);
	TimeLog(Start,"Arguments initial done");
	
	// STR；
	STRLocate(MinSTRBasicSize,MaxSTRBasicSize,MinRepeatNum,MaxRepeatNum,Flag4OverLap);
	
	// 日志;
	TimeLog(Start,"The end");
}