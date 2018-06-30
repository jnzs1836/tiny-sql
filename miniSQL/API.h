#ifndef api_h
#define api_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include "type.h"
using namespace std;

//测试用头文件，里面是假装的接口
#include "test.h"

void API_Module(string SQL);
void read(string sql);
void insertValue(string sql);
void createTable(string sql);
void createIndex(string sql);
void select_deleteFrom(string sql, int type);
void dropTable(string sql);
void dropIndex(string sql);

vector<where> whereValue(string wherestring);
vector<singleAttr> attrValue(string attrstring);

#endif