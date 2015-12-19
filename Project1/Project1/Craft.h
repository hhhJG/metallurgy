#pragma once
#include "Solution.h"


#include <string>
#include <list>
using namespace std;

struct CraftInfor
{
	int gKey; //工艺在数据库中的唯一ID
	int gProCheckState; //程序检验状态
	int gManuCheckState; //工艺生产检验状态

	string gCraftName;

	string gBasePath = "";

	string gDetailMsgFilePath; // 关于工艺的详细信息文件路径
	string gSTLFilePath; //用于前端三维展示的stl文件路径
	string gGrayFilePath; //灰度文件的ntl文件
	string gIDandTimeFilePath; //ID和时间txt文件路径
	string gAllPointFilePath; //所有坐标点以及网络的inp文件路径
	string g3DFilePath; //3D文件路径txt-------------
	string gSolutionFilePath; //运算结果txt-------------
	string gBadSolidFilePath; //标准缺陷文件路径txt-------------

	int gMrFileIndex; //冒口类型
	int gKeepWarm = 1; //是否是保温冒口
	int gGrayStand = 10; // 灰度标准，即达到什么标准才算是漏洞
	bool gMultiMrFileSear = true; //是否多个文件检索Mr
};


/**
一个工艺件
*/
class Craft
{
private:
	CraftInfor gCraftInfor;
	list<string> gCraftDetailMsgLt; //工艺的详细参数，如名称，材质，温度……
	vector<vector<Riser>> gAllPossAnswers;
public:
	Craft();
	~Craft();

	void SetCraftDetailMsg();
	list<string>* GetCraftDetailMsg();

	CraftInfor* GetCraftInfor();
	
	void SetAllPossAnswers();
	vector<vector<Riser>> *GetAllPossAnswers();

	void SetCraftInfor(CraftInfor* tCraftInfor);
	bool UpdateCheckState();
	bool Execute();

	void DisplayInfor();
};

