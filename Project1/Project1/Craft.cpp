#include "Craft.h"
#include "DataOperation.h"
#include "Solution.h"
#include "PriOperationData.h"

#include "MySqlDeal.h"

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>


Craft::Craft()
{
	gCraftInfor.gGrayStand = 10;
	gCraftInfor.gKeepWarm = 1;
	gCraftInfor.gMultiMrFileSear = true;
}


Craft::~Craft()
{

}

void Craft::SetCraftDetailMsg()
{
	string s;
	ifstream mCraftMsgFile(gCraftInfor.gDetailMsgFilePath);
	while (getline(mCraftMsgFile, s))
	{
		if (s.find("��������=") != string::npos)
		{
			gCraftInfor.gCraftName = s.substr(s.find("=") + 1);
		}
		gCraftDetailMsgLt.push_back(s);
	}
	mCraftMsgFile.close();
}


list<string>* Craft::GetCraftDetailMsg()
{
	return &(this->gCraftDetailMsgLt);
}


void Craft::SetCraftInfor(CraftInfor* tCraftInfor)
{
	gCraftInfor.gKey = tCraftInfor->gKey;
	gCraftInfor.gProCheckState = tCraftInfor->gProCheckState;
	gCraftInfor.gManuCheckState = tCraftInfor->gManuCheckState;

	gCraftInfor.gCraftName = tCraftInfor->gCraftName;
	gCraftInfor.gBasePath = tCraftInfor->gBasePath;

	gCraftInfor.gDetailMsgFilePath = tCraftInfor->gBasePath + "detailMsg.txt";
	gCraftInfor.g3DFilePath = tCraftInfor->gBasePath + "3D.txt";
	gCraftInfor.gAllPointFilePath = tCraftInfor->gBasePath + "idandpoint.inp";
	gCraftInfor.gBadSolidFilePath = tCraftInfor->gBasePath + "quexian.txt";
	gCraftInfor.gGrayFilePath = tCraftInfor->gBasePath + "huidu.ntl";
	gCraftInfor.gSolutionFilePath = tCraftInfor->gBasePath + "solution.txt";
	gCraftInfor.gSTLFilePath = tCraftInfor->gBasePath + "stl.stl";
	gCraftInfor.gIDandTimeFilePath = tCraftInfor->gBasePath + "idandtime.txt";

	gCraftInfor.gGrayStand = tCraftInfor->gGrayStand;	
	gCraftInfor.gKeepWarm = tCraftInfor->gKeepWarm;
	gCraftInfor.gMrFileIndex = tCraftInfor->gMrFileIndex;
	gCraftInfor.gMultiMrFileSear = tCraftInfor->gMultiMrFileSear;
	
}

bool Craft::UpdateCheckState()
{
	try
	{
		MySqlDeal mMySqlDealObj;
		mMySqlDealObj.UpdateState(&this->gCraftInfor);
		return true;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return false;
	}
}

CraftInfor* Craft::GetCraftInfor()
{
	return &gCraftInfor;
}


void Craft::SetAllPossAnswers()
{
	gAllPossAnswers.clear();


	string s;
	ifstream mSoulutionFile(gCraftInfor.gSolutionFilePath.c_str());
	double x, y, z;
	double mMr;
	int mAnswerIndex = 0;
	vector<Riser> mRiserVec;
	gAllPossAnswers.push_back(mRiserVec);
	while (getline(mSoulutionFile, s))
	{
		if(s.find("#") == string::npos)
		{
			Riser mRiser;
			x = atof(s.substr(0, s.find_first_of(",")).c_str());
			y = atof(s.substr(s.find_first_of(",") + 1, s.find_last_of(",")).c_str());
			z = atof(s.substr(s.find_last_of(",") + 1).c_str());			
			Point mCenter(x, y, z);
			mRiser.gCenter = mCenter;
			getline(mSoulutionFile, s);
			mRiser.gIDRemark = s;
			getline(mSoulutionFile, s);
			mMr = atof(s.c_str());
			mRiser.gRadius = mMr;
			gAllPossAnswers.at(mAnswerIndex).push_back(mRiser);
		}
		else
		{
			mAnswerIndex++;
			vector<Riser> mOneRiserVec;
			gAllPossAnswers.push_back(mOneRiserVec);
		}
	}
	gAllPossAnswers.erase(gAllPossAnswers.begin() + mAnswerIndex);
	mSoulutionFile.close();
}

vector<vector<Riser>>* Craft::GetAllPossAnswers()
{
	return &gAllPossAnswers;
}


