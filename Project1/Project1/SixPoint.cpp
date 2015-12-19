#include "SixPoint.h"
#include "Point.h"
#include <math.h>
#include <list>
#include <sstream>
#include <iostream>
#include <string>
using namespace std;

SixPoint::SixPoint(void)
{
	gAllSolidIDStr = "";
	this->gAllSolidVec.clear();
}

SixPoint::~SixPoint(void)
{
	
}

void SixPoint::GetCenter(Point* mResult)
{
	double mX = (this->gXMax->GetX() + this->gXMin->GetX()) / 2;
	double mY = (this->gYMax->GetY() + this->gYMax->GetY()) / 2;
	double mZ = (this->gZMax->GetZ() + this->gZMax->GetZ()) / 2;
	mResult->SetPoint(mX, mY, mZ);
	cout<<"x = "<<mX << "\ty = "<<mY << "\tz = "<< mZ<<endl;
}

const double SixPoint::GetRadius()
{
	double mRadius = 0.0f;
	double mX = this->gXMax->GetX() - this->gXMin->GetX();
	double mY = this->gYMax->GetY() - this->gYMax->GetY();
	double mZ = this->gZMax->GetZ() - this->gZMax->GetZ();
	mRadius = sqrt(mX * mX + mY * mY + mZ * mZ) / 2;
	cout<<"Radius : "<<mRadius<<endl;
	return mRadius;
}

void SixPoint::CompareForSixPoint(list<Point>* tAllPointList)
{
	list<Point>::iterator mPointLtIter = tAllPointList->begin();
	
	int mSolidID = 0;
	Point& mFirstPoint = *mPointLtIter;
	this->gXMax = &mFirstPoint;
	this->gXMin = &mFirstPoint;
	this->gYMax = &mFirstPoint;
	this->gYMin = &mFirstPoint;
	this->gZMax = &mFirstPoint;
	this->gZMin = &mFirstPoint;
	mPointLtIter ++;

	stringstream sstr;
	string str; 
	sstr << mSolidID; 
	sstr >> str; 
	this->gAllSolidIDStr.append(str.append(","));
	sstr.str("");

	while(mPointLtIter != tAllPointList->end())
	{
		Point& mOnePoint = *mPointLtIter;
		if(mOnePoint.GetID() != mSolidID)
		{
			this->gAllSolidVec.push_back(*gXMax);
			this->gAllSolidVec.push_back(*gXMin);
			this->gAllSolidVec.push_back(*gYMax);
			this->gAllSolidVec.push_back(*gYMin);
			this->gAllSolidVec.push_back(*gZMax);
			this->gAllSolidVec.push_back(*gZMin);

			this->gXMax = &mOnePoint;
			this->gXMin = &mOnePoint;
			this->gYMax = &mOnePoint;
			this->gYMin = &mOnePoint;
			this->gZMax = &mOnePoint;
			this->gZMin = &mOnePoint;
			mSolidID = mOnePoint.GetID();
		}
		else
		{
			if(this->gXMax->GetX() < mOnePoint.GetX())
			{
				this->gXMax = &mOnePoint;
			}
			else if(this->gXMin->GetX() > mOnePoint.GetX())
			{
				this->gXMin = &mOnePoint;
			}

			if(this->gYMax->GetY() < mOnePoint.GetY())
			{
				this->gYMax = &mOnePoint;
			}
			else if(this->gYMin->GetY() > mOnePoint.GetY())
			{
				this->gYMin = &mOnePoint;
			}

			if(this->gZMax->GetZ() < mOnePoint.GetZ())
			{
				this->gZMax = &mOnePoint;
			}
			else if(this->gZMin->GetZ() > mOnePoint.GetZ())
			{
				this->gZMin = &mOnePoint;
			}
		}
		mPointLtIter ++;
	}
	this->gAllSolidVec.push_back(*gXMax);
	this->gAllSolidVec.push_back(*gXMin);
	this->gAllSolidVec.push_back(*gYMax);
	this->gAllSolidVec.push_back(*gYMin);
	this->gAllSolidVec.push_back(*gZMax);
	this->gAllSolidVec.push_back(*gZMin);
}

void SixPoint::DisplayPointOfSolid()
{
	vector<Point>::iterator mTempIter = this->gAllSolidVec.begin();
	int i = 0;
	while(mTempIter != this->gAllSolidVec.end())
	{
		i ++;
		Point& mOne = *(mTempIter);
		mOne.PrintInfor();
		mTempIter ++;
		if(i == 6)
		{
			cout<<endl;
			i = 0;
		}
	}

}

void SixPoint::SetPartition(int n)
{
	int  *code, *maxi;       /* arrays for code[], maxi[]*/  
	int  i, j;
	code = (int *) malloc(sizeof(int) * n); /* get memory  */  
	maxi = (int *) malloc(sizeof(int) * n);  
  
	for (i = 0; i < n; i++)  /* initialization           */  
	{
		code[i] = 1;
		maxi[i] = 2;
	}
	while(1)
	{
		/* loop until done.         */
		
		//Display(code, n);   /* display one partition    */

		/*
		hhh

		*/
		GroupForSolids(&this->gAllSolidVec, code, n);

		for (i = n-1; code[i] == maxi[i]; i--)
		{
			;/* find next 'increasible'  */
		}
		
		if (i > 0)
		{
			/* found ?                  */
			code[i] ++;     /* YES, update              */
			for (j = i + 1; j < n; j++)
			{
				code[j] = 1;
				maxi[j] = maxi[i] + ((code[i] == maxi[i]) ? 1 : 0);
			}
		}
		else                /* NOT FOUND, done.         */
		{
			break;
		}
	}
	free(code);
	free(maxi);
}
	
