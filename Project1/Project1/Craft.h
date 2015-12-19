#pragma once
#include "Solution.h"


#include <string>
#include <list>
using namespace std;

struct CraftInfor
{
	int gKey; //���������ݿ��е�ΨһID
	int gProCheckState; //�������״̬
	int gManuCheckState; //������������״̬

	string gCraftName;

	string gBasePath = "";

	string gDetailMsgFilePath; // ���ڹ��յ���ϸ��Ϣ�ļ�·��
	string gSTLFilePath; //����ǰ����άչʾ��stl�ļ�·��
	string gGrayFilePath; //�Ҷ��ļ���ntl�ļ�
	string gIDandTimeFilePath; //ID��ʱ��txt�ļ�·��
	string gAllPointFilePath; //����������Լ������inp�ļ�·��
	string g3DFilePath; //3D�ļ�·��txt-------------
	string gSolutionFilePath; //������txt-------------
	string gBadSolidFilePath; //��׼ȱ���ļ�·��txt-------------

	int gMrFileIndex; //ð������
	int gKeepWarm = 1; //�Ƿ��Ǳ���ð��
	int gGrayStand = 10; // �Ҷȱ�׼�����ﵽʲô��׼������©��
	bool gMultiMrFileSear = true; //�Ƿ����ļ�����Mr
};


/**
һ�����ռ�
*/
class Craft
{
private:
	CraftInfor gCraftInfor;
	list<string> gCraftDetailMsgLt; //���յ���ϸ�����������ƣ����ʣ��¶ȡ���
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

