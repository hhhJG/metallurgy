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
		if (s.find("工件名称=") != string::npos)
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

		//对数据进行预处理，以便达到生成期望文件
		PriOperationData mPriOperationData(mKeepWarm, mStandGray);
		vector<Point> mAllPointsOfSolids;
		mPriOperationData.AllPointsOfSolids(mFileHuiStr, mFilePointsStr, mFileIDTimeStr);
		list<list<Point>> mSplitPointsLtLt;
		mPriOperationData.DetachSolids();
		mPriOperationData.WriteToFile(mFileSolidsStr);

		Point mTopSidePoint;//主要为了获取z值，确定打冒口的平面。z值的单位是mm
		mPriOperationData.GetTopSidePoint(&mTopSidePoint);

		//输出3D文件，供神经网络使用
		mPriOperationData.ThreeDDeal(mFilePointsStr);
		mPriOperationData.ThreeDWriteToFile(mThreeDFileStr);

		//提供缺陷文件
		DataOperation mDataObj;
		mDataObj.LoadFile(mFileSolidsStr.c_str());

		Solution mSolution(mDataObj.GetAllSolidVecP());
		mSolution.SetTopSidePoint(&mTopSidePoint);
		mSolution.SetSerchCoindition(mMrFileIndex, mMultiMrSearch);

		//聚类半径R
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
	cout << "3D文件路径：" << gCraftInfor.g3DFilePath << endl;
	cout << "坐标文件路径：" << gCraftInfor.gAllPointFilePath << endl;
	cout << "标准缺陷文件路径：" << gCraftInfor.gBadSolidFilePath << endl;	
	cout << "工艺详细信息文件路径：" << gCraftInfor.gDetailMsgFilePath << endl;
	cout << "灰度文件路径：" << gCraftInfor.gGrayFilePath << endl;
	cout << "凝固时间文件路径：" << gCraftInfor.gIDandTimeFilePath << endl;

	cout << "STL文件路径：" << gCraftInfor.gSTLFilePath << endl;
	cout << "解决方案文件路径：" << gCraftInfor.gSolutionFilePath << endl;
	cout << "程序检验状态：" << gCraftInfor.gProCheckState << endl;

	cout << "生产检验状态：" << gCraftInfor.gManuCheckState << endl;
	cout << "Key：" << gCraftInfor.gKey << endl;
	cout << "Mr文件编号：" << gCraftInfor.gMrFileIndex << endl;
	cout << "是否保温冒口(1-是，0-否)：" << gCraftInfor.gKeepWarm << endl;
	cout << "灰度标准：" << gCraftInfor.gGrayStand << endl;
	cout << "工艺名称：" << gCraftInfor.gCraftName << endl;

	list<string>::iterator mCraftDetailMsgLtIter = gCraftDetailMsgLt.begin();
	cout << "工艺详细信息：" << endl;
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
		cout << "第" << m++ << "个结果：格式为 中心-漏洞集合-模数（mm）" << endl;
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