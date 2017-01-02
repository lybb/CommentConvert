#define _CRT_SECURE_NO_WARNINGS

#include"CommentConvert.h"

enum STATE state = NUL_STATE;

void Do_NUL_State(FILE *pfRead,FILE *pfWrite)      //����NULL״̬�µĴ���
{
	int first = 0;
	int second = 0;
	first = fgetc(pfRead);
	switch(first)
	{
	case '/':
		{
			second = fgetc(pfRead);
			if(second == '*')            //NULL״̬������/*ʱ������Ϊ//������C״̬
			{
				fputc('/',pfWrite);
				fputc('/',pfWrite);
				state = C_STATE;
			}
			else if(second == '/')       //NULL״̬������//ʱֱ�����
			{
				fputc(first,pfWrite);
				fputc(second,pfWrite);
				state = CPP_STATE;
			}
			else                         //NULL״̬������/�������������ַ�����ֱ�����
			{
				fputc(first,pfWrite);
				fputc(second,pfWrite);
			}
		}
		break;
	case EOF:
		state = END_STATE;
	default :
		fputc(first,pfWrite);
		break;
	}
}


void Do_Cpp_State(FILE *pfRead,FILE *pfWrite)      //C++״̬�µĴ���
{
	int first = 0;
	first = fgetc(pfRead);
	switch(first)
	{
	case '\n':                   //CPP״̬������'\n'��˵��ǰһ��ע�������
		fputc(first,pfWrite);
		state = NUL_STATE;
		break;
	case EOF:
		state = END_STATE;
		break;
	default:
		fputc(first,pfWrite);
		break;
	}
}


void Do_C_State(FILE *pfRead,FILE *pfWrite)        //C״̬�µĴ���
{
	int first = fgetc(pfRead);
	int second = 0;
	int third = 0;
	switch(first)
	{
	case '*':
		{
			 
			second = fgetc(pfRead);
			if(second == '/')       //ƥ������
			{ 
				third = fgetc(pfRead);

				//����ע��������ע������
				if (third != '\n')           
                {                             
					fputc('\n', pfWrite);     
					ungetc(third, pfRead);  
                }  
				else                         
                {                             
					fputc('\n', pfWrite); 
                } 
				state = NUL_STATE;   
				//���'*/'֮����'\n'��˵����second֮�����ַ�������ڴ˴�����һ�����У�����
				//������third�ַ����ص�pfRead��ȥ������״̬��Ϊ NUL_STATE
				//�����'\n'��˵��c����ע�����꣬��һ���ַ��п��ǡ�/��Ҳ�п����������ַ������
				//��״̬�ص� NUL_STATE
			}
			else if(second == '*')   //������**/����
			{
				third = fgetc(pfRead);
				fputc(first,pfWrite);
				if(third == '/')
					state = NUL_STATE;
			}
			else
			{
				fputc(first,pfWrite);
				fputc(second,pfWrite);
			}
		}
		break;
	case '\n':
		fputc(first,pfWrite);
		fputc('/',pfWrite);
		fputc('/',pfWrite);
		break;
	case EOF:
		state = END_STATE;
		break;
	default:
		fputc(first,pfWrite);
		break;
	}
}


void CommentConvert(FILE *pfRead,FILE *pfWrite)
{
	while(state != END_STATE)
	{
		switch(state)
		{
		case NUL_STATE:
			Do_NUL_State(pfRead,pfWrite);
			break;
		case C_STATE:
			Do_C_State(pfRead,pfWrite);
			break;
		case CPP_STATE:
			Do_Cpp_State(pfRead,pfWrite);
			break;
		case END_STATE:
			break;
		default:
			break;
		}
	}
}