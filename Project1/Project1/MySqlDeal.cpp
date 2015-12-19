#include "MySqlDeal.h"
#include "stdio.h"
#include "winsock.h"
#include "mysql.h"
#include "Craft.h"


#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


MySqlDeal::MySqlDeal()
{
	string s;
	//漏洞中的一个特殊ID；还包括凝固时间文件操作begin
	ifstream mMysqlFile("source\\data\\mysql.txt");
	while (getline(mMysqlFile, s))
	{
		gMySqlUserName = s;
		getline(mMysqlFile, s);
		gMySqlPass = s;	
		break;
	}
	mMysqlFile.close();

	if (gMySqlUserName.length() == 0 || gMySqlPass.length() == 0)
	{
		gMySqlUserName = "root";
		gMySqlPass = "root";
	}
	gIP = "localhost";
	gDataBaseName = "testDaBao";
}

MySqlDeal::MySqlDeal(string tUserName, string tPass, string tBasePath)
{
	string mSavePath = tBasePath;
	mSavePath.append("mysql.txt");
	//put username and password into file, so that do other operation on mysql
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, mSavePath.c_str(), "wb");
	if (err == 0)
	{
		fclose(fp);
	}

	ofstream SaveFile(mSavePath.c_str(), ios::in);
	SaveFile << tUserName;
	SaveFile << "\n";
	SaveFile << tPass;
	SaveFile.flush();
	SaveFile.close();

	this->gMySqlUserName = tUserName;
	this->gMySqlPass = tPass;

	gIP = "localhost";
	gDataBaseName = "testDaBao";
}

MySqlDeal::~MySqlDeal()
{
}

int MySqlDeal::InitDataBase()
{
	int result = 0;
	char *query = NULL;
	int rt;//return value
	MYSQL_RES *res;

	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), NULL, 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			query = "set names \'UTF8\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
			res = mysql_store_result(con);//将结果保存在res结构体中
			mysql_free_result(res);
		}
		else
		{
			string mCreateDataBaseStr = "create database ";
			mCreateDataBaseStr.append(gDataBaseName);//
			mCreateDataBaseStr.append(" character set utf8;");

			mysql_query(con, mCreateDataBaseStr.c_str());
			res = mysql_store_result(con);//将结果保存在res结构体中
			mysql_free_result(res);
		}

		bool mNeedAddMaoKou = true;
		string mSelectMaoKouCmd = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'maokou' and TABLE_SCHEMA='";
		mSelectMaoKouCmd.append(gDataBaseName);
		mSelectMaoKouCmd.append("';");
		rt = mysql_real_query(con, mSelectMaoKouCmd.c_str(), strlen(mSelectMaoKouCmd.c_str()));
		if (rt)
		{			
			printf("Error making query: %s !!!\n", mysql_error(con));
		}
		res = mysql_store_result(con);//将结果保存在res结构体中
		while (row = mysql_fetch_row(res))
		{
			cout << row[0] << endl;
			mNeedAddMaoKou = false;
		}
		mysql_free_result(res);

		bool mNeeAddHaveDealCraft = true;
		string mSelectCraftCmd = "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_NAME = 'HaveDealCraft' and TABLE_SCHEMA='";
		mSelectCraftCmd.append(gDataBaseName);
		mSelectCraftCmd.append("';");
		rt = mysql_real_query(con, mSelectCraftCmd.c_str(), strlen(mSelectCraftCmd.c_str()));
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(con));
		}
		res = mysql_store_result(con);//将结果保存在res结构体中
		while (row = mysql_fetch_row(res))
		{
			cout << row[0] << endl;
			mNeeAddHaveDealCraft = false;
		}
		mysql_free_result(res);
		mysql_close(con);
		
		con = mysql_init((MYSQL*)0);
		if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
		{
			if (!mysql_select_db(con, gDataBaseName.c_str()))
			{
				query = "set names \'GBK\'";
				rt = mysql_real_query(con, query, strlen(query));
				if (rt)
				{
					printf("Error making query: %s !!!\n", mysql_error(con));
				}

				if (mNeedAddMaoKou)
				{
					string mCreateMaoKouCmd = "CREATE TABLE MaoKou(type int,Mr double,a int,b int,d int,h int,Vr double,Gr double,Vc double,Gc double,Percent double,FileIndex int); ";
					rt = mysql_real_query(con, mCreateMaoKouCmd.c_str(), strlen(mCreateMaoKouCmd.c_str()));\
					cout << "had add Maokou" << endl;
				}
				else
				{
					cout << "Don't need to add Maokou" << endl;
				}
				if (mNeeAddHaveDealCraft)
				{
					string mCreateMaoKouCmd = "CREATE TABLE HaveDealCraft(ID int primary key,CraftName varchar(100),BasePath varchar(200),MrFileIndex int,KeepWarm int,GrayStand int,MultiMrFileSearch bool,ProCheckState int,ManuCheckState int); ";
					rt = mysql_real_query(con, mCreateMaoKouCmd.c_str(), strlen(mCreateMaoKouCmd.c_str()));
					cout << "had add HaveDealCraft" << endl;
				}
				else
				{
					cout << "Don't need to add HaveDealCraft" << endl;
				}
			}
		}
		mysql_close(con);

		//system("pause");

		if (mNeedAddMaoKou)
		{
			string mRunJarStr = "java -jar source\\excelToMysql\\excelToMysql.jar ";
			mRunJarStr.append(gMySqlUserName);
			mRunJarStr.append(" ");
			mRunJarStr.append(gMySqlPass);
			mRunJarStr.append(" ");
			mRunJarStr.append(gDataBaseName);
			system(mRunJarStr.c_str());
			result = 1;
		}
		else if (mNeedAddMaoKou == false && mNeeAddHaveDealCraft == false)
		{
			result = 1;
		}
	}
	else
	{
		string m = mysql_error(con);
		cout << m << endl;
		printf("Unable to connect mysql,check your configuration!\n");
		mysql_close(con);
	}
	return result;
}

