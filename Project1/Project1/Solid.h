#pragma once
#include "Point.h"
#include <list>
using namespace std;

class Solid
{
private:
	Point* gXMin;
	Point* gXMax;
	Point* gYMin;
	Point* gYMax;
	Point* gZMin;
	Point* gZMax;
	double gModulus;//Ä£Êý
	int gID;//Ë÷Òý
	list<Point> gThisSolidPointLt;
public:
	Solid(void);
	Solid(double tModulus, int tID);
	~Solid(void);
	void CompareForCuboid();
	Point* GetXMax();
	Point* GetXMin();
	Point* GetYMax();
	Point* GetYMin();
	Point* GetZMax();
	Point* GetZMin();
	void AddPointToThisSolid(Point& tPointP);
	void SetFactor(double tModulus, int tID);
	void SetEightPointOfCuboid(Point* tResult);
	const double GetModulus();
	int GetID();
};
