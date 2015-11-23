#pragma once

#include "Point.h"
#include "Solid.h"
#include <vector>
using namespace std;


class DataOperation
{
private:
	vector<Solid> gAllSolidVec;
	int gSolidCount;

public:
	DataOperation(void);
	~DataOperation(void);
	void LoadFile(const char* t_path);
	void SaveFile(const char* t_path);

	vector<Solid>* GetAllSolidVecP();
	void LineToPoint(string t_lineStr, double* result);

	const int GetSolidCount();
};
