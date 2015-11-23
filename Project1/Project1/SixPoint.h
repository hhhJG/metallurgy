#pragma once
#include <list>
#include <vector>
#include "Point.h"
using namespace std;

class SixPoint
{
private:
	Point* gXMin;
	Point* gXMax;
	Point* gYMin;
	Point* gYMax;
	Point* gZMin;
	Point* gZMax;
	string gAllSolidIDStr;
	vector<Point> gAllSolidVec;
public:
	SixPoint(void);
	~SixPoint(void);
	void GetCenter(Point* mResult);
	const double GetRadius();
	void CompareForSixPoint(list<Point>* tAllPointList);
	void DisplayPointOfSolid();
	
	void SetPartition(int tN);
	void Display(int* tCode, int tNum);

	void CellForSolid(list<Point>* tPointsList);
	void GroupForSolids(vector<Point>* tAllSolidPointVec, int* tGroupCode, int tNumOfCode);
};
