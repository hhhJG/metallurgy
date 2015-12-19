#include "DataOperation.h"
#include "Solution.h"
#include "PriOperationData.h"
#include "Craft.h"
#include "MySqlDeal.h"

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <math.h>
#include "ManageData.h"
using namespace std;


int main()
{
	//string mFilePointsStr = "source\\gongzuotai\\IDandzuobiao.inp";
	//string mThreeDFileStr = "source\\gongzuotai\\3D.txt";

	//string mFileHuiStr = "source\\gongzuotai\\huidu.ntl";
	//string mFileIDTimeStr = "source\\gongzuotai\\IDandtime.txt";
	//string mFileSolidsStr = "source\\gongzuotai\\quexian.txt";

	////对数据进行预处理，以便达到生成期望文件
	//PriOperationData mPriOperationData(1, 10);
	//vector<Point> mAllPointsOfSolids;
	//mPriOperationData.AllPointsOfSolids(mFileHuiStr, mFilePointsStr, mFileIDTimeStr);
	//list<list<Point>> mSplitPointsLtLt;
	//mPriOperationData.DetachSolids();
	//mPriOperationData.WriteToFile(mFileSolidsStr);

	//Point mTopSidePoint;//主要为了获取z值，确定打冒口的平面。z值的单位是mm
	//mPriOperationData.GetTopSidePoint(&mTopSidePoint);

	////输出3D文件，供神经网络使用
	//mPriOperationData.ThreeDDeal(mFilePointsStr);
	//mPriOperationData.ThreeDWriteToFile(mThreeDFileStr);

	////提供缺陷文件
	//DataOperation mDataObj;
	//mDataObj.LoadFile(mFileSolidsStr.c_str());
	//	
	//Solution mSolution(mDataObj.GetAllSolidVecP());
	//mSolution.SetTopSidePoint(&mTopSidePoint);
	//mSolution.SetSerchCoindition(11, true);

	////聚类半径R
	//mSolution.Method();
	//mSolution.DisplaySolution();


	/*MySqlDeal mMySqlDeal;
	mMySqlDeal.InitDataBase();*/


	ManageData mManageData;
	mManageData.InitMySQLData("root", "root");



	list<string>* mMrFileNameLt;
	mMrFileNameLt = mManageData.GetAllMrFileName();
	list<string>::iterator mMrFileNameLtIter = mMrFileNameLt->begin();
	cout << "模数文件列表：" << endl;
	while (mMrFileNameLtIter != mMrFileNameLt->end())
	{
		cout << *mMrFileNameLtIter << endl;
		mMrFileNameLtIter++;
	}
	cout << endl << endl << endl;

	vector<Craft>* mUnOkCraftData;
	mUnOkCraftData = mManageData.GetCraftDataLt();
	cout << "数据库中已有未确认的工艺：" << endl;
	vector<Craft>::iterator mUnOkCraftDataIter = mUnOkCraftData->begin();
	while (mUnOkCraftDataIter != mUnOkCraftData->end())
	{
		Craft& mCraftOneObj = *mUnOkCraftDataIter;
		mCraftOneObj.SetAllPossAnswers();
		mCraftOneObj.DisplayInfor();
		mUnOkCraftDataIter++;
	}

	Craft* mCurrentCraftP;
	int x = 0;
	while (x < 3)
	{
		mManageData.NewCraft("source\\gongzuotai\\detailMsg.txt", "source\\gongzuotai\\stl.stl", "source\\gongzuotai\\huidu.ntl", "source\\gongzuotai\\IDandtime.txt", "source\\gongzuotai\\IDandzuobiao.inp", "标准圆柱形明冒口（h = d）");
		mCurrentCraftP = mManageData.GetCurrentDealCraft();		
		list<string>* mCreftDetailMsg;
		mCurrentCraftP->SetCraftDetailMsg();
		mCreftDetailMsg = mCurrentCraftP->GetCraftDetailMsg();
		mCurrentCraftP->Execute();

		Craft& mCraftM = mUnOkCraftData->at(x);

		mManageData.SetCurrentDealCraft(&mCraftM);
		CraftInfor* mCraftInfor;
		mCraftInfor = mCurrentCraftP->GetCraftInfor();
		mCraftInfor->gProCheckState = 1;
		mCraftInfor->gManuCheckState = 0;
		mCurrentCraftP->UpdateCheckState();
		x++;
	}


	system("pause");
	return 0;
}