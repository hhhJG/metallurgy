#pragma once
#include "Point.h"
#include "Solid.h"
#include <vector>
#include <string.h>
#include <list>
#include <map>


struct Riser
{
	string gIDRemark;
	Point gCenter;
	double gRadius;
};

class Solution
{
	vector<Riser> gAnswers;
	vector<Solid>* gAllSolidVec;
	map<int, int> gHaveDoneSolidNumber;//记录以及补缩过的漏洞编号，防止多次补漏
public:
	Solution();
	Solution(vector<Solid>* tAllSolidVec);
	~Solution();
	void GetCenterOfSolids(vector<Solid>* tSolidsVec, Point* tResult);
	string ListIntToString(list<int>* tData);
	string VectorIntToString(vector<int>* tData);
	void Method(double tR);
	void SubMethod(vector<int>* tClusteIndexLt, double tR);
	void OneGroupSolidAnswer(vector<Solid>* tSolidsVec, double tR, list<int>* tSameGroupSolidID);
	void DisplaySolution();
};

