#ifndef testing_233
#define testing_233

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include "type.h"
using namespace std;

//假装buffer的打开文件函数
dat* callBuffer(string name);
//假装buffer写入
void bufferFresh();
//假装catalog返回一个属性表
void createCatalog(string name, vector<singleAttr> sa);
attrNode callCatalog(string name);
void giveIndex(string name, int num);
//假的IndexManager
/*
class IndexManager {
public:
	void setName(string str, string attr);
	template <typename T>
	void Insert(T key, int data) {
		cout << "insert " << key << ", " << data << endl;
		return;
	};

	template <typename T>
	void Delete(T key) {
		cout << "delete " << key << endl;
		return;
	};

	template <typename T>
	vector<int> Search(T key, int type) {
		cout << "search " << key << ", " << type << endl;
		vector<int> ret;
		ret.push_back(16);
		ret.push_back(2);
		ret.push_back(9);
		return ret;
	};
};
*/
#endif
