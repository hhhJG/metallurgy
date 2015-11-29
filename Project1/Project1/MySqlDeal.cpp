#include "MySqlDeal.h"
#include "stdio.h"
#include "winsock.h"
#include "mysql.h"

#include <list>
#include <iostream>
using namespace std;


MySqlDeal::MySqlDeal()
{
}


MySqlDeal::~MySqlDeal()
{
}



void MySqlDeal::GetDataBy(double tMr, int tFileIndex)
{
	char tmp[400];

	//database configuartion
	char dbuser[30] = "root";
	char dbpasswd[30] = "root";
	char dbip[30] = "localhost";
	char dbname[50] = "deadheaddata";
	char tablename[50] = "maokou";
	char *query = NULL;

	int rt;//return value
	
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL * con = mysql_init((MYSQL*)0);
	if (con != NULL && mysql_real_connect(con, dbip, dbuser, dbpasswd, dbname, 3306/*TCP IP端口*/, NULL/*Unix Socket 连接类型*/, 0/*运行成ODBC数据库标志*/))
	{
		if (!mysql_select_db(con, dbname))
		{
			printf("Select successfully the database!\n");
			con->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(con, query, strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(con));
			}
			else
			{
				printf("query %s succeed!\n", query);
			}
		}
	}
	else
	{
		printf("Unable to connect the database,check your configuration!\n");
	}

	sprintf_s(tmp, "select * from %s %s %f %s %d", tablename, "where Mr =", tMr, "and FileIndex = ", tFileIndex);
	rt = mysql_real_query(con, tmp, strlen(tmp));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}
	else
	{
		printf("%s executed!!!\n", tmp);
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