void MySqlDeal::SelectCmd(double tMr, int tFileIndex, bool tMultiFlag, string* tSqlCmd)
{
	list<int> mFileIndexLt;
	if(tMultiFlag)
	{
		switch (tFileIndex)
		{
		case 1:
		case 2:
			mFileIndexLt.push_back(1);
			mFileIndexLt.push_back(2);
			break;
		case 3:
		case 4:
			mFileIndexLt.push_back(3);
			mFileIndexLt.push_back(4);
			break;
		case 5:
		case 6:
			mFileIndexLt.push_back(5);
			mFileIndexLt.push_back(6);
			break;
		case 7:
		case 8:
			mFileIndexLt.push_back(7);
			mFileIndexLt.push_back(8);
			break;
		case 9:
			mFileIndexLt.push_back(9);
			break;
		case 10:
		case 11:
			mFileIndexLt.push_back(10);
			mFileIndexLt.push_back(11);
			break;
		default:
			break;
		}
	}
	else
	{
		mFileIndexLt.push_back(tFileIndex);
	}
	
	list<int>::iterator mFileIndexLtIter = mFileIndexLt.begin();

	stringstream ss;
	ss << tMr;
	string mStr = ss.str();
	*tSqlCmd = "SELECT DISTINCT Mr FROM maokou WHERE Mr >= ";
	*tSqlCmd += mStr;
	*tSqlCmd += " and (FileIndex = ";
	
	ss.str("");

	ss << *mFileIndexLtIter;
	mStr = ss.str();
	*tSqlCmd += mStr;
	
	mFileIndexLtIter++;
	while (mFileIndexLtIter != mFileIndexLt.end())
	{
		ss.str("");
		ss << *mFileIndexLtIter;
		mStr = ss.str();
		*tSqlCmd += (" or FileIndex = " + mStr);

		mFileIndexLtIter++;
	}
	*tSqlCmd += ") ORDER BY Mr ASC";
}

void MySqlDeal::SelectMaxMrCmd(int tFileIndex, bool tMultiFlag, string *tSqlCmd)
{
	list<int> mFileIndexLt;
	if (tMultiFlag)
	{
		switch (tFileIndex)
		{
		case 1:
		case 2:
			mFileIndexLt.push_back(1);
			mFileIndexLt.push_back(2);
			break;
		case 3:
		case 4:
			mFileIndexLt.push_back(3);
			mFileIndexLt.push_back(4);
			break;
		case 5:
		case 6:
			mFileIndexLt.push_back(5);
			mFileIndexLt.push_back(6);
			break;
		case 7:
		case 8:
			mFileIndexLt.push_back(7);
			mFileIndexLt.push_back(8);
			break;
		case 9:
			mFileIndexLt.push_back(9);
			break;
		case 10:
		case 11:
			mFileIndexLt.push_back(10);
			mFileIndexLt.push_back(11);
			break;
		default:
			break;
		}
	}
	else
	{
		mFileIndexLt.push_back(tFileIndex);
	}

	list<int>::iterator mFileIndexLtIter = mFileIndexLt.begin();

	stringstream ss;
	string mStr;

	*tSqlCmd = "SELECT DISTINCT Mr FROM maokou WHERE (FileIndex = ";

	ss << *mFileIndexLtIter;
	mStr = ss.str();
	*tSqlCmd += mStr;

	mFileIndexLtIter++;
	while (mFileIndexLtIter != mFileIndexLt.end())
	{
		ss.str("");
		ss << *mFileIndexLtIter;
		mStr = ss.str();
		*tSqlCmd += (" or FileIndex = " + mStr);

		mFileIndexLtIter++;
	}
	*tSqlCmd += ") ORDER BY Mr DESC LIMIT 0,1";

}

