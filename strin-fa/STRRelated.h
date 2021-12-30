#ifndef _STRRELATED_H
#define _STRRELATED_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 用于获得及补齐具体长度的序列
unsigned int FaSeqGet(unsigned char *CurrId,unsigned char *Seq,unsigned int MaxSeqSize,unsigned int MinSeqSize,unsigned int *SeqFrom,unsigned int *SeqTo,unsigned int *InitialBaseId,unsigned int AddNum);
// 判断指定UnitSize条件下的重复次数，不能重新读取文件，否则不能判断其它Size的条件；
unsigned int RepeatNumCount(unsigned char *Seq,unsigned int SeqFrom,unsigned int UnitSize,unsigned int MaxRepeatNum,unsigned int *RepeatNum);
// STR
unsigned int STRLocate(unsigned int MinSTRBasicSize,unsigned int MaxSTRBasicSize,unsigned int MinRepeatNum,unsigned int MaxRepeatNum,unsigned int Flag4OverLap);

#endif
