#include "Solution.h"
#include "Cluster.h"
#include "CombinedSet.h"
#include "MySqlDeal.h"

#include <math.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
using namespace std;


Solution::Solution()
{
	gHaveDoneSolidNumber.clear();
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

string Solution::VectorIntToString(vector<int>* tData)
{
	string mResult = "";
	vector<int>::iterator mIter = tData->begin();
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

void Solution::OneGroupSolidAnswer(vector<Solid>* tSolidsVec, double tR, list<int>* tSameGroupSolidIDLtP)
{
	Point* mTempCenterObj = new Point();
	GetCenterOfSolids(tSolidsVec, mTempCenterObj);

	list<Point*> mOneGroupTopPointLt;

	double mMaxModulusOfClu = 0.0;
	vector<Solid>::iterator mOneSolidVecIter = tSolidsVec->begin();
	while (mOneSolidVecIter != tSolidsVec->end())
	{
		Solid& mOneSolidObj = *mOneSolidVecIter;
		if (mOneSolidObj.GetModulus() > mMaxModulusOfClu)
		{
			mMaxModulusOfClu = mOneSolidObj.GetModulus();
		}
		Point* mEightPointP = new Point[8];
		for(int x = 0; x < 8; x ++)
		{
			mOneGroupTopPointLt.push_back(mEightPointP + x);
		}		
		mOneSolidVecIter++;
	}

	list<Point*>::iterator mOneGroupTopPointLtIter = mOneGroupTopPointLt.begin();
	double mx = 0.0, my = 0.0, mz = 0.0, mMaxDistance = 0.0;//busuo中心距离所有该分组立方体中最远的长度
	while (mOneGroupTopPointLtIter != mOneGroupTopPointLt.end())
	{
		Point* mOnePointP = *mOneGroupTopPointLtIter;			
		mx = pow((mTempCenterObj->GetX() - mOnePointP->GetX()), 2);
		my = pow((mTempCenterObj->GetY() - mOnePointP->GetY()), 2);
		mz = pow((mTempCenterObj->GetZ() - mOnePointP->GetZ()), 2);
		if(mMaxDistance < sqrt(mx + my + mz))
		{
			mMaxDistance = sqrt(mx + my + mz);
		}
		mOneGroupTopPointLtIter++;
	}

	//根据模数，获取大于等于此模数对应的r集合
	list<double>* mModulusLt;
	MySqlDeal mMySqlDeal;
	mMySqlDeal.GetDataBy(mMaxModulusOfClu, 11);
	mModulusLt = mMySqlDeal.GetResultP();

	//遍历大于等于此模数对应的r，从小到大
	list<double>::iterator mModulusLtIter = mModulusLt->begin();
	bool mFlag = false;
	while (mModulusLtIter != mModulusLt->end())
	{
		if (mMaxDistance > (*mModulusLtIter + tR))
		{
			mModulusLtIter++;
		}
		else
		{
			/*将已经修复漏洞的编号放入【已修复漏洞编号Map】begin*/
			list<int>::iterator mSameGroupSolidIDIter = tSameGroupSolidIDLtP->begin();
			while (mSameGroupSolidIDIter != tSameGroupSolidIDLtP->end())
			{
				gHaveDoneSolidNumber.insert(pair<int, int>(*mSameGroupSolidIDIter - 1, *mSameGroupSolidIDIter - 1));
				mSameGroupSolidIDIter++;
			}
			//end

			//得到结果
			Point mTempCenPoint(mTempCenterObj->GetX(), mTempCenterObj->GetY(), mTempCenterObj->GetZ());
			Riser mRiser;
			mRiser.gCenter = mTempCenPoint;
			mRiser.gIDRemark = ListIntToString(tSameGroupSolidIDLtP);
			mRiser.gRadius = *mModulusLtIter;
			gAnswers.push_back(mRiser);
			mFlag = true;
			break;
		}
	}
	//说明一个漏洞用最大的冒口补缩不到
	if(tSolidsVec->size() == 1 && !mFlag)
	{
		/*将已经修复漏洞的编号放入【已修复漏洞编号Map】begin*/
		list<int>::iterator mSameGroupSolidIDIter = tSameGroupSolidIDLtP->begin();
		while (mSameGroupSolidIDIter != tSameGroupSolidIDLtP->end())
		{
			gHaveDoneSolidNumber.insert(pair<int, int>(*mSameGroupSolidIDIter - 1, *mSameGroupSolidIDIter - 1));
			mSameGroupSolidIDIter++;
		}
		//end

		//输出结果
		Point mTempCenPoint;
		Riser mRiser;
		mRiser.gCenter = mTempCenPoint;
		mRiser.gIDRemark = "无法填补漏洞";
		mRiser.gRadius = 0.0;
		gAnswers.push_back(mRiser);
	}
}


void Solution::Method(double tR)
{
	Cluster mClusterObj;
	mClusterObj.ClusterByR(gAllSolidVec, tR);
	cout << "排序之前" << endl;
	mClusterObj.DisplayClusterResult();

	mClusterObj.SortByLength();
	cout << "排序之后" << endl;
	mClusterObj.DisplayClusterResult();

	vector<vector<int>>* mClusterResult = mClusterObj.GetClusterResult();
	vector<vector<int>>::iterator mClusterIter = mClusterResult->begin();
	
	while (mClusterIter != mClusterResult->end())
	{
		vector<int>& mOneClusterLt = *mClusterIter;
		vector<int>::iterator mOneClusterLtIter = mOneClusterLt.begin();
		while (mOneClusterLtIter != mOneClusterLt.end())
		{
			cout << *mOneClusterLtIter << endl;
			if (gHaveDoneSolidNumber.find(*mOneClusterLtIter) != gHaveDoneSolidNumber.end())
			{
				mOneClusterLtIter = mOneClusterLt.erase(mOneClusterLtIter);
			}
			else
			{
				mOneClusterLtIter++;
			}			
		}
		SubMethod(&mOneClusterLt, tR);
		mClusterIter++;
	}
}

void Solution::SubMethod(vector<int>* tClusteIndexVec, double tR)
{
	int mSolidCount = tClusteIndexVec->size();
	/**
	根据聚类中元素的个数，形成互补子集合
	如tClusteIndexVec中包括3个元素，则得到的结果是【{1,1,1}】【{1,1,2}】【{1,2,1}】【{1,2,3}】
	可以参照 http://dsqiu.iteye.com/blog/1715253网页中的 问题3.7集合的所有分割方式（SETPART.C ）
	*/
	CombinedSet mCombinedSetObj(mSolidCount);
	mCombinedSetObj.SetPartition();
	mCombinedSetObj.Sort();

	vector<list<int>>* mCombinedSetVec = mCombinedSetObj.GetCombinedSetVecP();
	vector<list<int>>::iterator mCombinedSetVecIter = mCombinedSetVec->begin();
	while(mCombinedSetVecIter != mCombinedSetVec->end())
	{
		list<int> mOneSubComSetLt = *mCombinedSetVecIter;
		list<int>::iterator mOneSubComSetLtIter = mOneSubComSetLt.begin();
		vector<vector<Solid>> mGroupSetVecVec;
		int mGroupIndex = 0, mIndex = 0;

		//根据集合的所有分割方式中每个子集得到对应的Solid集合begin
		int mGrouMax = 0;
		while (mOneSubComSetLtIter != mOneSubComSetLt.end())
		{
			mGroupIndex = *mOneSubComSetLtIter;
			if (mGrouMax < mGroupIndex)
			{
				mGrouMax = mGroupIndex;
				vector<Solid> mNullSolidVec;
				mGroupSetVecVec.push_back(mNullSolidVec);
			}
			mOneSubComSetLtIter++;
		}

		mOneSubComSetLtIter = mOneSubComSetLt.begin();
		while (mOneSubComSetLtIter != mOneSubComSetLt.end())
		{
			mGroupIndex = *mOneSubComSetLtIter - 1;
			mGroupSetVecVec.at(mGroupIndex).push_back(gAllSolidVec->at(tClusteIndexVec->at(mIndex)));
			mOneSubComSetLtIter++;
			mIndex ++;
		}
		//end

		vector<vector<Solid>>::iterator mGroupSetVecVecIter = mGroupSetVecVec.begin();
		while(mGroupSetVecVecIter != mGroupSetVecVec.end())
		{
			vector<Solid>& mSameGroupSolidLt = *mGroupSetVecVecIter;
			OneGroupSolidAnswer(&mSameGroupSolidLt, tR, &mOneSubComSetLt);
			mGroupSetVecVecIter++;
		}
		mCombinedSetVecIter++;
	}
}

void Solution::DisplaySolution()
{
	vector<Riser>::iterator mRiserIter = gAnswers.begin();
	cout<<"中心点\t漏洞集合标识集合\t半径"<<endl;
	while(mRiserIter != gAnswers.end())
	{
		Riser mRiser = *mRiserIter;
		cout<<"("<<mRiser.gCenter.GetX()<<", "<<mRiser.gCenter.GetY()<<", "<<mRiser.gCenter.GetZ()<<")\t"<<mRiser.gIDRemark<<"\t"<<mRiser.gRadius<<endl;
		mRiserIter++;
	}
	cout<<"*************************************************"<<endl;
}