void MySqlDeal::GetDataBy(double tMr, int tFileIndex, bool tMultiFlag)
{
	//database configuartion
	char tablename[50] = "maokou";
	char *query = NULL;

	int rt;//return value
	
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			//printf("Select successfully the database!\n");
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}

	string mSqlCmd;
	SelectCmd(tMr, tFileIndex, tMultiFlag, &mSqlCmd);
	rt = mysql_real_query(con, mSqlCmd.c_str(), strlen(mSqlCmd.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}
	else
	{
		//printf("%s executed!!!\n", mSqlCmd.c_str());
	}

	res = mysql_store_result(con);//将结果保存在res结构体中
	
	this->gResult.clear();
	double mOneData = 0.0;
	while (row = mysql_fetch_row(res))
	{
		/**
		* MYSQL_ROW STDCALL mysql_fetch_row(MYSQL_RES *result);
		* 检索行
		*/
		mOneData = atof(row[0]);
		gResult.push_back(mOneData);
	}
	mysql_free_result(res);
	mysql_close(con);
}

void MySqlDeal::DisplayResult()
{
	list<double>::iterator mResIter = this->gResult.begin();
	while (mResIter != this->gResult.end())
	{
		cout << *mResIter << endl;
		mResIter++;
	}
}

list<double>* MySqlDeal::GetResultP()
{
	return &gResult;
}

