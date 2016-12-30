#pragma once

#include <iostream>
using namespace std;

struct __TrueType
{
	bool Get ()
	{
		return true ;
	}
};
struct __FalseType
{
	bool Get ()
	{
		return false ;
	}
};
template <class _Tp>
struct TypeTraits
{
	typedef __FalseType __IsPODType;
};
template <>
struct TypeTraits< bool>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< char>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< unsigned char >
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< short>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< unsigned short >
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< int>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< unsigned int >
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< long>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< unsigned long >
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< long long >
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< unsigned long long>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< float>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< double>
{
	typedef __TrueType __IsPODType;
};
template <>
struct TypeTraits< long double >
{
	typedef __TrueType __IsPODType;
};
template <class _Tp>
struct TypeTraits< _Tp*>
{
	typedef __TrueType __IsPODType;
};
//
// 使用参数推到的萃取处理
//
template <class T>
void Copy (const T* src , T* dst, size_t size, __FalseType )
{
	cout<<"__FalseType:" <<typeid( T).name ()<<endl;
	for (size_t i = 0; i < size ; ++i)
	{
		dst[i ] = src[ i];
	}
}
template <class T>
void Copy (const T* src , T* dst, size_t size, __TrueType )
{
	cout<<"__TrueType:" <<typeid( T).name ()<<endl;
	memcpy(dst , src, size*sizeof (T));
}
//
// 使用萃取判断类型的Get函数判断是否是 POD类型来处理
//
template <class T>
void Copy (const T* src , T* dst, size_t size)
{
	cout<<"__TrueType:" <<typeid( T).name ()<<endl;
	if (TypeTraits <T>:: __IsPODType().Get ())
	{
		memcpy(dst , src, size*sizeof (T));
	}
	else
	{
		for (size_t i = 0; i < size ; ++i)
		{
			dst[i ] = src[ i];
		}
	}
}