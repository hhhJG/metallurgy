#pragma once
#include "Craft.h"

#include <string>
#include <vector>
using namespace std;

class ManageData
{
private:
	vector<Craft> mSqlCraftDataLt;
	vector<Craft> mSqlCraftDataByNameLt;
	list<string> gAllMrFileNameLt;
	string BASEPATH;
	Craft gCurrentDealCraft;

public:
	ManageData();
	~ManageData();
	int InitMySQLData(string tUserName, string tPass);
	
	void InitAllMrFileNameLt();
	list<string>* GetAllMrFileName();

	vector<Craft>* GetCraftDataLt();
	vector<Craft>* GetCraftDataByNameLt(string tCraftName);

	int GetCraftCount();
	int CopyFile(string SourceFile, string NewFile);

	void NewCraft(string tDetailMsgFilePath, string tSTLFilePath, string tGrayFilePath, string tIDandTimeFilePath, string tAllPointFilePath, string mMrFileStr);
	Craft* GetCurrentDealCraft();
	void SetCurrentDealCraft(Craft* tData);
};

