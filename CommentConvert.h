#ifndef _COMMENT_CONVERT_H__
#define _COMMENT_CONVERT_H__

#include<stdio.h>
#include<stdlib.h>


#define INPUTNAME "input.c"
#define OUTPUTNAME "ouput.c"

enum STATE
{
	NUL_STATE,
	C_STATE,
	CPP_STATE,
	END_STATE
};

void CommentConvert(FILE *pfRead,FILE *pfWrite);
void Do_NUL_State(FILE *pfRead,FILE *pfWrite);
void Do_C_State(FILE *pfRead,FILE *pfWrite);
void Do_Cpp_State(FILE *pfRead,FILE *pfWrite);


#endif // _COMMENT_CONVERT_H__