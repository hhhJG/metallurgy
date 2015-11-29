#pragma once
#include <list>
using namespace std;

class MySqlDeal
{
private:
	list<double> gResult;
public:
	MySqlDeal();
	~MySqlDeal();
	void GetDataBy(double tMr, int tFileIndex);
	void DisplayResult();
	list<double>* GetResultP();
};