double MySqlDeal::GetMaxMr(int tFileIndex, bool tMultiFlag)
{
	char tmp[400];

	//database configuartion
	char tablename[50] = "maokou";
	char *query = NULL;

	int rt;//return value

	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			//printf("Select successfully the database!\n");
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
			else
			{
				//printf("query %s succeed!\n", query);
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}

	string mSqlCmd;
	SelectMaxMrCmd(tFileIndex, tMultiFlag, &mSqlCmd);
	rt = mysql_real_query(con, mSqlCmd.c_str(), strlen(mSqlCmd.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}

	res = mysql_store_result(con);//将结果保存在res结构体中

	double mResult = 0.0;
	while (row = mysql_fetch_row(res))
	{
		/**
		* MYSQL_ROW STDCALL mysql_fetch_row(MYSQL_RES *result);
		* 检索行
		*/
		mResult = atof(row[0]);
	}
	mysql_free_result(res);
	mysql_close(con);

	return mResult;
}


void MySqlDeal::UnCheckCraftVector(vector<Craft>* tResult)
{
	//database configuartion
	char *query = NULL;

	int rt;//return value

	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			//printf("Select successfully the database!\n");
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}

	string mSqlCmd = "SELECT * FROM havedealcraft WHERE ProCheckState = 0 OR ManuCheckState = 0 LIMIT 0, 15";
	rt = mysql_real_query(con, mSqlCmd.c_str(), strlen(mSqlCmd.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}

	res = mysql_store_result(con);//将结果保存在res结构体中
	tResult->clear();
	while (row = mysql_fetch_row(res))
	{
		CraftInfor mOneCraftInfor;
		mOneCraftInfor.gKey = atoi(row[0]);
		mOneCraftInfor.gCraftName = row[1];
		mOneCraftInfor.gBasePath = row[2];
		mOneCraftInfor.gMrFileIndex = atoi(row[3]);
		mOneCraftInfor.gKeepWarm = atoi(row[4]);
		mOneCraftInfor.gGrayStand = atoi(row[5]);
		mOneCraftInfor.gMultiMrFileSear = atoi(row[6]);
		mOneCraftInfor.gProCheckState = atoi(row[7]);
		mOneCraftInfor.gManuCheckState = atoi(row[8]);

		Craft mOneCraft;
		mOneCraft.SetCraftInfor(&mOneCraftInfor);

		tResult->push_back(mOneCraft);
	}
	mysql_free_result(res);
	mysql_close(con);
}

void MySqlDeal::FuzzyQuery(string tCraftName, vector<Craft>* tResult)
{
	//database configuartion
	char *query = NULL;

	int rt;//return value

	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			//printf("Select successfully the database!\n");
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}

	string mSqlCmd = "SELECT * FROM havedealcraft WHERE CraftName LIKE '%";
	mSqlCmd.append(tCraftName);
	mSqlCmd.append("%' AND (ProCheckState = 0 OR ManuCheckState = 0) LIMIT 0, 15");

	cout << mSqlCmd << endl;

	rt = mysql_real_query(con, mSqlCmd.c_str(), strlen(mSqlCmd.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}

	res = mysql_store_result(con);//将结果保存在res结构体中
	tResult->clear();
	while (row = mysql_fetch_row(res))
	{
		CraftInfor mOneCraftInfor;
		mOneCraftInfor.gKey = atoi(row[0]);
		mOneCraftInfor.gCraftName = row[1];
		mOneCraftInfor.gBasePath = row[2];
		mOneCraftInfor.gMrFileIndex = atoi(row[3]);
		mOneCraftInfor.gKeepWarm = atoi(row[4]);
		mOneCraftInfor.gGrayStand = atoi(row[5]);
		mOneCraftInfor.gMultiMrFileSear = atoi(row[6]);
		mOneCraftInfor.gProCheckState = atoi(row[7]);
		mOneCraftInfor.gManuCheckState = atoi(row[8]);

		Craft mOneCraft;
		mOneCraft.SetCraftInfor(&mOneCraftInfor);

		tResult->push_back(mOneCraft);
	}
	mysql_free_result(res);
	mysql_close(con);
}

void MySqlDeal::UpdateState(CraftInfor* tData)
{
	char *query = NULL;

	int rt;//return value

	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}
	stringstream mSS;
	string mVaule;
	string mSqlCmd = "UPDATE havedealcraft SET ProCheckState = ";
	mSS << tData->gProCheckState;
	mVaule = mSS.str();
	mSqlCmd.append(mVaule);
	mSS.str("");
	mSqlCmd.append(",ManuCheckState = ");
	mSS << tData->gManuCheckState;
	mVaule = mSS.str();
	mSqlCmd.append(mVaule);
	mSS.str("");

	mSqlCmd.append(" WHERE ID = ");
	mSS << tData->gKey;
	mVaule = mSS.str();
	mSqlCmd.append(mVaule);
	mSS.str("");

	cout << mSqlCmd << endl;

	rt = mysql_real_query(con, mSqlCmd.c_str(), strlen(mSqlCmd.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}
	res = mysql_store_result(con);
	mysql_free_result(res);
	mysql_close(con);
}

void MySqlDeal::InsertCraft(CraftInfor* tData)
{
	char *query = NULL;
	int rt;//return value

	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}
	stringstream mSS;
	string mVaule;
	string mSqlCmd = "insert into HaveDealCraft values(";
	mSS << tData->gKey;
	mVaule = mSS.str();
	mSqlCmd.append(mVaule);
	mSS.str("");

	mSqlCmd.append(",'" + tData->gCraftName);
	mSqlCmd.append("','" + tData->gBasePath);

	mSS << tData->gMrFileIndex;
	mVaule = mSS.str();
	mSqlCmd.append("'," + mVaule);
	mSS.str("");

	mSS << tData->gKeepWarm;
	mVaule = mSS.str();
	mSqlCmd.append("," + mVaule);
	mSS.str("");

	mSS << tData->gGrayStand;
	mVaule = mSS.str();
	mSqlCmd.append("," + mVaule);
	mSS.str("");

	mSS << tData->gMultiMrFileSear;
	mVaule = mSS.str();
	mSqlCmd.append("," + mVaule);
	mSS.str("");

	mSqlCmd.append(",0,0)");

	cout << mSqlCmd << endl;

	rt = mysql_real_query(con, mSqlCmd.c_str(), strlen(mSqlCmd.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}
	res = mysql_store_result(con);
	mysql_free_result(res);
	mysql_close(con);
}

int MySqlDeal::GetCraftCount()
{
	int result = 0;
	char *query = NULL;
	int rt;//return value

	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, gIP.c_str(), gMySqlUserName.c_str(), gMySqlPass.c_str(), gDataBaseName.c_str(), 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, gDataBaseName.c_str()))
		{
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}

	string mSqlCmd = "SELECT COUNT(*) as amount FROM havedealcraft";
	rt = mysql_real_query(con, mSqlCmd.c_str(), strlen(mSqlCmd.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}
	res = mysql_store_result(con);
	while (row = mysql_fetch_row(res))
	{
		result = atoi(row[0]);
	}
	mysql_free_result(res);
	mysql_close(con);
	return result;
}