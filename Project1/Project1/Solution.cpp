#include "Solution.h"
#include "Cluster.h"
#include "CombinedSet.h"
#include "MySqlDeal.h"

#include <math.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
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

void Solution::SetTopSidePoint(Point* tTopPoint)
{
	gTopSideP.SetPoint(tTopPoint->GetX(), tTopPoint->GetY(), tTopPoint->GetZ());
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
		ss.str("");
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
		ss.str("");
		mIter++;
	}
	if (mResult.length() > 0)
	{
		mResult = mResult.substr(0, mResult.length() - 1);
	}
	return mResult;
}

string Solution::VectorSolidIDToString(vector<Solid>* tSolidsVec)
{
	string mResult = "";
	vector<Solid>::iterator mIter = tSolidsVec->begin();
	while (mIter != tSolidsVec->end())
	{
		stringstream ss;
		ss << (*mIter).GetID();
		string s1 = ss.str();
		mResult.append(ss.str().append(","));
		ss.str("");
		mIter++;
	}
	if (mResult.length() > 0)
	{
		mResult = mResult.substr(0, mResult.length() - 1);
	}
	return mResult;
}

/**
��ȡһ��©��������
*/
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
	tResult->SetPoint((tXMin->GetX() + tXMax->GetX()) / 2, (tYMin->GetY() + tYMax->GetY()) / 2, gTopSideP.GetZ());
}


void Solution::SetSerchCoindition(int tSearchFileIndex, bool tSerchMulFile)
{
	this->gSearchFileIndex = tSearchFileIndex;
	this->gSearchMulFile = tSerchMulFile;
}

/**

*/
bool Solution::OneGroupSolidAnswer(vector<Solid>* tSolidsVec, vector<Riser> *mOnePossAnswer, map<int, int>* tHaveDoneSolidNumber)
{
	Point* mTempCenterObj = new Point();
	GetCenterOfSolids(tSolidsVec, mTempCenterObj);

	list<Point*> mOneGroupTopPointLt;

	double mMaxModulusOfClu = 0.0;
	double mMaxR = 0.0;
	vector<Solid>::iterator mOneSolidVecIter = tSolidsVec->begin();
	while (mOneSolidVecIter != tSolidsVec->end())
	{
		Solid& mOneSolidObj = *mOneSolidVecIter;
		if (mOneSolidObj.GetModulus() > mMaxModulusOfClu)
		{
			mMaxModulusOfClu = mOneSolidObj.GetModulus();
		}
		if (mOneSolidObj.GetJuLeiRadius() > mMaxR)
		{
			mMaxR = mOneSolidObj.GetJuLeiRadius();
		}
		Point* mEightPointP = new Point[8];
		mOneSolidObj.SetEightPointOfCuboid(mEightPointP);
		for (int x = 0; x < 8; x++)
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
		if (mMaxDistance < sqrt(mx + my + mz))
		{
			mMaxDistance = sqrt(mx + my + mz);
		}
		mOneGroupTopPointLtIter++;
	}

	//����ģ������ȡ���ڵ��ڴ�ģ����Ӧ��r����
	list<double>* mModulusLt;
	MySqlDeal mMySqlDeal;
	mMySqlDeal.GetDataBy(mMaxModulusOfClu, this->gSearchFileIndex, this->gSearchMulFile);
	mModulusLt = mMySqlDeal.GetResultP();

	//�������ڵ��ڴ�ģ����Ӧ��r����С����
	list<double>::iterator mModulusLtIter = mModulusLt->begin();
	bool mFlag = false;
	while (mModulusLtIter != mModulusLt->end())
	{
		//���ݿ��д洢��ģ����λ��cm�������굥λ��mm
		if (mMaxDistance > ((*mModulusLtIter) * 10 + mMaxR * 10))
		{
			mModulusLtIter++;
		}
		else
		{
			/*���Ѿ��޸�©���ı�ŷ��롾���޸�©�����Map��begin*/
			vector<Solid>::iterator mSameGroupSolidIDIter = tSolidsVec->begin();
			while (mSameGroupSolidIDIter != tSolidsVec->end())
			{
				int mID = (*mSameGroupSolidIDIter).GetID();
				tHaveDoneSolidNumber->insert(pair<int, int>(mID, mID));
				mSameGroupSolidIDIter++;
			}
			//end

			//�õ����
			Point mTempCenPoint(mTempCenterObj->GetX(), mTempCenterObj->GetY(), mTempCenterObj->GetZ());
			Riser mRiser;
			mRiser.gCenter = mTempCenPoint;
			mRiser.gIDRemark = VectorSolidIDToString(tSolidsVec);
			mRiser.gRadius = (*mModulusLtIter) * 10;
			mOnePossAnswer->push_back(mRiser);
			mFlag = true;
			return true;
		}
	}
	//˵��һ��©��������ð�ڲ�������
	if (tSolidsVec->size() == 1 && mFlag == false)
	{
		/*���Ѿ��޸�©���ı�ŷ��롾���޸�©�����Map��begin*/
		vector<Solid>::iterator mSameGroupSolidIDIter = tSolidsVec->begin();
		while (mSameGroupSolidIDIter != tSolidsVec->end())
		{
			Solid& mRecordSolid = *mSameGroupSolidIDIter;
			tHaveDoneSolidNumber->insert(pair<int, int>(mRecordSolid.GetID(), mRecordSolid.GetID()));
			mSameGroupSolidIDIter++;
		}
		//end

		//������
		Point mTempCenPoint(mTempCenterObj->GetX(), mTempCenterObj->GetY(), mTempCenterObj->GetZ());
		Riser mRiser;
		mRiser.gCenter = mTempCenPoint;
		mRiser.gIDRemark = VectorSolidIDToString(tSolidsVec);//"�޷��©��";
		mRiser.gRadius = 0.0;
		mOnePossAnswer->push_back(mRiser);
		return true;
	}
	else
	{
		return false;
	}
}

