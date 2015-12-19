#include "Solid.h"
#include "Point.h"
#include <stdlib.h>
#include <list>
#include <iostream>
using namespace std;

Solid::Solid(void)
{
	this->gThisSolidPointLt.clear();
}

Solid::~Solid(void)
{
}

Solid::Solid(double tModulus, int tID, double tJuLeiRadius)
{
	this->gModulus = tModulus;
	this->gID = tID;
	this->gJuLeiRadius = tJuLeiRadius;
}


void Solid::CompareForCuboid(list<Point> *tThisSolidPointLt)
{
	list<Point>::iterator mPointLtIter = tThisSolidPointLt->begin();
	Point& mFirstPoint = *mPointLtIter;
	this->gXMax = mFirstPoint;
	this->gXMin = mFirstPoint;
	this->gYMax = mFirstPoint;
	this->gYMin = mFirstPoint;
	this->gZMax = mFirstPoint;
	this->gZMin = mFirstPoint;
	mPointLtIter ++;

	


	while(mPointLtIter != tThisSolidPointLt->end())
	{
		Point& mOnePoint = *mPointLtIter;
		if(this->gXMax.GetX() < mOnePoint.GetX())
		{
			this->gXMax = mOnePoint;
		}
		else if(this->gXMin.GetX() > mOnePoint.GetX())
		{
			this->gXMin = mOnePoint;
		}

		if(this->gYMax.GetY() < mOnePoint.GetY())
		{
			this->gYMax = mOnePoint;
		}
		else if(this->gYMin.GetY() > mOnePoint.GetY())
		{
			this->gYMin = mOnePoint;
		}

		if(this->gZMax.GetZ() < mOnePoint.GetZ())
		{
			this->gZMax = mOnePoint;
		}
		else if(this->gZMin.GetZ() > mOnePoint.GetZ())
		{
			this->gZMin = mOnePoint;
		}
		mPointLtIter ++;
	}
}

Point* Solid::GetXMax()
{
	return &gXMax;
}

	
Point* Solid::GetXMin()
{
	return &gXMin;
}


Point* Solid::GetYMax()
{
	return &gYMax;
}


Point* Solid::GetYMin()
{
	return &gYMin;
}

Point* Solid::GetZMax()
{
	return &gZMax;
}

Point* Solid::GetZMin()
{
	return &gZMin;
}

void Solid::SetFactor(double tModulus, int tID, double tJuLeiRadius)
{
	this->gModulus = tModulus;
	this->gID = tID;
	this->gJuLeiRadius = tJuLeiRadius;
}

const double Solid::GetModulus()
{
	return this->gModulus;
}


int Solid::GetID()
{
	return this->gID;
}

const double Solid::GetJuLeiRadius()
{
	return this->gJuLeiRadius;
}

void Solid::AddPointToThisSolid(Point& tPointP)
{
	this->gThisSolidPointLt.push_back(tPointP);
}

list<Point>* Solid::GetThisSolidPointLt()
{
	return &gThisSolidPointLt;
}

void Solid::SetEightPointOfCuboid(Point* tResult)
{
	//(0,0,0)
	Point mxyz(this->gXMin.GetX(), this->gYMin.GetY(), this->gZMin.GetZ());
	*(tResult + 0) = mxyz;
	//(0,0,1)
	Point mxyZ(this->gXMin.GetX(), this->gYMin.GetY(), this->gZMax.GetZ());
	*(tResult + 1) = mxyZ;
	//(0,1,0)
	Point mxYz(this->gXMin.GetX(), this->gYMax.GetY(), this->gZMin.GetZ());
	*(tResult + 2) = mxYz;
	//(0,1,1)
	Point mxYZ(this->gXMin.GetX(), this->gYMax.GetY(), this->gZMax.GetZ());
	*(tResult + 3) = mxYZ;

	//(1,0,0)
	Point mXyz(this->gXMax.GetX(), this->gYMin.GetY(), this->gZMin.GetZ());
	*(tResult + 4) = mXyz;
	//(1,0,1)
	Point mXyZ(this->gXMax.GetX(), this->gYMin.GetY(), this->gZMax.GetZ());
	*(tResult + 5) = mXyZ;
	//(1,1,0)
	Point mXYz(this->gXMax.GetX(), this->gYMax.GetY(), this->gZMin.GetZ());
	*(tResult + 6) = mXYz;
	//(1,1,1)
	Point mXYZ(this->gXMax.GetX(), this->gYMax.GetY(), this->gZMax.GetZ());
	*(tResult + 7) = mXYZ;
}