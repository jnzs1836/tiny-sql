#include "test.h"

#define unitK 50000000
dat memory[unitK] = { 0 };
string tabnow;

dat* callBuffer(string name)
{
	if (tabnow != name) {
		bufferFresh();
		string dir = "./data/" + name;
		FILE *fp = fopen(dir.c_str(), "rb");
		//memset(memory, 0, unitK);
		fread(memory, 4, unitK, fp);
		fclose(fp);
		tabnow = name;
	}

	return memory;
}
void bufferFresh()
{
	if (!tabnow.empty()) {
		string dir = "./data/" + tabnow;
		FILE *fp = fopen(dir.c_str(), "wb");
		int len = memory[1].i + 1000;
		fwrite(memory, 4, len, fp);
		fclose(fp);
	}
}

map<string, attrNode> mp;
void createCatalog(string name, vector<singleAttr> sa)
{
	attrNode a;
	a.attrnum = sa.size();
	for (int i = 0; i < a.attrnum; i++) {
		a.attrname[i] = sa[i].attrname;
		a.type[i] = sa[i].type;
		a.length[i] = sa[i].length;
		a.isPrimary[i] = sa[i].isprimary;
		a.isUnique[i] = sa[i].isunique;
		a.hasIndex[i] = 0;
	}
	mp.insert(pair<string, attrNode>(name, a));
}

//假装catalog返回一个属性表
attrNode callCatalog(string name)
{
	/*a.attrnum = 3;
	a.attrname[0] = "int";
	a.attrname[1] = "char";
	a.attrname[2] = "float";
	a.type[0] = INT;
	a.type[1] = CHAR;
	a.type[2] = FLOAT;
	a.length[0] = 1;
	a.length[1] = 5;
	a.length[2] = 1;
	a.hasIndex[0] = ii[0];
	a.hasIndex[1] = ii[1];
	a.hasIndex[2] = ii[2];
	a.isUnique[0] = 1;
	a.isUnique[1] = 1;
	a.isUnique[2] = 1;
	return a;*/
	map<string, attrNode>::iterator it;
	it = mp.find(name);
	//cout << it->first << endl;
	return it->second;
}
void giveIndex(string name, int num)
{
	map<string, attrNode>::iterator it;
	it = mp.find(name);
	it->second.hasIndex[num] = 1;
}

//假的IndexManager
void IndexManager::setName(string str, string attr) 
{
	cout << "now is using " << str << endl;
	cout << "the attribute is " << attr << endl;
	return;
}