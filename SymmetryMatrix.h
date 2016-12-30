#pragma once

#include <iostream>
using namespace std;

template<class T>
class SymmetryMatrix
{
public:
	SymmetryMatrix(const T* a,size_t n)
		:_matrix(new T[n*(n+1)/2])
		,_size(n*(n+1)/2)
		,_n(n)
	{
		size_t index = 0;	//��ʾһά������±�
		for (size_t i=0; i<n; ++i)
		{
			for (size_t j=0; j<n; ++j)
			{
				if (i >= j)		//�洢������
				{
					_matrix[index++] = a[i*n+j];
					//index++;
				}
				else
					continue;
			}
		}
	}

	T& Access(size_t row,size_t col)	//��������
	{
		if (row < col)
		{
			std::swap(row,col);
		}
		return _matrix[row*(row+1)/2+col];
	}

	void Display()
	{
		for (size_t i=0; i<_n; i++)
		{
			for (size_t j=0; j<_n; j++)
			{
				cout<<Access(i,j)<<" ";
			}
			cout<<endl;
		}
	}
protected:
	T* _matrix;		//ѹ���洢��һά����
	size_t _size;	//�ɴ洢�Ĵ�С
	size_t _n;		//���еĴ�С		
};


void TestSymmetryMatrix()
{
	int a[5][5] = 
	{
		{0,1,2,3,4},
		{1,0,1,2,3},
		{2,1,0,1,2},
		{3,2,1,0,1},
		{4,3,2,1,0},
	};
	SymmetryMatrix<int> sm((int *)a,5);
	sm.Display();
}