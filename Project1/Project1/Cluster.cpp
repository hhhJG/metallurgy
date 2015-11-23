#include "Cluster.h"
#include "Solid.h"
#include "Point.h"

#include <math.h>
#include <vector>
#include <list>
#include <algorithm> 

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
		list<int> mOneClusterLt;
		for(int y = 0; y < mSolidCount; y ++)
		{
			if(MaxDisOfTwoSolid(&tSolidVecP->at(x), &tSolidVecP->at(y)) <= tR)
			{
				mOneClusterLt.push_back(tSolidVecP->at(y).GetID());
			}
		}
		gClusterResult.push_back(mOneClusterLt);
	}
}


bool SortElementCluster(list<int>& tFirst, list<int>& tSecond)
{
	return tFirst.size() > tSecond.size();
}

void Cluster::SortByLength()
{
	sort(gClusterResult.begin(), gClusterResult.end(), SortElementCluster);
}

vector<list<int>>* Cluster::GetClusterResult()
{
	return &(this->gClusterResult);
}