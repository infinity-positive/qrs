/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：sequence_list.cpp
*   创 建 者：YQQ
*   创建日期：2021年05月26日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <iostream>
using namespace std;

class SeqList{
	private:
		int size;
		int *arr;
		int tail;

	public:
		//声明构造函数
		SeqList(int size, int tail = 0)
		{
			size = size;
			tail = tail;
		}
		//声明析构函数
		~SeqList()
		{
			delete []arr;
		}

	public:
		void init(int size);
		void insert(int data);//增
		void show();//显示顺序表
		void del(int pos);//删
		void modf(int pos, int data);//改
		int & at(int pos);//查找该位置的元素
};

void SeqList::init(int size)
{
	arr = new int[size];
}

void SeqList::insert(int data)
{
	arr[tail++] = data;
}

void SeqList::show()
{
	for(int i = 0; i < tail; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

void SeqList::del(int pos)
{
	for(int i = pos; i < tail; i++)
	{
		arr[i] = arr[i+1];
	}
	tail--;
}

void SeqList::modf(int pos, int data)
{
	arr[pos] = data;
}

int& SeqList::at(int pos)
{
	return arr[pos];
}

int main(int argc, char * argv[])
{
	SeqList a(10);
	a.init(10);

	cout << "生成顺序表" << endl;
	for(int i = 2; i < 12; i++)
	{
		a.insert(i);
	}
	a.show();

	cout << "删除顺序表中第3个元素"<< endl;
	a.del(3);
	a.show();
	
	cout << "将顺序表中第4个元素修改成999" << endl;
	a.modf(3, 999);
	a.show();

	cout << "获取顺序表中第4个元素" << endl;
	cout << a.at(3) << endl;
	
	return 0;
}
