#include "Cluster.h"
#include "Solid.h"
#include "Point.h"

#include <math.h>
#include <vector>
#include <list>
#include <algorithm> 
#include <iostream>
using namespace std;


Cluster::Cluster(void)
{
}

Cluster::~Cluster(void)
{
}

double Cluster::MaxDisOfTwoSolid(Solid* tFirst, Solid* tSecond)
{
	Point *mEightPointFir = new Point[8];
	tFirst->SetEightPointOfCuboid(mEightPointFir);
	Point *mEightPointSec = new Point[8];
	tSecond->SetEightPointOfCuboid(mEightPointSec);
	double mResult = 0.0, tempRes = 0.0;
	double mx = 0.0, my = 0.0, mz = 0.0;
	for(int x = 0; x < 8; x ++)
	{
		for(int y = 0; y < 8; y ++)
		{
			mx = pow(((mEightPointFir + x)->GetX() - (mEightPointSec + y)->GetX()), 2);
			my = pow(((mEightPointFir + x)->GetY() - (mEightPointSec + y)->GetY()), 2);
			mz = pow(((mEightPointFir + x)->GetZ() - (mEightPointSec + y)->GetZ()), 2);
			tempRes = sqrt(mx + my + mz);
		}
		if(tempRes > mResult)
		{
			mResult = tempRes;
		}
	}
	return mResult;
}

void Cluster::ClusterByR(vector<Solid>* tSolidVecP, double tR)
{
	int mSolidCount = tSolidVecP->size();
	for(int x = 0; x < mSolidCount; x ++)
	{
		vector<int> mOneClusterVec;
		//邻居漏洞
		for(int y = 0; y < mSolidCount; y ++)
		{
			if(MaxDisOfTwoSolid(&tSolidVecP->at(x), &tSolidVecP->at(y)) <= tR && x != y)
			{
				mOneClusterVec.push_back(tSolidVecP->at(y).GetID());
			}
		}
		//自身漏洞
		mOneClusterVec.push_back(tSolidVecP->at(x).GetID());

		gClusterResult.push_back(mOneClusterVec);
	}
}


bool SortElementCluster(vector<int>& tFirst, vector<int>& tSecond)
{
	return tFirst.size() > tSecond.size();
}

/**
根据聚类之后，每个集合的大小进行排序
集合中元素多的排在前面，集合中元素少的排在后面
即从大到小进行排序
说明：主要是为了用尽可能少的补缩工具补尽可能多的漏洞
*/
void Cluster::SortByLength()
{
	sort(gClusterResult.begin(), gClusterResult.end(), SortElementCluster);
}

vector<vector<int>>* Cluster::GetClusterResult()
{
	return &(this->gClusterResult);
}

void Cluster::DisplayClusterResult()
{
	cout << "******************聚类结果begin**************************" << endl;
	vector<vector<int>>::iterator mVecVecIter = this->gClusterResult.begin();
	while (mVecVecIter != this->gClusterResult.end())
	{
		vector<int> mOneClusterVec = *mVecVecIter;
		vector<int>::iterator  mOneClusterVecIter = mOneClusterVec.begin();
		while (mOneClusterVecIter != mOneClusterVec.end())
		{
			cout << *mOneClusterVecIter << "\t";
			mOneClusterVecIter++;
		}
		cout << endl;
		mVecVecIter++;
	}
	cout << "******************聚类结果end**************************" << endl;
}