#pragma once


class Point
{
private:
	double x;
	double y;
	double z;
	int ID;
public:
	Point(void);
	Point(double tx, double ty, double tz);
	Point(double tx, double ty, double tz, int tID);
	~Point(void);
	double GetX();
	double GetY();
	double GetZ();
	int GetID();
	void SetPoint(double tx = 0.0f, double ty = 0.0f, double tz = 0.0f, int tID = 0);
	void PrintInfor();
};
