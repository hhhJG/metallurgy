#include "PriOperationData.h"
#include "Point.h"

#include <string>
#include <stdlib.h>
#include <list>
#include <map>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
using namespace std;


PriOperationData::PriOperationData()
{
	this->gKeepWarm = 0;
	this->gStandNumOfGray = 10;
}

PriOperationData::PriOperationData(int tKeepWarm, double tStandNumOfGray)
{
	this->gKeepWarm = tKeepWarm;
	this->gStandNumOfGray = tStandNumOfGray;
}

void PriOperationData::SetKeepWarmStdGray(int tKeepWarm, double tStandNumOfGray)
{
	this->gKeepWarm = tKeepWarm;
	this->gStandNumOfGray = tStandNumOfGray;
}


PriOperationData::~PriOperationData()
{
}


/**
tGrayFilePath���Ҷ��ļ�·�����ļ����ݣ�ID �Ҷȣ�����ID���������У��Ҷ��޹���
tIDPointFile������������ļ�·�����ļ����ݣ���Ҷ��ļ��ж�Ӧ��ID����Ӧ��������㣨ID x y z��
tSolidIDTimeFile:©��������ID��ʱ��
*/
void PriOperationData::AllPointsOfSolids(string tGrayFilePath, string tIDPointsFile, string tSolidIDTimeFile)
{
	string s;

	//©���е�һ������ID������������ʱ���ļ�����begin
	ifstream mSolidIDTimeFile(tSolidIDTimeFile.c_str());
	getline(mSolidIDTimeFile, s);//������Ҫ������
	long mSpecialID = 0;
	double mSpeTime = 0.0;
	double mMr = 0.0;
	double mR = 0.0;
	while (getline(mSolidIDTimeFile, s))
	{
		if (s.find("point") == string::npos)
		{
			mSpecialID = atoi(s.substr(0, s.find_first_of(" ")).c_str());
			mSpeTime = atof(s.substr(s.find_first_of(" ")).c_str());
			double mRadio = 0.0885;
			if (mSpeTime < 5000)
			{
				mRadio = 0.0885;
			}
			else if (mSpeTime >= 5000 && mSpeTime < 20000)
			{
				mRadio = 0.0684;
			}
			else if (mSpeTime >= 20000 && mSpeTime < 50000)
			{
				mRadio = 0.06;
			}
			else if (mSpeTime >= 50000)
			{
				mRadio = 0.053;
			}
			mMr = sqrt(mSpeTime) * mRadio * 1.2;
			if (gKeepWarm == 1)
			{
				mMr = mMr / 1.43;
			}

			mR = sqrt(mSpeTime) * mRadio * 9;

			mMrByID.insert(pair<long, double>(mSpecialID, mMr));
			mRByID.insert(pair<long, double>(mSpecialID, mR));

			gSpeIDOfPointToSolidLt.push_back(mSpecialID);
		}
		else if (s.find("point=") != string::npos)
		{
			s = s.substr(s.find("=") + 1);
			double x = atof(s.substr(0, s.find(",")).c_str());
			s = s.substr(s.find_first_of(",") + 1);
			double y = atof(s.substr(0, s.find(",")).c_str());
			s = s.substr(s.find_first_of(",") + 1);
			double z = atof(s.substr(0, s.find(",")).c_str());
			gTopSidePoint.SetPoint(x, y, z);
		}
	}
	mSolidIDTimeFile.close();
	//©���е�һ������ID������������ʱ���ļ�����end

	//�Ҷ��ļ�����begin
	ifstream mFinGray(tGrayFilePath.c_str());
	if (!mFinGray.is_open())
	{
		cout << "Error opening file"<<endl;
	}

	double mNumGray = 0.0;
	long mOkID = 0;
	getline(mFinGray, s);//������Ҫ������
	getline(mFinGray, s);//������Ҫ������
	getline(mFinGray, s);//������Ҫ������
	while (getline(mFinGray, s) && !s.empty())
	{
		mOkID = atoi(s.substr(0, s.find_last_of(" ")).c_str()); 
		mNumGray = atof(s.substr(s.find_last_of(" ")).c_str());
		if (mNumGray > gStandNumOfGray)
		{
			gOKIDToGrayMap.insert(pair<long, long>(mOkID, mOkID));
		}
	}
	mFinGray.close();
	//�Ҷ��ļ�����end

	//���ϻҶ�Ҫ������е������Լ������ĸ�����ID�ļ�����begin
	ifstream mFinIDPoints(tIDPointsFile.c_str());
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	long mIDOfPoint = 0;
	bool mTopData = true;

	double x = 0.0, y = 0.0, z = 0.0;
	int first = 0, second = 0, third = 0, fourth = 0;
	while (getline(mFinIDPoints, s))
	{
		if (s.find("ELEMENT") != string::npos)
		{
			mTopData = false;
		}
		else
		{
			//���з��ϻҶ�Ҫ�������
			if (mTopData)
			{
				mIDOfPoint = atoi(s.substr(0, s.find_first_of(",")).c_str());
				if (gOKIDToGrayMap.find(mIDOfPoint) != gOKIDToGrayMap.end())
				{
					s = s.substr(s.find_first_of(",") + 1);					

					x = atof(s.substr(0, s.find_first_of(",")).c_str());
					s = s.substr(s.find_first_of(",") + 1);

					y = atof(s.substr(0, s.find_first_of(",")).c_str());
					s = s.substr(s.find_first_of(",") + 1);

					z = atof(s.substr(0, s.find_first_of(",")).c_str());
					Point mOneP(x, y, z);
					gAllOkPointsMap.insert(pair<long, Point>(mIDOfPoint, mOneP));
				}
			}
			//������ĸ�����
			else
			{
				s = s.substr(s.find_first_of(",") + 1);

				first = atoi(s.substr(0, s.find_first_of(",")).c_str());
				s = s.substr(s.find_first_of(",") + 1);

				second = atoi(s.substr(0, s.find_first_of(",")).c_str());
				s = s.substr(s.find_first_of(",") + 1);

				third = atoi(s.substr(0, s.find_first_of(",")).c_str());
				s = s.substr(s.find_first_of(",") + 1);

				fourth = atoi(s.substr(0, s.find_first_of(",")).c_str());

				Grid mOneGrid;
				mOneGrid.first = first;
				mOneGrid.second = second;
				mOneGrid.third = third;
				mOneGrid.fourth = fourth;
				gAllGridLt.push_back(mOneGrid);
			}
		}
	}
	//���ϻҶ�Ҫ������е������Լ������ĸ�����ID�ļ�����end
}

