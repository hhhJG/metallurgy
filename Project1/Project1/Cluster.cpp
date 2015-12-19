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

void Cluster::SetMaxMrFromData(double tMaxMr)
{
	this->gMaxMrFromData = tMaxMr;
}

double Cluster::MaxDisOfTwoSolid(Solid* tFirst, Solid* tSecond)
{
	Point *mEightPointFir = new Point[8];
	tFirst->SetEightPointOfCuboid(mEightPointFir);
	Point *mEightPointSec = new Point[8];
	tSecond->SetEightPointOfCuboid(mEightPointSec);
	double mResult = 0.0, tempRes = 0.0;
	double mx = 0.0, my = 0.0, mz = 0.0;

	double mSelfCenterX = (tFirst->GetXMax()->GetX() + tFirst->GetXMin()->GetX()) / 2;
	double mSelfCenterY = (tFirst->GetYMax()->GetY() + tFirst->GetYMin()->GetY()) / 2;
	double mSelfCenterZ = (tFirst->GetZMax()->GetZ() + tFirst->GetZMin()->GetZ()) / 2;
	for (int y = 0; y < 8; y++)
	{
		mx = pow((mSelfCenterX - (mEightPointSec + y)->GetX()), 2);
		my = pow((mSelfCenterY - (mEightPointSec + y)->GetY()), 2);
		mz = pow((mSelfCenterZ - (mEightPointSec + y)->GetZ()), 2);
		tempRes = sqrt(mx + my + mz);
		if (tempRes > mResult)
		{
			mResult = tempRes;
		}
	}
	/*for(int x = 0; x < 8; x ++)
	{
		for(int y = 0; y < 8; y ++)
		{
			mx = pow(((mEightPointFir + x)->GetX() - (mEightPointSec + y)->GetX()), 2);
			my = pow(((mEightPointFir + x)->GetY() - (mEightPointSec + y)->GetY()), 2);
			mz = pow(((mEightPointFir + x)->GetZ() - (mEightPointSec + y)->GetZ()), 2);
			tempRes = sqrt(mx + my + mz);
			if (tempRes > mResult)
			{
				mResult = tempRes;
			}
		}
	}*/
	return mResult;
}

void Cluster::ClusterByR(vector<Solid>* tSolidVecP)
{
	int mSolidCount = tSolidVecP->size();
	for(int x = 0; x < mSolidCount; x ++)
	{
		vector<int> mOneClusterVec;
		//����©��
		mOneClusterVec.push_back(tSolidVecP->at(x).GetID());
		
		//����뾶Ϊcm�������굥λmm
		double mR = tSolidVecP->at(x).GetJuLeiRadius() * 10 + gMaxMrFromData * 10;
		
		//�ھ�©��
		for(int y = 0; y < mSolidCount; y ++)
		{
			if(MaxDisOfTwoSolid(&tSolidVecP->at(x), &tSolidVecP->at(y)) <= mR && tSolidVecP->at(x).GetModulus() > tSolidVecP->at(y).GetModulus() && x != y)
			{
				mOneClusterVec.push_back(tSolidVecP->at(y).GetID());
			}
		}
		
		gClusterResult.push_back(mOneClusterVec);
	}
}


bool SortElementCluster(vector<int>& tFirst, vector<int>& tSecond)
{
	return tFirst.size() > tSecond.size();
}

/**
���ݾ���֮��ÿ�����ϵĴ�С��������
������Ԫ�ض������ǰ�棬������Ԫ���ٵ����ں���
���Ӵ�С��������
˵������Ҫ��Ϊ���þ������ٵĲ������߲������ܶ��©��
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
	cout << "������begin" << endl;
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
	cout << "������end" << endl;
}