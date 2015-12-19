#pragma once
#include "Craft.h"

#include <list>
#include <vector>
#include <string>
using namespace std;

class MySqlDeal
{
private:
	list<double> gResult;
	string gMySqlUserName;
	string gMySqlPass;
	string gIP;
	string gDataBaseName;
public:
	MySqlDeal();
	MySqlDeal(string tUserName, string tPass, string tBasePath);
	~MySqlDeal();

	int InitDataBase();

	void GetDataBy(double tMr, int tFileIndex, bool tMultiFlag);
	void SelectCmd(double tMr, int tFileIndex, bool tMultiFlag, string *tSqlCmd);
	void SelectMaxMrCmd(int tFileIndex, bool tMultiFlag, string *tSqlCmd);

	void DisplayResult();
	list<double>* GetResultP();
	double GetMaxMr(int tFileIndex, bool tMultiFlag);

	void UnCheckCraftVector(vector<Craft>* tResult);
	void FuzzyQuery(string tCraftName, vector<Craft>* tResult);

	void UpdateState(CraftInfor* tData);

	void InsertCraft(CraftInfor* tData);

	int GetCraftCount();
};