bool Craft::Execute()
{
	try
	{
		string mFilePointsStr = gCraftInfor.gAllPointFilePath;
		string mThreeDFileStr = gCraftInfor.g3DFilePath;

		string mFileHuiStr = gCraftInfor.gGrayFilePath;
		string mFileIDTimeStr = gCraftInfor.gIDandTimeFilePath;
		string mFileSolidsStr = gCraftInfor.gBadSolidFilePath;

		string mFileSolutionStr = gCraftInfor.gSolutionFilePath;

		int mStandGray = gCraftInfor.gGrayStand;
		int mKeepWarm = gCraftInfor.gKeepWarm;
		int mMrFileIndex = gCraftInfor.gMrFileIndex;
		bool mMultiMrSearch = gCraftInfor.gMultiMrFileSear;

		//�����ݽ���Ԥ�����Ա�ﵽ���������ļ�
		PriOperationData mPriOperationData(mKeepWarm, mStandGray);
		vector<Point> mAllPointsOfSolids;
		mPriOperationData.AllPointsOfSolids(mFileHuiStr, mFilePointsStr, mFileIDTimeStr);
		list<list<Point>> mSplitPointsLtLt;
		mPriOperationData.DetachSolids();
		mPriOperationData.WriteToFile(mFileSolidsStr);

		Point mTopSidePoint;//��ҪΪ�˻�ȡzֵ��ȷ����ð�ڵ�ƽ�档zֵ�ĵ�λ��mm
		mPriOperationData.GetTopSidePoint(&mTopSidePoint);

		//���3D�ļ�����������ʹ��
		mPriOperationData.ThreeDDeal(mFilePointsStr);
		mPriOperationData.ThreeDWriteToFile(mThreeDFileStr);

		//�ṩȱ���ļ�
		DataOperation mDataObj;
		mDataObj.LoadFile(mFileSolidsStr.c_str());

		Solution mSolution(mDataObj.GetAllSolidVecP());
		mSolution.SetTopSidePoint(&mTopSidePoint);
		mSolution.SetSerchCoindition(mMrFileIndex, mMultiMrSearch);

		//����뾶R
		mSolution.Method();
		mSolution.WriteSolutionToFile(mFileSolutionStr);

		MySqlDeal mMySqlDealObj;
		gCraftInfor.gBasePath.replace(gCraftInfor.gBasePath.find_first_of("\\"), 1, "\\\\");
		gCraftInfor.gBasePath.replace(gCraftInfor.gBasePath.find_last_of("\\"), 1, "\\\\");
		cout << gCraftInfor.gBasePath << endl;
		mMySqlDealObj.InsertCraft(&gCraftInfor);

		SetAllPossAnswers();
		return true;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return false;
	}
}

void Craft::DisplayInfor()
{
	cout << "3D�ļ�·����" << gCraftInfor.g3DFilePath << endl;
	cout << "�����ļ�·����" << gCraftInfor.gAllPointFilePath << endl;
	cout << "��׼ȱ���ļ�·����" << gCraftInfor.gBadSolidFilePath << endl;	
	cout << "������ϸ��Ϣ�ļ�·����" << gCraftInfor.gDetailMsgFilePath << endl;
	cout << "�Ҷ��ļ�·����" << gCraftInfor.gGrayFilePath << endl;
	cout << "����ʱ���ļ�·����" << gCraftInfor.gIDandTimeFilePath << endl;

	cout << "STL�ļ�·����" << gCraftInfor.gSTLFilePath << endl;
	cout << "��������ļ�·����" << gCraftInfor.gSolutionFilePath << endl;
	cout << "�������״̬��" << gCraftInfor.gProCheckState << endl;

	cout << "��������״̬��" << gCraftInfor.gManuCheckState << endl;
	cout << "Key��" << gCraftInfor.gKey << endl;
	cout << "Mr�ļ���ţ�" << gCraftInfor.gMrFileIndex << endl;
	cout << "�Ƿ���ð��(1-�ǣ�0-��)��" << gCraftInfor.gKeepWarm << endl;
	cout << "�Ҷȱ�׼��" << gCraftInfor.gGrayStand << endl;
	cout << "�������ƣ�" << gCraftInfor.gCraftName << endl;

	list<string>::iterator mCraftDetailMsgLtIter = gCraftDetailMsgLt.begin();
	cout << "������ϸ��Ϣ��" << endl;
	while (mCraftDetailMsgLtIter != gCraftDetailMsgLt.end())
	{
		cout << *mCraftDetailMsgLtIter << endl;
		mCraftDetailMsgLtIter++;
	}

	int m = 1;
	vector<vector<Riser>>::iterator mAllPossAnswersIter = gAllPossAnswers.begin();
	while (mAllPossAnswersIter != gAllPossAnswers.end())
	{
		vector<Riser>& mOneSolutionVec = *mAllPossAnswersIter;
		vector<Riser>::iterator mOneSolutionVecIter = mOneSolutionVec.begin();
		cout << "��" << m++ << "���������ʽΪ ����-©������-ģ����mm��" << endl;
		while (mOneSolutionVecIter != mOneSolutionVec.end())
		{
			Riser& mOneRiser = *mOneSolutionVecIter;
			cout << "(" << mOneRiser.gCenter.GetX() << " , " << mOneRiser.gCenter.GetY() << " , " << mOneRiser.gCenter.GetZ() <<")" << endl;
			cout << mOneRiser.gIDRemark << endl;
			cout << mOneRiser.gRadius << endl;
			mOneSolutionVecIter++;
		}
		mAllPossAnswersIter++;
		cout << endl << endl;
	}
}