void Solution::Gnome_sort(int length, list<vector<int>>* result)
{
	vector<int> mSourceData;
	for (int x = 0; x < length; x++)
	{
		mSourceData.push_back(x);
	}
	result->push_back(mSourceData);

	int i = 0;
	while (i < length)
	{
		if (i == 0 || mSourceData.at(i - 1) > mSourceData.at(i))
		{
			i++;
		}
		else
		{
			int tmp = mSourceData[i];
			mSourceData[i] = mSourceData[i - 1];
			mSourceData[i - 1] = tmp;
			i--;
			result->push_back(mSourceData);
		}
	}

}

void Solution::Method()
{
	//����R���о���begin
	Cluster mClusterObj;
	
	MySqlDeal mMySqlDeal;
	double mMaxMrFromData = mMySqlDeal.GetMaxMr(this->gSearchFileIndex, this->gSearchMulFile);
	cout << mMaxMrFromData << endl;
	mClusterObj.SetMaxMrFromData(mMaxMrFromData);

	mClusterObj.ClusterByR(gAllSolidVec);
	cout << "����֮ǰ" << endl;
	mClusterObj.DisplayClusterResult();

	mClusterObj.SortByLength();
	cout << "����֮��" << endl;
	mClusterObj.DisplayClusterResult();
	//����R���о���end

	
	vector<vector<int>>* mClusterResult = mClusterObj.GetClusterResult();
	list<vector<int>> mSortingResultLtVec;
	Gnome_sort(mClusterResult->size(), &mSortingResultLtVec);
	list<vector<int>>::iterator mSortingResultLtVecIter = mSortingResultLtVec.begin();
	while (mSortingResultLtVecIter != mSortingResultLtVec.end())
	{
		vector<int> mSubSortingResVec = *mSortingResultLtVecIter;
		vector<int>::iterator mSubSortingResVecIter = mSubSortingResVec.begin();
		vector<vector<int>> mOneAllSolidsForAnswer;
		while (mSubSortingResVecIter != mSubSortingResVec.end())
		{
			mOneAllSolidsForAnswer.push_back(mClusterResult->at(*mSubSortingResVecIter));
			mSubSortingResVecIter++;
		}


		vector<vector<int>>::iterator mClusterIter = mOneAllSolidsForAnswer.begin();
		vector<Riser> mOnePossAnswer;
		map<int, int> mHaveDoneSolidNumber;//��¼�Լ���������©����ţ���ֹ��β�©
		while (mClusterIter != mOneAllSolidsForAnswer.end())
		{
			vector<int>& mOneClusterLt = *mClusterIter;
			vector<int>::iterator mOneClusterLtIter = mOneClusterLt.begin();
			while (mOneClusterLtIter != mOneClusterLt.end())
			{
				if (mHaveDoneSolidNumber.find(*mOneClusterLtIter) != mHaveDoneSolidNumber.end())
				{
					mOneClusterLtIter = mOneClusterLt.erase(mOneClusterLtIter);
				}
				else
				{
					mOneClusterLtIter++;
				}
			}
			if (mOneClusterLt.size() > 0)
			{
				SubMethod(&mOneClusterLt, &mOnePossAnswer, &mHaveDoneSolidNumber);
			}
			mClusterIter++;
		}
		this->gAllPossAnswers.push_back(mOnePossAnswer);
		mSortingResultLtVecIter++;
	}
}


