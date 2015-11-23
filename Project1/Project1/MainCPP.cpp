#include "DataOperation.h"
#include "SixPoint.h"
#include "CombinedSet.h"

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <math.h>
#include <map>
#include <algorithm> 
#include <sstream>
using namespace std;

bool SortByLength(list<int>& tFirst, list<int>& tSecond)
{
	return tFirst.size() > tSecond.size();
}



int main()
{

	/*char* PATH = "quexian.txt";
	DataOperation mDataObj;
	mDataObj.LoadFile(PATH);*/

	/*vector<list<int>> gClusterResult;
	list<int> mOneClusterLt;
	mOneClusterLt.push_back(1);
	gClusterResult.push_back(mOneClusterLt);

	list<int> mOneClusterLt2;
	mOneClusterLt2.push_back(21);
	mOneClusterLt2.push_back(22);
	gClusterResult.push_back(mOneClusterLt2);

	list<int> mOneClusterLt3;
	mOneClusterLt3.push_back(31);
	mOneClusterLt3.push_back(32);
	mOneClusterLt3.push_back(33);
	gClusterResult.push_back(mOneClusterLt3);

	vector<list<int>>::iterator mVecIter = gClusterResult.begin();
	while(mVecIter != gClusterResult.end())
	{
		list<int>::iterator mLtIter = mVecIter->begin();
		while(mLtIter != mVecIter->end())
		{
			cout<<*mLtIter<<"\t";
			mLtIter ++;
		}
		cout<<endl;
		mVecIter ++;
	}

	cout<<endl;
	cout<<endl;

	sort(gClusterResult.begin(), gClusterResult.end(), SortByLength);
	mVecIter = gClusterResult.begin();
	while(mVecIter != gClusterResult.end())
	{
		list<int>::iterator mLtIter = mVecIter->begin();
		while(mLtIter != mVecIter->end())
		{
			cout<<*mLtIter<<"\t";
			mLtIter ++;
		}
		cout<<endl;
		mVecIter ++;
	}*/
	
	/*CombinedSet mCombinedSetObj(4);
	mCombinedSetObj.SetPartition();
	mCombinedSetObj.Display();
	cout << endl;
	cout << endl;
	mCombinedSetObj.Sort();
	mCombinedSetObj.Display();*/

	system("pause");
	return 0;
}