#include "Point.h"
#include <iostream>
using namespace std;

Point::Point(void)
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->ID = 0;
}

Point::~Point(void)
{

}

Point::Point(double tx, double ty, double tz)
{
	this->x = tx;
	this->y = ty;
	this->z = tz;
	this->ID = 0;
}

Point::Point(double tx, double ty, double tz, int tID)
{
	this->x = tx;
	this->y = ty;
	this->z = tz;
	this->ID = tID;
}

double Point::GetX()
{
	return this->x;
}

double Point::GetY()
{
	return this->y;
}

double Point::GetZ()
{
	return this->z;
}

int Point::GetID()
{
	return this->ID;
}

void Point::SetPoint(double tx, double ty, double tz, int tID)
{
	this->x = tx;
	this->y = ty;
	this->z = tz;
	this->ID = tID;
}

void Point::PrintInfor()
{
	cout<< this->x<< "\t"<< this->y<< "\t"<< this->z<<endl;
}