#ifndef type_h
#define type_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

//单个数据
union dat {
	int i;
	float f;
	char c;
};

//数据类型
enum rua {
	INT, CHAR, FLOAT
};

//where类
class where{
public:
	string attr;
	int logic;
	string data;
};

//单个属性
class singleAttr {
public:
	string attrname;
	rua type;
	int length;
	bool isprimary;
	bool isunique;
	
	singleAttr() {
		attrname = "initial";
		type = INT;
		length = 1;
		isprimary = false;
		isunique = false;
	};
};

//属性表
class attrNode {
public:
	int attrnum;
	string attrname[32];
	rua type[32];
	int length[32];
	bool isPrimary[32];
	bool isUnique[32];
	bool hasIndex[32];
};

#endif
