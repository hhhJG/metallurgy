#pragma once
#include "Point.h"
#include "Solid.h"
#include <vector>
#include <string.h>
#include <list>


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

public:
	Solution();
	Solution(vector<Solid>* tAllSolidVec);
	~Solution();
	void GetCenterOfSolids(vector<Solid>* tSolidsVec, Point* tResult);
	string ListIntToString(list<int>* tData);
	void Method(double tR);
	void SubMethod(list<int>* tClusteIndexLt, list<int>* tModulusLt);
};