void PriOperationData::GetTopSidePoint(Point *result)
{
	result->SetPoint(gTopSidePoint.GetX(), gTopSidePoint.GetY(), gTopSidePoint.GetZ());
}

void PriOperationData::DetachSolids()
{
	list<long>::iterator mSpeIDOfPointToSolidLtIter = this->gSpeIDOfPointToSolidLt.begin();
	while (mSpeIDOfPointToSolidLtIter != this->gSpeIDOfPointToSolidLt.end())
	{
		long mSpecialIDOfSolid = *mSpeIDOfPointToSolidLtIter;
		map<long, long> mHaveAddIDsMap; //�����жϵ�ǰID�Ƿ�����ӵ���©��		
		mHaveAddIDsMap.insert(pair<long, long>(mSpecialIDOfSolid, mSpecialIDOfSolid));
		
		list<long> mThisSolidIDLt;
		mThisSolidIDLt.push_back(mSpecialIDOfSolid);

		list<Grid>::iterator mAllGridLtIter;
		while (true)
		{
			long mAddID = 0;
			mAllGridLtIter = this->gAllGridLt.begin();
			while (mAllGridLtIter != this->gAllGridLt.end())
			{
				Grid& mTempGrid = *mAllGridLtIter;
				if (mHaveAddIDsMap.find(mTempGrid.first) != mHaveAddIDsMap.end())
				{
					long second = mTempGrid.second;
					if (gOKIDToGrayMap.find(second) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(second) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(second, second));
						mThisSolidIDLt.push_back(second);
						mAddID++;
					}

					long third = mTempGrid.third;
					if (gOKIDToGrayMap.find(third) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(third) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(third, third));
						mThisSolidIDLt.push_back(third);
						mAddID++;
					}

					long fourth = mTempGrid.fourth;
					if (gOKIDToGrayMap.find(fourth) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(fourth) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(fourth, fourth));
						mThisSolidIDLt.push_back(fourth);
						mAddID++;
					}
					//mAllGridLtIter++;
					mAllGridLtIter = this->gAllGridLt.erase(mAllGridLtIter);
				}
				else if (mHaveAddIDsMap.find(mTempGrid.second) != mHaveAddIDsMap.end())
				{
					long first = mTempGrid.first;
					if (gOKIDToGrayMap.find(first) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(first) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(first, first));
						mThisSolidIDLt.push_back(first);
						mAddID++;
					}

					long third = mTempGrid.third;
					if (gOKIDToGrayMap.find(third) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(third) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(third, third));
						mThisSolidIDLt.push_back(third);
						mAddID++;
					}

					long fourth = mTempGrid.fourth;
					if (gOKIDToGrayMap.find(fourth) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(fourth) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(fourth, fourth));
						mThisSolidIDLt.push_back(fourth);
						mAddID++;
					}
					//mAllGridLtIter++;
					mAllGridLtIter = this->gAllGridLt.erase(mAllGridLtIter);
				}
				else if(mHaveAddIDsMap.find(mTempGrid.third) != mHaveAddIDsMap.end())
				{
					long first = mTempGrid.first;
					if (gOKIDToGrayMap.find(first) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(first) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(first, first));
						mThisSolidIDLt.push_back(first);
						mAddID++;
					}

					long second = mTempGrid.second;
					if (gOKIDToGrayMap.find(second) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(second) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(second, second));
						mThisSolidIDLt.push_back(second);
						mAddID++;
					}

					long fourth = mTempGrid.fourth;
					if (gOKIDToGrayMap.find(fourth) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(fourth) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(fourth, fourth));
						mThisSolidIDLt.push_back(fourth);
						mAddID++;
					}
					//mAllGridLtIter++;
					mAllGridLtIter = this->gAllGridLt.erase(mAllGridLtIter);
				}
				else if (mHaveAddIDsMap.find(mTempGrid.fourth) != mHaveAddIDsMap.end())
				{
					long first = mTempGrid.first;
					if (gOKIDToGrayMap.find(first) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(first) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(first, first));
						mThisSolidIDLt.push_back(first);
						mAddID++;
					}

					long second = mTempGrid.second;
					if (gOKIDToGrayMap.find(second) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(second) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(second, second));
						mThisSolidIDLt.push_back(second);
						mAddID++;
					}

					long third = mTempGrid.third;
					if (gOKIDToGrayMap.find(third) != gOKIDToGrayMap.end() && mHaveAddIDsMap.find(third) == mHaveAddIDsMap.end())
					{
						mHaveAddIDsMap.insert(pair<long, long>(third, third));
						mThisSolidIDLt.push_back(third);
						mAddID++;
					}
					//mAllGridLtIter++;
					mAllGridLtIter = this->gAllGridLt.erase(mAllGridLtIter);
				}
				else
				{
					mAllGridLtIter++;
				}
			}
			if (mAddID == 0)
			{
				break;
			}
		}
		this->gSolidGroupMap.insert(pair<long, list<long>>(mSpecialIDOfSolid, mThisSolidIDLt));
		mSpeIDOfPointToSolidLtIter++;
	}

	mSpeIDOfPointToSolidLtIter = this->gSpeIDOfPointToSolidLt.begin();
	while (mSpeIDOfPointToSolidLtIter != this->gSpeIDOfPointToSolidLt.end())
	{
		long mSpecialIDOfSolid = *mSpeIDOfPointToSolidLtIter;
		list<long> mTempSolidIDsLt = this->gSolidGroupMap.at(mSpecialIDOfSolid);
		list<long>::iterator mTempSolidIDsLtIter = mTempSolidIDsLt.begin();
		list<Point> mTempSolidPointsLt;
		while (mTempSolidIDsLtIter != mTempSolidIDsLt.end())
		{
			if (this->gOKIDToGrayMap.find(*mTempSolidIDsLtIter) != this->gOKIDToGrayMap.end())
			{
				mTempSolidPointsLt.push_back(gAllOkPointsMap.at(*mTempSolidIDsLtIter));
			}			
			mTempSolidIDsLtIter++;
		}
		gPointOfAllSolidsResult.insert(pair<long, list<Point>>(mSpecialIDOfSolid, mTempSolidPointsLt));
		mSpeIDOfPointToSolidLtIter++;
	}
}


