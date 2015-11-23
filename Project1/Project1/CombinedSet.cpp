#include "CombinedSet.h"
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <algorithm> 
using namespace std;


CombinedSet::CombinedSet(void)
{
}

CombinedSet::CombinedSet(int tElementCount)
{
	this->gElementCount = tElementCount;
}

CombinedSet::~CombinedSet(void)
{
}

void CombinedSet::SetPartition()
{
	int  *code, *maxi;       /* arrays for code[], maxi[]*/  
	int  i, j;
	code = (int *) malloc(sizeof(int) * gElementCount); /* get memory  */  
	maxi = (int *) malloc(sizeof(int) * gElementCount);  
  
	for (i = 0; i < gElementCount; i++)  /* initialization           */  
	{
		code[i] = 1;
		maxi[i] = 2;
	}

	while(1)
	{
		/* loop until done.         */
		list<int> mIntPointToLt;
		for (int x = 0; x < gElementCount; x++)
		{
			mIntPointToLt.push_back(*(code + x));
		}
		this->gResultVecLt.push_back(mIntPointToLt);
		//Display(code, n);   /* display one partition    */
		
		/*
		hhh
		*/
		//GroupForSolids(&this->gAllSolidVec, code, n);

		for (i = gElementCount-1; code[i] == maxi[i]; i--)
		{
			;/* find next 'increasible'  */
		}
		
		if (i > 0)
		{
			/* found ?                  */
			code[i] ++;     /* YES, update              */
			for (j = i + 1; j < gElementCount; j++)
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
	
void CombinedSet::Display(int* tCode)
{
	cout<<"{";
	for(int i = 0; i < gElementCount; i++)
	{
		if(i == gElementCount - 1)
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

void CombinedSet::Display()
{
	vector<list<int>>::iterator mIter = this->gResultVecLt.begin();
	while(mIter != this->gResultVecLt.end())
	{
		list<int> mOne = *mIter;
		list<int>::iterator mInerIter = mOne.begin();
		while (mInerIter != mOne.end())
		{
			cout << *mInerIter << "\t";
			mInerIter++;
		}
		cout<<endl;
		mIter ++;
	}
}

int GetGroupCount(list<int>& tData)
{
	int mMaxValue = 0;
	list<int>::iterator mLtIter = tData.begin();
	while (mLtIter != tData.end())
	{
		if (*mLtIter > mMaxValue)
		{
			mMaxValue = *mLtIter;
		}
		mLtIter++;
	}
	return mMaxValue;
}

bool SortElementCombined(list<int>& tFirst, list<int>& tSecond)
{
	return GetGroupCount(tFirst) < GetGroupCount(tSecond);
}


void CombinedSet::Sort()
{
	sort(gResultVecLt.begin(), gResultVecLt.end(), SortElementCombined);
}

vector<list<int>>* CombinedSet::GetCombinedSetVecP()
{
	return &(this->gResultVecLt);
}