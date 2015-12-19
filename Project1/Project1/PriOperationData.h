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
	//0-����ð�ڣ�1-����ð�ڡ�Ĭ�������Ϊ0
	int gKeepWarm;
	//©����׼��Ĭ��10%
	double gStandNumOfGray;
	//���ϻҶ�Ҫ���ID
	map<long, long> gOKIDToGrayMap;

	//���е�����꣬��������
	map<long,Point> gAllOkPointsMap;

	//������������ͳ��
	list<Grid> gAllGridLt;

	//©���е�һ������ID
	list<long> gSpeIDOfPointToSolidLt;

	//ID��ģ��
	map<long, double> mMrByID;

	//ID�벹������R�����ڴ���©����©���ľ��࣬�Լ�ȷ����������
	map<long, double> mRByID;

	//��������ID���Ҹ�©��������ID��ͳ����Ϣ
	map<long, list<long>> gSolidGroupMap;

	//����֮��ÿ��©����Ӧ�����ͳ����Ϣ
	map<long, list<Point>> gPointOfAllSolidsResult;
	
	//��ҪΪ�˻�ȡzֵ��ȷ����ð�ڵ�ƽ�档zֵ�ĵ�λ��mm
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
	tGrayFilePath���Ҷ��ļ�·�����ļ����ݣ�ID �Ҷȣ�����ID���������У��Ҷ��޹���
	tIDPointFile������������ļ�·�����ļ����ݣ���Ҷ��ļ��ж�Ӧ��ID����Ӧ��������㣨ID x y z��
	tSolidIDTimeFile:©��������ID��ʱ��
	*/
	void AllPointsOfSolids(string tGrayFilePath, string tIDPointsFile, string tSolidIDTimeFile);

	void DetachSolids();

	void WriteToFile(string tFilePath);

	void GetTopSidePoint(Point *result);

	void ThreeDDeal(string tIDPointsFile);
	void ThreeDWriteToFile(string tFilePath);
};