void PriOperationData::WriteToFile(string tFilePath)
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, tFilePath.c_str(), "wb");
	if (err == 0)
	{
		fclose(fp);
	}

	ofstream SaveFile(tFilePath.c_str(), ios::in);
	SaveFile << "That's new!\n";

	list<long>::iterator mgSpeIDOfPointToSolidLtIter = gSpeIDOfPointToSolidLt.begin();
	stringstream  mSStream;
	while (mgSpeIDOfPointToSolidLtIter != gSpeIDOfPointToSolidLt.end())
	{
		long mTempSpeID = *mgSpeIDOfPointToSolidLtIter;

		mSStream << (mMrByID.at(mTempSpeID));
		string mMr = mSStream.str();
		mSStream.str("");

		mSStream << (mRByID.at(mTempSpeID));
		string mR = mSStream.str();
		mSStream.str("");
		SaveFile << ("solid Mr=" + mMr + " R=" + mR + "\n");

		list<Point> mTempPointsLt = gPointOfAllSolidsResult.at(mTempSpeID);
		list<Point>::iterator mTempLtIter = mTempPointsLt.begin();
		while (mTempLtIter != mTempPointsLt.end())
		{
			SaveFile << "\t\tvertex " << (*mTempLtIter).GetX() << " " << (*mTempLtIter).GetY() << " " << (*mTempLtIter).GetZ() << "\n";
			mTempLtIter++;
		}
		SaveFile << "endsolidsolid\n";
		mgSpeIDOfPointToSolidLtIter++;
	}
	SaveFile.flush();
	SaveFile.close();
}


