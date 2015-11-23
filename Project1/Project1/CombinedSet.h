/**
���ϵ����зָʽ

һ������S�ķָPartition��,���ǰ�S�ֽ����ǧ���Ӽ�S1��S2������Sp��ʹ��S1��S2�ȡ�USp=S����������������ͬ���Ӽ�Si��Sj���ԣ�Si��Sj=?��
���仰˵�����ϵķָ���ǰѸü��Ϸֽ�������໥������Ӽ����������ԣ�{1��2��3}�ķָ���� {1},{2},{3}�� {1,2},{3}�� {1,3},{2}�� {2��3}��{1}; {1��2��3}�� 5 �֡�
��дһ�����򣬶��� n���г�{1,2,3������n}�����зָʽ��

*/

#pragma once
#include <list>
#include <vector>
using namespace std;

class CombinedSet
{
private:
	vector<list<int>> gResultVecLt;
	int gElementCount;
public:
	CombinedSet(void);
	CombinedSet(int tElementCount);
	~CombinedSet(void);
	void SetPartition();
	void Display(int* tCode);
	void Display();
	void Sort();
	vector<list<int>>* GetCombinedSetVecP();
};
