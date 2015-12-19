#pragma once
#include "Point.h"
#include <list>
using namespace std;

class Solid
{
private:
	Point gXMin;
	Point gXMax;
	Point gYMin;
	Point gYMax;
	Point gZMin;
	Point gZMax;
	double gModulus;//模数
	int gID;//索引
	double gJuLeiRadius; //聚类半径，用于处理漏洞与漏洞的聚类，以及确定补缩距离
	list<Point> gThisSolidPointLt;
public:
	Solid(void);
	Solid(double tModulus, int tID, double tJuLeiRadius);
	~Solid(void);
	void CompareForCuboid(list<Point> *tThisSolidPointLt);
	Point* GetXMax();
	Point* GetXMin();
	Point* GetYMax();
	Point* GetYMin();
	Point* GetZMax();
	Point* GetZMin();
	void AddPointToThisSolid(Point& tPointP);
	void SetFactor(double tModulus, int tID, double tJuLeiRadius);
	void SetEightPointOfCuboid(Point* tResult);
	list<Point>* GetThisSolidPointLt();
	const double GetModulus();
	int GetID();
	const double GetJuLeiRadius();
};
