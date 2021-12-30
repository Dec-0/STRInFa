#ifndef _BASICRELATED_H
#define _BASICRELATED_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 日志 
unsigned int TimeLog(time_t Start,unsigned char *String);

// 块读取
unsigned char MultiLineCap(unsigned char *BuffContent,unsigned int MaxBuffSize,unsigned int *BuffSize,unsigned int *LineStart,unsigned int *LineEnd);

// 数字/字符转换
unsigned char String2Char(unsigned char *String);
unsigned int String2Int(unsigned char *String);
unsigned int Int2String(unsigned int Num, unsigned char *Char);
unsigned int StringCopy2String(unsigned char *StringTo, unsigned char *StringFrom);
unsigned int StringCopy2StringInSpecRange(unsigned char *StringTo, unsigned char *StringFrom, unsigned int From, unsigned int To);
unsigned int CharAdd2String(unsigned char *StringMain, unsigned char Char);
unsigned int StringAdd2String(unsigned char *StringMain, unsigned char *String);
unsigned int StringShiftForward(unsigned char *String,unsigned int From,unsigned int To);
unsigned int StringShiftForward2(unsigned char *String,unsigned int From,unsigned int To);

#endif
