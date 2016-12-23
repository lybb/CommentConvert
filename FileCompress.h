#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "HuffmanTree.h"
#include <string>
#include <Windows.h>
#include <cassert>

typedef long long LongType;
struct CharInfo
{
	CharInfo(LongType count=0)
		:_count(count)
	{}

	CharInfo operator+(const CharInfo& info)
	{
		return CharInfo(_count + info._count);
	}
	bool operator< (const CharInfo& info)
	{
		return _count < info._count;
	}
	bool operator !=(const CharInfo& info)
	{
		return _count != info._count;
	}


	unsigned char _ch;	//���ֵ��ַ�
	LongType _count;	//�ַ����ֵĴ���
	string _code;		//�ַ�����
};

class FileCompress
{
public:
	FileCompress()
	{
		for (size_t i=0; i<256; i++)
		{
			_info[i]._ch = i;
			_info[i]._count = 0;
		}
	}   
public:
	string Compress(const char* filename)		//�ļ�ѹ��
	{
		FILE* fout = fopen(filename,"rb");
		assert(fout);
		//ÿ�δ��ļ��ж�ȡһ���ַ���ͳ�Ƹ��ַ����ֵĴ���
		unsigned char ch = fgetc(fout);
		while(!feof(fout))
		{
			_info[ch]._count ++;
			ch = fgetc(fout);
		}

		//�ø��ַ��Ĵ���������������
		CharInfo invalid;
		HuffmanTree<CharInfo> h(_info,256,invalid);

		//�õ�����������
		string code;
		GenerateHuffmanTreeCode(h.GetRoot(),code);
		//GenerateHuffmanTreeCode(h.GetRoot());

		//����ѹ��
		string compressfile = filename;
		size_t index1 = compressfile.rfind('.');
		compressfile = compressfile.substr(0,index1);
		compressfile += ".huffman";	//�ȸı��ļ��ĺ�׺
		FILE* fin = fopen(compressfile.c_str(),"wb");//��д��ķ�ʽ��"compressfile.huffman"
		assert(fin);
		fseek(fout,0,SEEK_SET);	//�����ļ�ָ���λ��Ϊ�ļ���ͷ
		int pos = 0;
		char value = 0;
		unsigned char c = fgetc(fout);
		//while (c != EOF)
		while(!feof(fout))
		{
			//���ַ�תΪ������λ����8λ��д��
			string& code = _info[c]._code;
			for (size_t i=0; i<code.size(); ++i)
			{
				value <<= 1;
				if (code[i] == '1')	
					value |= 1;
				else
					value |= 0;
				++pos;	//��ʾ8λ������λ
				if(pos == 8)	//��8λ�ͽ�valueд��ѹ���ļ�
				{
					fputc(value,fin);
					pos = 0;
					value = 0;
				}
			}
			c = fgetc(fout);	//��ȡ��һ���ַ�
		}
		if (pos)	//pos==0��˵���պô���8λ��pos!=0��˵�����ж����λ
		{
			value <<= (8-pos);
			fputc(value,fin);
		}
		fclose(fin);
		fclose(fout);

		//�����ļ�---�洢�ַ����ֵĴ���
		string configfilename = filename;
		size_t index2 = configfilename.rfind('.');
		configfilename = configfilename.substr(0,index1);
		configfilename += ".config";
		FILE* foutConfig = fopen(configfilename.c_str(),"wb");
		string line;
		char buff[128];
		for(size_t i=0; i<256; ++i)
		{
			if (_info[i]._count)
			{
				line += _info[i]._ch;
				line += ',';
				line += _itoa(_info[i]._count,buff,10);
				line += '\n';
				fputs(line.c_str(),foutConfig);
				//fwrite(line.c_str(),1,line.size(),foutConfig);//��Ƶ����Ƶ
				line.clear();
			}
		}
		fclose(foutConfig);
		return compressfile;
	}
	//��ȡ�ļ���һ�� 
	bool ReadLine(FILE* configfile,string& line)
	{
		char ch = fgetc(configfile);
		if (ch == EOF)
			return false;
		while (ch != EOF && ch != '\n')
		{
			line += ch;
			ch = fgetc(configfile);
		}
		return true;
	}
	string UnCompress(const char* filename)
	{
		assert(filename);
		//��ȡ�����ļ�---��ȡ�ַ����ֵĴ���
		string configfilename = filename;	//filename --- "file.huffman"	�����ļ�---"file.config"
		size_t index1 = configfilename.rfind('.');
		configfilename = configfilename.substr(0,index1);
		configfilename += ".config";
		FILE* foutConfig = fopen(configfilename.c_str(),"rb");
		assert(foutConfig);
		string line;
		unsigned char ch = 0;
		while (ReadLine(foutConfig,line))
		{
			if (line.empty())
			{
				line += '\n';
				continue;
			}
			else
			{
				ch = line[0];
				_info[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
		}
		//����Huaaman��
		CharInfo invalid;
		HuffmanTree<CharInfo> h1(_info,256,invalid);
		//���ڵ��ȨֵΪ�ַ����ֵ��ܺ�
		LongType charCount = h1.GetRoot()->_weight._count;
		//ѹ���ļ�
		string name = filename;
		size_t index = name.rfind('.');
		name = name.substr(0,index);
		name += ".huffman";
		FILE* fout = fopen(name.c_str(),"rb");
		assert(fout);
		//��ѹ���ļ���ԭԭ�ļ�
		string uncompressfilename = filename;
		size_t index2 = uncompressfilename.find('.');
		uncompressfilename = uncompressfilename.substr(0,index2);
		uncompressfilename += ".unhufman";
		FILE* foutuncompress = fopen(uncompressfilename.c_str(),"wb");
		assert(foutuncompress);

		int pos = 7;
		unsigned char c = fgetc(fout);
		HuffmanTreeNode<CharInfo>* root = h1.GetRoot();
		HuffmanTreeNode<CharInfo>* cur = root;
		//while (charCount && ch != EOF)
		while (charCount && !feof(fout))
		{
			//��Ҷ�ӽڵ�˵���ҵ��ַ��������ѹ�ļ�
			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,foutuncompress);
				charCount--;
				cur = root;
			} 
			//�������Ҷ�ӽ�����Ҷ�ӽڵ㣬0�����ߣ�1������
			if (c & (1<<pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			pos--;
			//��pos<0ʱ��˵����ʱ�Ѿ���ѹ��һ���ֽڣ���ʼ��ѹ��һ���ֽ�
			if (pos < 0)
			{
				pos = 7;
				c = fgetc(fout);
			}
			//��charCount == 0ʱ���ַ��Ѿ�ȫ����ѹ��
			if (charCount == 0)
				break;
		}
		return uncompressfilename.c_str();
	}
protected:
	void GenerateHuffmanTreeCode(HuffmanTreeNode<CharInfo>* root,string& code)//�õ�����������
	{
		if(root == NULL)
			return ;


		GenerateHuffmanTreeCode(root->_left,code+'0');
		GenerateHuffmanTreeCode(root->_right,code+'1');

		if (root->_left == NULL && root->_right == NULL)//��Ҷ�ӽ�㣬˵���������
			_info[root->_weight._ch]._code = code;
		
		//�ȱ������������ٱ���������
		//if (root->_left)	//��Ϊ0
			//GenerateHuffmanTreeCode(root->_left,code+'0');
		//if (root->_right)		//��Ϊ1
			//GenerateHuffmanTreeCode(root->_right,code+'1');
	}

	void GenerateHuffmanTreeCode(HuffmanTreeNode<CharInfo>* root)
	{
		if(root == NULL)
			return ;

		//�ȱ������������ٱ���������
		GenerateHuffmanTreeCode(root->_left);
		GenerateHuffmanTreeCode(root->_right);

		if (root->_left == NULL && root->_right == NULL)//�ҵ�Ҷ�ӽڵ㣬˵���ҵ��ַ�,���ɹ���������
		{
			string code;
			HuffmanTreeNode<CharInfo>* cur = root;
			HuffmanTreeNode<CharInfo>* parent = cur->_parent;
			while (parent)
			{
				if (parent->_left == cur)	//��߾Ͳ���'0'
					//code.push_back('0');
					code += '0';
				else
					//code.push_back('1');	//�ұ߾Ͳ���'1'
					code += '1';

				cur = parent;
				parent = cur->_parent;
			}
			reverse(code.begin(),code.end());
			_info[root->_weight._ch]._code = code;

		}
	}
protected:
	CharInfo _info[256];
};

void TestCompress()
{
	FileCompress filecompress;
	int start = GetTickCount();
	//filecompress.Compress("file.txt");
	filecompress.Compress("music.mp3");
	//filecompress.Compress("picture.jpg");
	//filecompress.Compress("Input.BIG");
	int end = GetTickCount();
	cout<<"Compress():"<<end-start<<endl;

	start = GetTickCount();
	//filecompress.UnCompress("file.huffman");
	filecompress.UnCompress("music.huffman");
	//filecompress.UnCompress("picture.huffman");
	//filecompress.UnCompress("Input.huffman");
	end = GetTickCount();
	cout<<"UnCompress:"<<end-start<<endl;
}