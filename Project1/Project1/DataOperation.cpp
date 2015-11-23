#include "DataOperation.h"
#include "Point.h"


#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;



DataOperation::DataOperation(void)
{
	gSolidCount = 0;
}

DataOperation::~DataOperation(void)
{
}

void DataOperation::LineToPoint(string t_lineStr, double* result)
{
	int mLastSpaceIndex = t_lineStr.find_last_of(" ");
	*(result + 2) = atof(t_lineStr.substr(mLastSpaceIndex).c_str());


	t_lineStr = t_lineStr.substr(0, mLastSpaceIndex);
	mLastSpaceIndex = t_lineStr.find_last_of(" ");
	*(result + 1) = atof(t_lineStr.substr(mLastSpaceIndex).c_str());

	t_lineStr = t_lineStr.substr(0, mLastSpaceIndex);
	mLastSpaceIndex = t_lineStr.find_last_of(" ");
	*result = atof(t_lineStr.substr(mLastSpaceIndex).c_str());
}


void DataOperation::LoadFile(const char* t_path)
{
	string mRowData;
	ifstream mNetFile;
	mNetFile.open(t_path, ios::in);
	if(mNetFile.fail())
	{
		cout<<"The File "<<t_path << "\t is not exist"<<endl;
	}
	else
	{
		int mSolidID = -1;
		while(getline(mNetFile, mRowData))
		{
			if(mRowData.find("solid") != string::npos && mRowData.find("endsolid") == string::npos)
			{
				mSolidID ++;
				gSolidCount ++;
				double mModulus = 1.0;
				Solid mOneSolidObj(mModulus, mSolidID);
				this->gAllSolidVec.push_back(mOneSolidObj);
			}
			/*若一个文件中含有多个漏洞，即有多个solid
			则可以通过改变Point的ID属性，进而实现标识
			*/
			else if(mRowData.find("vertex") != string::npos)
			{
				double* mOnePointArr = new double[3];
				LineToPoint(mRowData, mOnePointArr);
				Point mOnePointObj(*mOnePointArr, *(mOnePointArr+1), *(mOnePointArr+2), mSolidID);
				this->gAllSolidVec.at(mSolidID).AddPointToThisSolid(mOnePointObj);
				delete mOnePointArr;
			}
			else if(mRowData.find("endsolid") != string::npos)
			{
				this->gAllSolidVec.at(mSolidID).CompareForCuboid();
			}
		}
	}
}



void DataOperation::SaveFile(const char* t_path)
{

}

vector<Solid>* DataOperation::GetAllSolidVecP()
{
	return &this->gAllSolidVec;
}

const int DataOperation::GetSolidCount()
{
	return gSolidCount;
}

