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

//��װbuffer�Ĵ��ļ�����
dat* callBuffer(string name);
//��װbufferд��
void bufferFresh();
//��װcatalog����һ�����Ա�
void createCatalog(string name, vector<singleAttr> sa);
attrNode callCatalog(string name);
void giveIndex(string name, int num);
//�ٵ�IndexManager
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
