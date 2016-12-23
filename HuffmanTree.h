#define  _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
using namespace std;
#include <string>
#include "Heap.h"


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode()
	{}
	HuffmanTreeNode(const T& weight)
		:_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_weight(weight)
	{}

	HuffmanTreeNode<T>* _left;			//����
	HuffmanTreeNode<T>* _right;		//�Һ���
	HuffmanTreeNode<T>* _parent;		//���׽��
	T _weight;		//Ȩֵ
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
	{}
	HuffmanTree(T* a,size_t size,const T& invalid)
	{
		assert(a);
		struct NodeLess
		{
			bool operator()(Node* left,Node* right)const
			{
				return (left->_weight < right->_weight);
			}
		};

		Heap<Node* ,NodeLess> minheap;

		for (size_t i=0; i<size; ++i)	//��С�ѣ�����С��ֵ���ڶѶ�
		{
			if (a[i] != invalid)
			{
				Node* node = new Node(a[i]);
				minheap.Push(node);
			}
		}

		//������������
		while (minheap.Size() > 1)
		{
			Node* left = minheap.Top();		//�ֱ�ȡ��С������ֵ��left��right
			minheap.Pop();
			Node* right = minheap.Top();
			minheap.Pop();

			Node* parent = new Node(left->_weight + right->_weight);	//�������ڵ�

			parent->_left = left;	//ʹleft��rightָ�򸸽ڵ㣬���ڵ�����ҷֱ�ָ��left��right
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;

			minheap.Push(parent);
		}
		_root = minheap.Top();
		minheap.Pop();
	}

	~HuffmanTree()
	{
		_Destory(_root);
	}

	Node* GetRoot()
	{
		return _root;
	}
protected:

	void _Destory(Node* root)
	{
		if (root == NULL)
			return ;

		Node* del = root;
		if (del)
		{
			_Destory(root->_left);
			_Destory(root->_right);
			delete del;
			del = NULL;
		}
	}
protected:
	Node* _root;
};

void TestHuffmanTree()
{
	int a[] = {1,2,3,4,5};
	size_t size = sizeof(a)/sizeof(a[0]);
	HuffmanTree<int> ht1(a,size,0);
}