void SixPoint::Display(int* tCode, int tNum)
{
	cout<<"{";
	for(int i = 0; i < tNum; i++)
	{
		if(i == tNum - 1)
		{
			cout<<*(tCode + i);
		}
		else
		{
			cout<<*(tCode + i)<<",";
		}
	}
	cout<<"}"<<endl;
}


/*
给定一个缺陷，找到其对应的圆心和半径
其中tPointsList对应此缺陷的点
*/
void SixPoint::CellForSolid(list<Point>* tPointsList)
{
	list<Point>::iterator mPointLtIter = tPointsList->begin();
	
	Point& mFirstPoint = *mPointLtIter;
	this->gXMax = &mFirstPoint;
	this->gXMin = &mFirstPoint;
	this->gYMax = &mFirstPoint;
	this->gYMin = &mFirstPoint;
	this->gZMax = &mFirstPoint;
	this->gZMin = &mFirstPoint;
	mPointLtIter ++;

	while(mPointLtIter != tPointsList->end())
	{
		Point& mOnePoint = *mPointLtIter;
		if(this->gXMax->GetX() < mOnePoint.GetX())
		{
			this->gXMax = &mOnePoint;
		}
		else if(this->gXMin->GetX() > mOnePoint.GetX())
		{
			this->gXMin = &mOnePoint;
		}

		if(this->gYMax->GetY() < mOnePoint.GetY())
		{
			this->gYMax = &mOnePoint;
		}
		else if(this->gYMin->GetY() > mOnePoint.GetY())
		{
			this->gYMin = &mOnePoint;
		}

		if(this->gZMax->GetZ() < mOnePoint.GetZ())
		{
			this->gZMax = &mOnePoint;
		}
		else if(this->gZMin->GetZ() > mOnePoint.GetZ())
		{
			this->gZMin = &mOnePoint;
		}
		mPointLtIter ++;
	}

	Point* mReultPointObj = new Point;
	GetCenter(mReultPointObj);
	GetRadius();
	delete mReultPointObj;
}

void SixPoint::GroupForSolids(vector<Point>* tAllSolidPointVec, int* tGroupCode, int tNumOfCode)
{
	int* mTempCode;
	mTempCode = (int *) malloc(sizeof(int) * tNumOfCode);
	for(int x = 0; x < tNumOfCode; x ++)
	{
		*(mTempCode + x) = *(tGroupCode + x);
	}

	const int PointNumOfSolid = 6;
	list<Point> mOneSolidLt;
	int mIndex = -1;
	vector<Point>::iterator mAllPointIter = tAllSolidPointVec->begin();
	int mOldIndex = -1;
	int mNegativeOneNum = 0;
	while(mNegativeOneNum < tNumOfCode)
	{
		string mSolidInforStr = "";
		mNegativeOneNum = 0;
		for(int x = 0; x < tNumOfCode; x ++)
		{
			if(*(mTempCode + x) != -1)
			{
				if(mIndex == *(mTempCode + x))
				{
					*(mTempCode + x) = -1;
					for(int y = 0; y < PointNumOfSolid; y ++)
					{
						mOneSolidLt.push_back(mAllPointIter[x * PointNumOfSolid + y]);
					}

					stringstream ss;
					ss<< (x + 1);
					mSolidInforStr.append(ss.str().append(","));
					ss.str("");
				}
				else
				{
					mOldIndex = mIndex;
					mIndex = *(mTempCode + x);
					if(mOldIndex == -1)
					{
						*(mTempCode + x) = -1;
						for(int y = 0; y < PointNumOfSolid; y ++)
						{
							mOneSolidLt.push_back(mAllPointIter[x * PointNumOfSolid + y]);
						}
						stringstream ss;
						ss<< (x + 1);
						mSolidInforStr.append(ss.str().append(","));
						ss.str("");
					}
					if(mOldIndex != -1 && mOldIndex != mIndex)
					{
						break;
					}					
				}
			}
			else
			{
				mNegativeOneNum ++;
			}
		}
		if(mOneSolidLt.size() != 0)
		{
			list<Point>::iterator mIter = mOneSolidLt.begin();
			while(mIter != mOneSolidLt.end())
			{
				Point& mOne = *mIter;
				mOne.PrintInfor();
				mIter ++;
			}
			if(mSolidInforStr.size() > 0)
			{
				cout<<"the combination of {"<<mSolidInforStr.substr(0, mSolidInforStr.size() - 1)<<"} come into being the ball:"<<endl;
			}
			CellForSolid(&mOneSolidLt);
			mOneSolidLt.clear();
			cout<<endl;
		}
	}

	free(mTempCode);
	cout<<"***********************************************************"<<endl;
}

