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
	vector<vector<Riser>> gAllPossAnswers;
	vector<Solid>* gAllSolidVec;
	Point gTopSideP; //上表面（打冒口的平面），主要为了获取z值
	bool gSearchMulFile;
	int gSearchFileIndex;
public:
	Solution();
	Solution(vector<Solid>* tAllSolidVec);
	~Solution();
	void SetTopSidePoint(Point* tTopPoint);
	void GetCenterOfSolids(vector<Solid>* tSolidsVec, Point* tResult);
	string ListIntToString(list<int>* tData);
	string VectorIntToString(vector<int>* tData);
	
	void SetSerchCoindition(int tSearchFileIndex, bool tSearchMulFile = false);

	string VectorSolidIDToString(vector<Solid>* tSolidsVec);
	void Gnome_sort(int length, list<vector<int>>* result);

	void Method();
	void SubMethod(vector<int>* tClusteIndexLt, vector<Riser> *mOnePossAnswer, map<int, int>* tHaveDoneSolidNumber);
	bool OneGroupSolidAnswer(vector<Solid>* tSolidsVec, vector<Riser> *mOnePossAnswer, map<int, int>* tHaveDoneSolidNumber);
	void DisplaySolution();
	void WriteSolutionToFile(string tSolutionFilePath);

	vector<vector<Riser>> *GetAllPossAnswers();
};

