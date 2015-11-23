#include "Solution.h"
#include "Cluster.h"

#include <math.h>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;


Solution::Solution()
{
}

Solution::Solution(vector<Solid>* tAllSolidVec)
{
	this->gAllSolidVec = tAllSolidVec;
}

Solution::~Solution()
{
}

string Solution::ListIntToString(list<int>* tData)
{
	string mResult = "";
	list<int>::iterator mIter = tData->begin();
	while (mIter != tData->end())
	{
		stringstream ss;
		ss << *mIter;
		string s1 = ss.str();
		mResult.append(ss.str().append(","));
		mIter++;
	}
	if (mResult.length() > 0)
	{
		mResult = mResult.substr(0, mResult.length() - 1);
	}
	return mResult;
}

void Solution::GetCenterOfSolids(vector<Solid>* tSolidsVec, Point* tResult)
{
	int mSolidCount = tSolidsVec->size();
	vector<Solid>::iterator mSolidVecIter = tSolidsVec->begin();
	Solid& mFirstSolid = *mSolidVecIter;
	Point* tXMin = mFirstSolid.GetXMin();
	Point* tXMax = mFirstSolid.GetXMax();
	Point* tYMin = mFirstSolid.GetYMin();
	Point* tYMax = mFirstSolid.GetYMax();
	Point* tZMin = mFirstSolid.GetZMin();
	Point* tZMax = mFirstSolid.GetZMax();
	mSolidVecIter++;
	while (mSolidVecIter != tSolidsVec->end())
	{
		Solid& mOneSolid = *mSolidVecIter;
		if (tXMax->GetX() < mOneSolid.GetXMax()->GetX())
		{
			tXMax = mOneSolid.GetXMax();
		}
		else if (tXMin->GetX() > mOneSolid.GetXMin()->GetX())
		{
			tXMin = mOneSolid.GetXMin();
		}

		if (tYMax->GetY() < mOneSolid.GetYMax()->GetY())
		{
			tYMax = mOneSolid.GetYMax();
		}
		else if (tYMin->GetY() > mOneSolid.GetYMin()->GetY())
		{
			tYMin = mOneSolid.GetYMin();
		}

		if (tZMax->GetZ() < mOneSolid.GetZMax()->GetZ())
		{
			tZMax = mOneSolid.GetZMax();
		}
		else if (tZMin->GetZ() > mOneSolid.GetZMin()->GetZ())
		{
			tZMin = mOneSolid.GetZMin();
		}
		mSolidVecIter++;
	}
	/**
	对于上表面补漏的情况，此处Z坐标应该设置成某一固定值
	*/
	tResult->SetPoint((tXMin->GetX() + tXMax->GetX()) / 2, (tYMin->GetX() + tYMax->GetY()) / 2, (tZMin->GetZ() + tZMax->GetZ()) / 2);
}

void Solution::Method(double tR)
{
	Cluster mClusterObj;
	mClusterObj.ClusterByR(gAllSolidVec, tR);
	mClusterObj.SortByLength();
	vector<list<int>>* mClusterResult = mClusterObj.GetClusterResult();
	vector<list<int>>::iterator mClusterIter = mClusterResult->begin();
	while (mClusterIter != mClusterResult->end())
	{
		list<int>& mOneClusterLt = *mClusterIter;
		list<int>::iterator mOneCluLtIter = mOneClusterLt.begin();
		double mMaxModulusOfClu = 0.0;
		vector<Solid> mOneGroupSolidsVec;
		list<Point*> mOneGroupTopPointLt;
		while (mOneCluLtIter != mOneClusterLt.end())
		{
			Solid& mOneSolidObj = this->gAllSolidVec->at(*mOneCluLtIter);
			if (mOneSolidObj.GetModulus() > mMaxModulusOfClu)
			{
				mMaxModulusOfClu = mOneSolidObj.GetModulus();
			}
			mOneGroupTopPointLt.push_back(mOneSolidObj.GetXMax());
			mOneGroupSolidsVec.push_back(mOneSolidObj);
			mOneCluLtIter++;
		}
		Point* mTempCenterObj = new Point();
		GetCenterOfSolids(&mOneGroupSolidsVec, mTempCenterObj);

		list<Point*>::iterator mOneGroupTopPointLtIter = mOneGroupTopPointLt.begin();
		double mx = 0.0, my = 0.0, mz = 0.0;
		while (mOneGroupTopPointLtIter != mOneGroupTopPointLt.end())
		{
			Point* mOnePointP = *mOneGroupTopPointLtIter;			
			mx = pow((mTempCenterObj->GetX() - mOnePointP->GetX()), 2);
			my = pow((mTempCenterObj->GetY() - mOnePointP->GetY()), 2);
			mz = pow((mTempCenterObj->GetZ() - mOnePointP->GetZ()), 2);

			mOneGroupTopPointLtIter++;
		}

		//根据模数，获取大于等于此模数对应的r集合
		list<int> mModulusLt;
		//遍历大于等于此模数对应的r，从小到大
		list<int>::iterator mModulusLtIter = mModulusLt.begin();
		bool mFlag = false;
		while (mModulusLtIter != mModulusLt.end())
		{
			if (sqrt(mx + my + mz) > (*mModulusLtIter + tR))
			{
				mModulusLtIter++;
			}
			else
			{
				//得到结果
				Point mTempCenPoint(mTempCenterObj->GetX(), mTempCenterObj->GetY(), mTempCenterObj->GetZ());
				Riser mRiser;
				mRiser.gCenter = mTempCenPoint;
				mRiser.gIDRemark = ListIntToString(&mOneClusterLt);
				mRiser.gRadius = *mModulusLtIter;
				mFlag = true;
				break;
			}
		}
		//将聚类划分子集，继续求解
		if (!mFlag)
		{

		}

		mClusterIter++;
	}
}

void Solution::SubMethod(list<int>* tClusteIndexLt, list<int>* tModulusLt)
{

}