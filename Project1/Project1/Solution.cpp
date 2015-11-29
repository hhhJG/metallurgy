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
	�����ϱ��油©��������˴�Z����Ӧ�����ó�ĳһ�̶�ֵ
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
	double mx = 0.0, my = 0.0, mz = 0.0, mMaxDistance = 0.0;//busuo���ľ������и÷�������������Զ�ĳ���
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

	//����ģ������ȡ���ڵ��ڴ�ģ����Ӧ��r����
	list<double>* mModulusLt;
	MySqlDeal mMySqlDeal;
	mMySqlDeal.GetDataBy(mMaxModulusOfClu, 11);
	mModulusLt = mMySqlDeal.GetResultP();

	//�������ڵ��ڴ�ģ����Ӧ��r����С����
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
			/*���Ѿ��޸�©���ı�ŷ��롾���޸�©�����Map��begin*/
			list<int>::iterator mSameGroupSolidIDIter = tSameGroupSolidIDLtP->begin();
			while (mSameGroupSolidIDIter != tSameGroupSolidIDLtP->end())
			{
				gHaveDoneSolidNumber.insert(pair<int, int>(*mSameGroupSolidIDIter - 1, *mSameGroupSolidIDIter - 1));
				mSameGroupSolidIDIter++;
			}
			//end

			//�õ����
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
	//˵��һ��©��������ð�ڲ�������
	if(tSolidsVec->size() == 1 && !mFlag)
	{
		/*���Ѿ��޸�©���ı�ŷ��롾���޸�©�����Map��begin*/
		list<int>::iterator mSameGroupSolidIDIter = tSameGroupSolidIDLtP->begin();
		while (mSameGroupSolidIDIter != tSameGroupSolidIDLtP->end())
		{
			gHaveDoneSolidNumber.insert(pair<int, int>(*mSameGroupSolidIDIter - 1, *mSameGroupSolidIDIter - 1));
			mSameGroupSolidIDIter++;
		}
		//end

		//������
		Point mTempCenPoint;
		Riser mRiser;
		mRiser.gCenter = mTempCenPoint;
		mRiser.gIDRemark = "�޷��©��";
		mRiser.gRadius = 0.0;
		gAnswers.push_back(mRiser);
	}
}


void Solution::Method(double tR)
{
	Cluster mClusterObj;
	mClusterObj.ClusterByR(gAllSolidVec, tR);
	cout << "����֮ǰ" << endl;
	mClusterObj.DisplayClusterResult();

	mClusterObj.SortByLength();
	cout << "����֮��" << endl;
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
	���ݾ�����Ԫ�صĸ������γɻ����Ӽ���
	��tClusteIndexVec�а���3��Ԫ�أ���õ��Ľ���ǡ�{1,1,1}����{1,1,2}����{1,2,1}����{1,2,3}��
	���Բ��� http://dsqiu.iteye.com/blog/1715253��ҳ�е� ����3.7���ϵ����зָʽ��SETPART.C ��
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

		//���ݼ��ϵ����зָʽ��ÿ���Ӽ��õ���Ӧ��Solid����begin
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
	cout<<"���ĵ�\t©�����ϱ�ʶ����\t�뾶"<<endl;
	while(mRiserIter != gAnswers.end())
	{
		Riser mRiser = *mRiserIter;
		cout<<"("<<mRiser.gCenter.GetX()<<", "<<mRiser.gCenter.GetY()<<", "<<mRiser.gCenter.GetZ()<<")\t"<<mRiser.gIDRemark<<"\t"<<mRiser.gRadius<<endl;
		mRiserIter++;
	}
	cout<<"*************************************************"<<endl;
}