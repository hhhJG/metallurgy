#pragma once
#include "Solid.h"
#include <vector>
#include <list>
using namespace std;

class Cluster
{
private:
	vector<vector<int>> gClusterResult;
	double gMaxMrFromData;
public:
	Cluster(void);
	~Cluster(void);
	double MaxDisOfTwoSolid(Solid* tFirst, Solid* tSecond);
	void ClusterByR(vector<Solid>* tSolidVecP);
	void SortByLength();
	vector<vector<int>>* GetClusterResult();
	void DisplayClusterResult();
	void SetMaxMrFromData(double tMaxMr);
};