/**
���ݾ���Ľ�������л��ֻ����Ӽ���
*/
void Solution::SubMethod(vector<int>* tClusteIndexVec, vector<Riser> *mOnePossAnswer, map<int, int>* tHaveDoneSolidNumber)
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
	
	//mCombinedSetObj.Display();

	vector<list<int>>* mCombinedSetVec = mCombinedSetObj.GetCombinedSetVecP();
	vector<list<int>>::iterator mCombinedSetVecIter = mCombinedSetVec->begin();
	while (mCombinedSetVecIter != mCombinedSetVec->end())
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
			mIndex++;
		}
		//���ݼ��ϵ����зָʽ��ÿ���Ӽ��õ���Ӧ��Solid����end

		bool mFlag = false;
		//���ݻ����Ӽ��ϵ���Ϣ������©�����ļ��뾶
		vector<vector<Solid>>::iterator mGroupSetVecVecIter = mGroupSetVecVec.begin();
		vector<Solid>& mSameGroupSolidLt = *mGroupSetVecVecIter;
		if (OneGroupSolidAnswer(&mSameGroupSolidLt, mOnePossAnswer, tHaveDoneSolidNumber))
		{
			break;
		}

		//int mHaveFirst = 1;
		//while (mGroupSetVecVecIter != mGroupSetVecVec.end())
		//{
		//	vector<Solid>& mSameGroupSolidLt = *mGroupSetVecVecIter;
		//	//���ݻ����Ӽ��ϵ�Ԫ����м���©�����ļ��뾶
		//	if (OneGroupSolidAnswer(&mSameGroupSolidLt, mOnePossAnswer, tHaveDoneSolidNumber) && mHaveFirst == 1)
		//	{
		//		mFlag = true;
		//		break;
		//	}
		//	mHaveFirst--;
		//	mGroupSetVecVecIter++;
		//}
		//if (mFlag)
		//{
		//	break;
		//}
		
		mCombinedSetVecIter++;
	}
}

void Solution::DisplaySolution()
{
	vector<vector<Riser>>::iterator mAllPossAnswersVecVecIter = this->gAllPossAnswers.begin();
	int x = 1;
	while (mAllPossAnswersVecVecIter != this->gAllPossAnswers.end())
	{
		cout<< "�� " << x++ << " �������" << endl;
		vector<Riser>& mTempOneAnswersVec = *mAllPossAnswersVecVecIter;
		vector<Riser>::iterator mRiserIter = mTempOneAnswersVec.begin();
		cout << "���ĵ�\t\t©������\tģ����mm��" << endl;
		while (mRiserIter != mTempOneAnswersVec.end())
		{
			Riser mRiser = *mRiserIter;
			cout << "(" << mRiser.gCenter.GetX() << ", " << mRiser.gCenter.GetY() << ", " << mRiser.gCenter.GetZ() << ")\t\t" << mRiser.gIDRemark << "\t" << mRiser.gRadius << endl;
			mRiserIter++;
		}
		cout << "*************************************************" << endl << endl<<endl;
		mAllPossAnswersVecVecIter++;
	}
}

void Solution::WriteSolutionToFile(string tSolutionFilePath)
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, tSolutionFilePath.c_str(), "wb");
	if (err == 0)
	{
		fclose(fp);
	}
	ofstream SaveFile(tSolutionFilePath.c_str(), ios::in);

	vector<vector<Riser>>::iterator mAllPossAnswersVecVecIter = this->gAllPossAnswers.begin();
	while (mAllPossAnswersVecVecIter != this->gAllPossAnswers.end())
	{
		vector<Riser>& mTempOneAnswersVec = *mAllPossAnswersVecVecIter;
		vector<Riser>::iterator mRiserIter = mTempOneAnswersVec.begin();
		while (mRiserIter != mTempOneAnswersVec.end())
		{
			Riser mRiser = *mRiserIter;
			SaveFile << mRiser.gCenter.GetX() << "," << mRiser.gCenter.GetY() << "," << mRiser.gCenter.GetZ() << "\n";
			SaveFile << mRiser.gIDRemark << "\n";
			SaveFile << mRiser.gRadius << "\n";
			mRiserIter++;
		}
		SaveFile << "#\n";
		mAllPossAnswersVecVecIter++;
	}
	SaveFile.flush();
	SaveFile.close();
}

vector<vector<Riser>>* Solution::GetAllPossAnswers()
{
	return &gAllPossAnswers;
}