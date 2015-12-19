#include "ManageData.h"
#include "MySqlDeal.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <direct.h>
using namespace std;

void ManageData::InitAllMrFileNameLt()
{
	if (gAllMrFileNameLt.size() == 0)
	{
		gAllMrFileNameLt.push_back("��׼��ð�ڣ�h=1.5d��");
		gAllMrFileNameLt.push_back("��׼��ð�ڣ�h = 2d��");
		gAllMrFileNameLt.push_back("��׼���ΰ�ð�ڣ�b = 2a��h = 1.5a��");
		gAllMrFileNameLt.push_back("��׼���ΰ�ð�ڣ�b = h = 1.5a��");
		gAllMrFileNameLt.push_back("��׼������ð�ڣ�b = 1.5a, h = 1.25a��");
		gAllMrFileNameLt.push_back("��׼������ð�ڣ�b = 2a, h = 1.5a��");
		gAllMrFileNameLt.push_back("��׼Բ���ΰ�ð�ڣ�h = 1.2d��");
		gAllMrFileNameLt.push_back("��׼Բ���ΰ�ð�ڣ�h = 1.5d��");
		gAllMrFileNameLt.push_back("��׼Բ���ΰ�ð�ڣ�h = d��");
		gAllMrFileNameLt.push_back("��׼Բ������ð�ڣ�h = 1.5d��");
		gAllMrFileNameLt.push_back("��׼Բ������ð�ڣ�h = d��");
	}
}

list<string>* ManageData::GetAllMrFileName()
{
	return &gAllMrFileNameLt;
}

ManageData::ManageData()
{
	InitAllMrFileNameLt();
	BASEPATH = "source\\data\\";
}


ManageData::~ManageData()
{

}

int ManageData::InitMySQLData(string tUserName, string tPass)
{
	int result = 0;
	MySqlDeal mMySqlDealObj(tUserName, tPass, BASEPATH);
	result = mMySqlDealObj.InitDataBase();
	return result;
}


vector<Craft>* ManageData::GetCraftDataLt()
{
	MySqlDeal mMySqlDealObj;
	mMySqlDealObj.UnCheckCraftVector(&mSqlCraftDataLt);
	return &mSqlCraftDataLt;
}

vector<Craft>* ManageData::GetCraftDataByNameLt(string tCraftName)
{
	MySqlDeal mMySqlDealObj;
	mMySqlDealObj.FuzzyQuery(tCraftName, &mSqlCraftDataByNameLt);
	return &mSqlCraftDataByNameLt;
}

int ManageData::GetCraftCount()
{
	return mSqlCraftDataLt.size();
}


void ManageData::NewCraft(string tDetailMsgFilePath, string tSTLFilePath, string tGrayFilePath, string tIDandTimeFilePath, string tAllPointFilePath, string mMrFileStr)
{
	MySqlDeal mMySqlDeal;
	int mHadCraftsCount = mMySqlDeal.GetCraftCount() + 1;

	stringstream  mSStream;
	mSStream << mHadCraftsCount;
	string mx = mSStream.str();
	mSStream.str("");

	string mRootPathStr = BASEPATH + mx + "\\";
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, mRootPathStr.c_str(), "r");
	if (err == 0)
	{
		fclose(fp);
	}
	else
	{
		_mkdir(mRootPathStr.c_str());
	}


	CopyFile(tDetailMsgFilePath, mRootPathStr + "detailMsg.txt");
	CopyFile(tSTLFilePath, mRootPathStr + "stl.stl");
	CopyFile(tGrayFilePath, mRootPathStr + "huidu.ntl");
	CopyFile(tIDandTimeFilePath, mRootPathStr + "idandtime.txt");
	CopyFile(tAllPointFilePath, mRootPathStr + "idandpoint.inp");

	CraftInfor mCraftInfor;
	mCraftInfor.gBasePath = mRootPathStr;
	mCraftInfor.gKey = mHadCraftsCount;
	if (mMrFileStr.find("��׼��ð�ڣ�h=1.5d��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 1;
	}
	else if (mMrFileStr.find("��׼��ð�ڣ�h = 2d��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 2;
	}
	else if (mMrFileStr.find("��׼���ΰ�ð�ڣ�b = 2a��h = 1.5a��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 3;
	}
	else if (mMrFileStr.find("��׼���ΰ�ð�ڣ�b = h = 1.5a��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 4;
	}
	else if (mMrFileStr.find("��׼������ð�ڣ�b = 1.5a, h = 1.25a��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 5;
	}
	else if (mMrFileStr.find("��׼������ð�ڣ�b = 2a, h = 1.5a��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 6;
	}
	else if (mMrFileStr.find("��׼Բ���ΰ�ð�ڣ�h = 1.2d��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 7;
	}
	else if (mMrFileStr.find("��׼Բ���ΰ�ð�ڣ�h = 1.5d��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 8;
	}
	else if (mMrFileStr.find("��׼Բ���ΰ�ð�ڣ�h = d��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 9;
	}
	else if (mMrFileStr.find("��׼Բ������ð�ڣ�h = 1.5d��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 10;
	}
	else if (mMrFileStr.find("��׼Բ������ð�ڣ�h = d��") != string::npos)
	{
		mCraftInfor.gMrFileIndex = 11;
	}
	gCurrentDealCraft.SetCraftInfor(&mCraftInfor);
	mSqlCraftDataLt.push_back(gCurrentDealCraft);
}

Craft* ManageData::GetCurrentDealCraft()
{
	return &gCurrentDealCraft;
}

void ManageData::SetCurrentDealCraft(Craft* tData)
{
	gCurrentDealCraft.SetCraftInfor(tData->GetCraftInfor());
}

int ManageData::CopyFile(string SourceFile, string NewFile)
{
	ifstream in;
	ofstream out;
	in.open(SourceFile, ios::binary);//��Դ�ļ�
	if (in.fail())//��Դ�ļ�ʧ��
	{
		cout << "Error 1: Fail to open the source file." << endl;
		in.close();
		out.close();
		return 0;
	}
	out.open(NewFile, ios::binary);//����Ŀ���ļ� 
	if (out.fail())//�����ļ�ʧ��
	{
		cout << "Error 2: Fail to create the new file." << endl;
		out.close();
		in.close();
		return 0;
	}
	else//�����ļ�
	{
		out << in.rdbuf();
		out.close();
		in.close();
		return 1;
	}
}