void PriOperationData::ThreeDDeal(string tIDPointsFile)
{
	string s;
	//���ϻҶ�Ҫ������е������Լ������ĸ�����ID�ļ�����begin
	ifstream mFinIDPoints(tIDPointsFile.c_str());
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������
	getline(mFinIDPoints, s);//������Ҫ������

	double xMax = 0.0, xMin = 0.0;
	double yMax = 0.0, yMin = 0.0;
	double zMax = 0.0, zMin = 0.0;

	double x = 0.0, y = 0.0, z = 0.0;
	while (getline(mFinIDPoints, s))
	{
		if (s.find("ELEMENT") != string::npos)
		{
			break;
		}
		else
		{
			s = s.substr(s.find_first_of(",") + 1);

			x = atof(s.substr(0, s.find_first_of(",")).c_str());
			if (x > xMax)
			{
				xMax = x;
			}
			else if (x < xMin)
			{
				xMin = x;
			}
			s = s.substr(s.find_first_of(",") + 1);

			y = atof(s.substr(0, s.find_first_of(",")).c_str());
			if (y > yMax)
			{
				yMax = y;
			}
			else if (y < yMin)
			{
				yMin = y;
			}
			s = s.substr(s.find_first_of(",") + 1);

			z = atof(s.substr(0, s.find_first_of(",")).c_str());
			if (z > zMax)
			{
				zMax = z;
			}
			else if (z < zMin)
			{
				zMin = z;
			}
			Point mOneP(x, y, z);
			gAllPointLt.push_back(mOneP);
		}
	}
	//���ϻҶ�Ҫ������е������Լ������ĸ�����ID�ļ�����end

	list<Point>::iterator mAllPointLtIter = gAllPointLt.begin();
	while (mAllPointLtIter != gAllPointLt.end())
	{
		Point& mOnePoint = *mAllPointLtIter;

		IntPoint mOneIntPoint;
		mOneIntPoint.x = round(mOnePoint.GetX() - xMin);
		mOneIntPoint.y = round(mOnePoint.GetY() - yMin);
		int mTempZ = round(mOnePoint.GetZ() - zMin);
		mOneIntPoint.z = mTempZ;

		if (gAllIntPointMap.find(mTempZ) == gAllIntPointMap.end())
		{
			list<IntPoint> mOneIntPointLt;
			mOneIntPointLt.push_back(mOneIntPoint);
			gAllIntPointMap.insert(pair<int, list<IntPoint>>(mTempZ, mOneIntPointLt));
		}
		else
		{
			list<IntPoint> &mOneIntPointLt = gAllIntPointMap[mTempZ];
			mOneIntPointLt.push_back(mOneIntPoint);
		}

		mAllPointLtIter++;
	}
}


void PriOperationData::ThreeDWriteToFile(string tFilePath)
{
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, tFilePath.c_str(), "wb");
	if (err == 0)
	{
		fclose(fp);
	}

	ofstream SaveFile(tFilePath.c_str(), ios::in);

	map<int, list<IntPoint>>::iterator mAllIntPointMapIter = gAllIntPointMap.begin();
	while (mAllIntPointMapIter != gAllIntPointMap.end())
	{
		int mZ = (*mAllIntPointMapIter).first;
		list<IntPoint>& mOneZIntPLt = (*mAllIntPointMapIter).second;
		list<IntPoint>::iterator mOneZIntPLtIter = mOneZIntPLt.begin();
		stringstream  mSStream;
		IntPoint& mOneIntPoint = *mOneZIntPLtIter;
		mSStream << mOneIntPoint.x;
		string mx = mSStream.str();
		mSStream.str("");

		mSStream << mOneIntPoint.y;
		string my = mSStream.str();
		mSStream.str("");

		SaveFile << (mx + "," + my);
		mOneZIntPLtIter++;

		while (mOneZIntPLtIter != mOneZIntPLt.end())
		{
			IntPoint& mOneIntPoint = *mOneZIntPLtIter;
			mSStream << mOneIntPoint.x;
			mx = mSStream.str();
			mSStream.str("");

			mSStream << mOneIntPoint.y;
			my = mSStream.str();
			mSStream.str("");

			SaveFile << (";" + mx + "," + my);
			mOneZIntPLtIter++;
		}
		mSStream << mZ;
		string mz = mSStream.str();
		mSStream.str("");
		SaveFile << ("$" + mz + "\r\n");

		mAllIntPointMapIter++;
	}
	SaveFile.flush();
	SaveFile.close();
}