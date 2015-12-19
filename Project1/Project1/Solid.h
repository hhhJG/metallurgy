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
	double gModulus;//ģ��
	int gID;//����
	double gJuLeiRadius; //����뾶�����ڴ���©����©���ľ��࣬�Լ�ȷ����������
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
