#pragma once

#include<iostream>
using namespace std;
#include <vector>

template<class T>
struct Triple
{
	Triple(size_t row,size_t col,const T& value = T())
		:_row(row)
		,_col(col)
		,_value(value)
	{}
	Triple()
	{}

	size_t _row;	//行
	size_t _col;	//列
	T _value;		//有效值
};

template<class T>
class SpareMatrix
{
public:
	SpareMatrix()
		:_martix(NULL)
		,_rowSize(0)
		,_colSize(0)
		,_invalid(T())
	{}
	//数据的存储
	SpareMatrix(const T* a,size_t row,size_t col,const T& invalid)
		:_rowSize(row)
		,_colSize(col)
		,_invalid(invalid)
	{
		for(size_t i=0; i<row; i++)
		{
			for (size_t j=0; j<col; j++)
			{
				if (a[i*col+j] != invalid)	//有效值
				{
					Triple<T> t(i,j,a[i*col+j]);
					t._value = a[i*col+j];
					t._row = i;
					t._col = j;
					_martix.push_back(t);
				}
			}
		}
	}
	//输出
	void Display()
	{
		size_t index = 0;
		for (size_t i=0; i<_rowSize; ++i)
		{
			for (size_t j=0; j<_colSize; ++j)
			{
				if (index < _martix.size()
					&& i == _martix[index]._row
					&& j == _martix[index]._col)
				{
					cout<<_martix[index]._value<<" ";
					++index;
				}
				else
				{
					cout<<0<<" ";
				}
			}
			cout<<endl;
		}
		cout<<endl;
	}

	//SpareMatrix<T> Transport()	//矩阵的转置
	//{
	//	SpareMatrix<T> TransMartix;
	//	TransMartix._rowSize = _colSize;//交换行列
	//	TransMartix._colSize = _rowSize;
	//	
	//	Triple<T> tmp;
	//	size_t index=0;
	//	for (size_t col=0; col<_colSize; ++col)	//以原矩阵的列优先查找非零元
	//	{
	//		for (size_t index=0; index<_martix.size(); ++index)
	//		{
	//			if (_martix[index]._col == col)
	//			{
	//				tmp._row = _martix[index]._col;
	//				tmp._col = _martix[index]._row;
	//				tmp._value = _martix[index]._value;
	//				TransMartix._martix.push_back(tmp);
	//			}
	//		}
	//	}
	//	return TransMartix;
	//}
	SpareMatrix<T> Transport()	//矩阵的转置
	{
		SpareMatrix<T> TSMartix;
		TSMartix._rowSize = _colSize;//交换行列
		TSMartix._colSize = _rowSize;
		TSMartix._invalid = _invalid;
		TSMartix._martix.reserve(_martix.size());

		for (size_t col=0; col<_colSize; ++col)	//以列优先进行查找
		{
			for (size_t index=0; index<_martix.size(); ++index)
			{
				if (_martix[index]._col == col)		
				{
					Triple<T> tmp(_martix[index]._col,_martix[index]._row,_martix[index]._value);
					TSMartix._martix.push_back(tmp);
				}
			}
		}
		return TSMartix;
	}

	SpareMatrix<T> FastTransport()
	{
		SpareMatrix<T> FSTMartix;
		FSTMartix._rowSize = _colSize;//交换行列
		FSTMartix._colSize = _rowSize;
		FSTMartix._invalid = _invalid;
		FSTMartix._martix.resize(_martix.size());

		int* count = new int[_colSize];
		memset(count,0,_martix.size()*sizeof(int));
		int* start = new int[_colSize];
		memset(start,0,_martix.size()*sizeof(int));

		//统计每列的有效值个数
		for (size_t i=0; i<_martix.size(); ++i)
		{
			count[_martix[i]._col]++;
		}

		//计算每一列的起始位置
		for (size_t j=1; j<_colSize; ++j)
		{
			start[j] = start[j-1] + count[j-1];//每一列起始位置=上一列起始位置+上一列有效数个数
		}

		//转置
		for (size_t index=0; index<_martix.size(); ++index)
		{
			size_t tmp = _martix[index]._col;	//取到原三元组中每个数的列
			FSTMartix._martix[start[tmp]]._col = _martix[index]._row;
			FSTMartix._martix[start[tmp]]._row = _martix[index]._col;
			FSTMartix._martix[start[tmp]]._value = _martix[index]._value;
			++start[tmp];	//start指针要向后移位
		}
		return FSTMartix;
	}
protected:
	vector<Triple<T>> _martix;
	size_t _rowSize;
	size_t _colSize;
	T _invalid;	//非法值
};


void TestSpareMatrix()
{
	int a[4][5] = 
	{
		{1,0,2,0,4},
		{0,0,0,0,0},
		{3,0,0,5,0},
		{4,0,0,0,0}
	};

	SpareMatrix<int> sm((int*)a,4,5,0);
	sm.Display();
	SpareMatrix<int> tsm = sm.Transport();
	tsm.Display();
	SpareMatrix<int> ftsm = sm.FastTransport();
	ftsm.Display();
}