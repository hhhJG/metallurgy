#pragma once
#include "Point.h"

#include <string>
#include <list>
#include <map>
#include <vector>
using namespace std;

struct Grid
{
	long first;
	long second;
	long third;
	long fourth;
};

struct IntPoint
{
	int x;
	int y;
	int z;
};

class PriOperationData
{
private:
	//0-正常冒口；1-保温冒口。默认情况下为0
	int gKeepWarm;
	//漏洞标准，默认10%
	double gStandNumOfGray;
	//符合灰度要求的ID
	map<long, long> gOKIDToGrayMap;

	//所有点的坐标，包括浇道
	map<long,Point> gAllOkPointsMap;

	//所有网格数据统计
	list<Grid> gAllGridLt;

	//漏洞中的一个特殊ID
	list<long> gSpeIDOfPointToSolidLt;

	//ID与模数
	map<long, double> mMrByID;

	//ID与补缩距离R，用于处理漏洞与漏洞的聚类，以及确定补缩距离
	map<long, double> mRByID;

	//根据特殊ID查找该漏洞其他的ID的统计信息
	map<long, list<long>> gSolidGroupMap;

	//聚类之后每个漏洞对应坐标的统计信息
	map<long, list<Point>> gPointOfAllSolidsResult;
	
	//主要为了获取z值，确定打冒口的平面。z值的单位是mm
	Point gTopSidePoint;


	///////////////////////////////////////////////////////////////////////
	map<int, list<IntPoint>> gAllIntPointMap;
	list<Point> gAllPointLt;
public:
	PriOperationData();
	PriOperationData(int tKeepWarm, double tStandNumOfGray = 0.1);
	~PriOperationData();
	void SetKeepWarmStdGray(int tKeepWarm, double tStandNumOfGray = 0.1);

	/**
	tGrayFilePath：灰度文件路径。文件内容：ID 灰度（其中ID是自增序列，灰度无规则）
	tIDPointFile：所有坐标点文件路径。文件内容：与灰度文件中对应的ID所对应所有坐标点（ID x y z）
	tSolidIDTimeFile:漏洞中特殊ID和时间
	*/
	void AllPointsOfSolids(string tGrayFilePath, string tIDPointsFile, string tSolidIDTimeFile);

	void DetachSolids();

	void WriteToFile(string tFilePath);

	void GetTopSidePoint(Point *result);

	void ThreeDDeal(string tIDPointsFile);
	void ThreeDWriteToFile(string tFilePath);
};

