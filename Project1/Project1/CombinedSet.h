/**
集合的所有分割方式

一个集合S的分割（Partition）,就是把S分解成若千个子集S1，S2，…，Sp，使得S1∪S2∪…USp=S，但对任意两个不同的子集Si与Sj而言，Si∩Sj=?。
换句话说，集合的分割，就是把该集合分解成若干相互分离的子集。举例而言，{1，2，3}的分割就有 {1},{2},{3}； {1,2},{3}； {1,3},{2}； {2，3}，{1}; {1，2，3}这 5 种。
编写一个程序，读入 n，列出{1,2,3，…，n}的所有分割方式。

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
