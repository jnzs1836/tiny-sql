#include "API.h"
#include "interpreter.h"
#include "recordManager.h"
using namespace std;

/* --- 输出缓存 --- */
dat outputBuffer[50000000];

/* --- 自定义属性类 --- */

int attrMine::getNum(string attr)
{
	for (int i = 0; i < ad.attrnum; i++) {
		if (ad.attrname[i] == attr) {
			return i;
		}
	}
	return -1;
}

int attrMine::getOffset(int num)
{
	int ofs = 0;
	for (int i = 0; i < num; i++) {
		if (ad.type[i] == CHAR) ofs += ad.length[i];
		else ofs += 1;
	}
	return ofs;
}

int attrMine::totLength()
{
	int len = 0;
	for (int i = 0; i < ad.attrnum; i++) {
		if (ad.type[i] == CHAR) len += ad.length[i];
		else len += 1;
	}
	return len;
}

/* --- 内部函数 --- */

//获取block
dat *recordManager::getBlock(int type, string name)
{
	//调用buffer提供的函数，这里自己写一个进行模拟
	dat *block;
	if (type) block = (dat *)callBuffer(name);
	else block = outputBuffer;

	return block;
}

//获取属性表
attrMine recordManager::getAttribute(string name)
{
	attrMine ret;

	//调用catalog的函数，这里自己瞎模拟
	ret.ad = callCatalog(name);
	
	return ret;
}

//检查是否重复
bool recordManager::checkUnique(dat *block, string name, attrMine atr, int num, vector<dat> val)
{
	int nextSP = block[0].i;
	int finalTP = block[1].i;
	int ofs = atr.getOffset(num);
	int len = (atr.ad.type[num] == CHAR) ? atr.ad.length[num] : 1;
	int address = 2;
	int adder = atr.totLength();

	while (address == nextSP && address <= finalTP) {
		nextSP = block[nextSP].i;
		address += adder;
	}
	bool ret = 0;
	while (address <= finalTP) {
		// << address << " " << nextSP << " " << finalTP << endl;
		bool f = 1;
		for (int i = 0; i < (len < val.size() ? len : val.size()); i++) {
			if (block[address + ofs + i].i != val[i].i) {
				f = 0;
				break;
			}
		}

		if (f) {
			ret = 1;
			break;
		}

		address += adder;
		while (address == nextSP && address <= finalTP) {
			nextSP = block[nextSP].i;
			address += adder;
		}
	}

	return ret;
}

//检查单个条件
bool recordManager::checkWhere(dat *block, rua tp, int len, where w)
{
	switch (tp) {
	case CHAR: {
		string str;
		for (int i = 0; i < len; i++) {
			if (block[i].c) {
				str += block[i].c;
			}
		}
		//cout << str << " " << w.data << endl;
		return compare(str, w.data, w.logic);
	}
	case INT: {
		int cp = stoi(w.data);
		//cout << block[0].i << " " << cp << endl;
		return compare(block[0].i, cp, w.logic);
	}
	case FLOAT: {
		float cp = stof(w.data);
		//cout << block[0].f << " " << cp << endl;
		return compare(block[0].f, cp, w.logic);
	}
	}
}

//输出select结果
void recordManager::outputSelect(dat *block, attrMine atr, int cnt)
{
	cout << endl;
	int adder = atr.totLength();

	dat* preb = block;
	//计算每列长度
	int wid[32];
	for (int i = 0; i < atr.ad.attrnum; i++) {
		wid[i] = atr.ad.attrname[i].length();
	}
	for (int loop = 0; loop < cnt; loop++) {
		int sta = 0;
		for (int i = 0; i < atr.ad.attrnum; i++) {
			stringstream ss;
			string ww;
			switch (atr.ad.type[i]) {
			case INT:
				ss.clear();
				ss.str("");
				ss << block[sta].i;
				ww = ss.str();
				if (ww.length() > wid[i]) wid[i] = ww.length();

				sta++;
				break;
			case CHAR: {
				string tmp;
				for (int j = 0; j < atr.ad.length[i]; j++) {
					if (block[sta].c) {
						tmp += block[sta].c;
					}
					sta++;
				}
				if (tmp.length() > wid[i]) wid[i] = tmp.length();
				break;
			}
			case FLOAT:
				ss.clear();
				ss.str("");
				ss << block[sta].f;
				ww = ss.str();
				if (ww.length() > wid[i]) wid[i] = ww.length();

				sta++;
				break;
			}
		}
		block += adder;
	}
	block = preb;

	//输出表头
	cout << "+";
	for (int i = 0; i < atr.ad.attrnum; i++) {
		for (int j = 0; j < wid[i] + 2; j++) {
			cout << "-";
		}
		cout << "+";
	}
	cout << endl;

	bool isstart = 1;
	for (int i = 0; i < atr.ad.attrnum; i++) {
		if (isstart) {
			cout << "|";
			isstart = 0;
		}
		cout << " " << setw(wid[i]) << setiosflags(ios::left) << atr.ad.attrname[i] << " |";
	}
	cout << endl;

	cout << "+";
	for (int i = 0; i < atr.ad.attrnum; i++) {
		for (int j = 0; j < wid[i] + 2; j++) {
			cout << "-";
		}
		cout << "+";
	}
	cout << endl;

	//输出内容
	for (int loop = 0; loop < cnt; loop++) {
		isstart = 1;
		int sta = 0;
		for (int i = 0; i < atr.ad.attrnum; i++) {
			if (isstart) {
				cout << "|";
				isstart = 0;
			}

			switch (atr.ad.type[i]) {
			case INT:
				cout << " " << setw(wid[i]) << setiosflags(ios::left) << block[sta].i << " |";
				sta++;
				break;
			case CHAR: {
				string tmp;
				for (int j = 0; j < atr.ad.length[i]; j++) {
					if (block[sta].c) {
						tmp += block[sta].c;
					}
					sta++;
				}
				cout << " " << setw(wid[i]) << setiosflags(ios::left) << tmp << " |";
				break;
			}
			case FLOAT:
				cout << " " << setw(wid[i]) << setiosflags(ios::left) << block[sta].f << " |";
				sta++;
				break;
			}
		}
		cout << endl;
		block += adder;
	}

	cout << "+";
	for (int i = 0; i < atr.ad.attrnum; i++) {
		for (int j = 0; j < wid[i] + 2; j++) {
			cout << "-";
		}
		cout << "+";
	}
	cout << endl;
}

/* --- 外部函数 --- */

//创建表
void recordManager::createTable(string name)
{
	string dir = "./data/" + name;
	FILE *fp = fopen(dir.c_str(), "w");

	//表头第一个元素存下一个空行的位置
	//第二个元素存最后一条元组的位置
	dat u[2];
	u[0].i = 2;
	u[1].i = 0;
	fwrite((char *)u, 4, 2, fp);

	fclose(fp);
}

//删除表
void recordManager::dropTable(string name)
{
	string dir = "./data/" + name;
	remove(dir.c_str());
}

//插入数据 + index插入
int recordManager::insertValue(string name, vector<dat> v)
{
	dat *block = getBlock(1, name);
	attrMine atr = getAttribute(name);

	//测试数据是否有重复(primary key 或者 unique)
	int sta = 0;
	for (int i = 0; i < atr.ad.attrnum; i++) {
		int len = (atr.ad.type[i] == CHAR) ? atr.ad.length[i] : 1;

		vector<dat> tmp;
		for (int j = 0; j < len; j++) {
			tmp.push_back(v[sta]);
			sta++;
		}
		if (atr.ad.isPrimary[i] || atr.ad.isUnique[i]) {
			if (checkUnique(block, name, atr, i, tmp)) {
				return 0;
			}
		}
	}

	//进行插入
	int nextSP = block[0].i;
	int finalTP = block[1].i;
	if (nextSP > finalTP) {
		block[1].i = nextSP;
		block[0].i += atr.totLength();
	}
	else {
		block[0].i = block[nextSP].i;
	}
	for (int i = 0; i < atr.totLength(); i++) {
		block[nextSP + i].i = v[i].i;
	}

	//调用index的插入
	for (int i = 0; i < atr.ad.attrnum; i++) {
		if (atr.ad.hasIndex[i]) {
			im.setName(name, atr.ad.attrname[i]);
			
			int ofs = atr.getOffset(i);
			switch (atr.ad.type[i]) {
			case INT:
				im.Insert(block[nextSP + ofs].i, nextSP);
				break;
			case CHAR: {
				string tmp;
				for (int j = 0; j < atr.ad.length[i]; j++) {
					if (block[nextSP + ofs + j].c) {
						tmp += block[nextSP + ofs + j].c;
					}
				}
				im.Insert(tmp, nextSP);
				break;
			}
			case FLOAT:
				im.Insert(block[nextSP + ofs].f, nextSP);
				break;
			}
		}
	}

	return 1;
}

//删除数据 + index删除
int recordManager::deleteFrom(string name, vector<where> w)
{
	dat *block = getBlock(1, name);
	attrMine atr = getAttribute(name);
	int lastSP = 0;
	int nextSP = block[0].i;
	int lastTP = 0;
	int finalTP = block[1].i;

	int address = 2;
	int adder = atr.totLength();
	while (address == nextSP && address <= finalTP) {
		lastSP = nextSP;
		nextSP = block[nextSP].i;
		address += adder;
	}

	//cout << lastSP << " " << address << " " << nextSP << endl;

	int cnt = 0;
	while (address <= finalTP) {
		bool f = 1;
		for (int i = 0; i < w.size(); i++) {
			int num = atr.getNum(w[i].attr);
			int ofs = atr.getOffset(num);

			if (!checkWhere(block + address + ofs, atr.ad.type[num], atr.ad.length[num], w[i])) {
				f = 0;
				break;
			}
		}
		if (f) {
			//调用index的删除
			for (int i = 0; i < atr.ad.attrnum; i++) {
				if (atr.ad.hasIndex[i]) {
					im.setName(name, atr.ad.attrname[i]);

					int ofs = atr.getOffset(i);
					switch (atr.ad.type[i]) {
					case INT:
						im.Delete(block[address + ofs].i);
						break;
					case CHAR: {
						string tmp;
						for (int j = 0; j < atr.ad.length[i]; j++) {
							if (block[address + ofs + j].c) {
								tmp += block[address + ofs + j].c;
							}
						}
						im.Delete(tmp);
						break;
					}
					case FLOAT:
						im.Delete(block[address + ofs].f);
						break;
					}
				}
			}

			//cout << address << endl;
			block[lastSP].i = address;
			block[address].i = nextSP;
			lastSP = address;
			cnt++;
			if (address == finalTP) block[1].i = lastTP;
		}
		else {
			lastTP = address;
		}

		address += adder;
		while (address == nextSP && address <= finalTP) {
			lastSP = nextSP;
			nextSP = block[nextSP].i;
			address += adder;
		}
	}

	return cnt;
}

//选择数据 + index选择
int recordManager::selectFrom(string name, vector<where> w)
{
	dat *block = getBlock(1, name);
	attrMine atr = getAttribute(name);
	dat *output = getBlock(0, "");
	int nextSP = block[0].i;
	int finalTP = block[1].i;

	int address = 2;
	int adder = atr.totLength();

	while (address == nextSP && address <= finalTP) {
		nextSP = block[nextSP].i;
		address += adder;
	}

	//查看是否符合使用index的条件
	bool useIndex = 0;
	if (w.size() == 1) {
		if (w[0].logic != 1) {
			int num = atr.getNum(w[0].attr);
			if (atr.ad.hasIndex[num]) {
				useIndex = 1;
			}
		}
	}

	int cnt = 0;
	int outnow = 0;

	if (useIndex) {
		//使用index进行选择
		im.setName(name, w[0].attr);

		int num = atr.getNum(w[0].attr);
		vector<int> res;

		switch (atr.ad.type[num]) {
		case INT: {
			int cp = stoi(w[0].data);
			res = im.Search(cp, w[0].logic);
			break;
		}
		case CHAR: {
			res = im.Search(w[0].data, w[0].logic);
			break;
		}
		case FLOAT: {
			float cp = stof(w[0].data);
			res = im.Search(cp, w[0].logic);
			break;
		}
		}

		cnt = res.size();
		for (int i = 0; i < cnt; i++) {
			memcpy(output + outnow, block + res[i], sizeof(dat) * adder);
			outnow += adder;
		}
	}
	else {
		while (address <= finalTP) {
			bool f = 1;
			for (int i = 0; i < w.size(); i++) {
				int num = atr.getNum(w[i].attr);
				int ofs = atr.getOffset(num);

				if (!checkWhere(block + address + ofs, atr.ad.type[num], atr.ad.length[num], w[i])) {
					f = 0;
					break;
				}
			}
			if (f) {
				memcpy(output + outnow, block + address, sizeof(dat) * adder);
				cnt++;
				outnow += adder;
			}

			address += adder;
			while (address == nextSP && address <= finalTP) {
				nextSP = block[nextSP].i;
				address += adder;
			}
		}
	}
	
	if(cnt > 0) outputSelect(output, atr, cnt);
	else cout << "Empty set.\n";

	return cnt;
}

//创建index
void recordManager::createIndex(string name, string attr)
{
	
	
	//开始创建
	dat *block = getBlock(1, name);
	attrMine atr = getAttribute(name);
	int nextSP = block[0].i;
	int finalTP = block[1].i;

	int address = 2;
	int adder = atr.totLength();

	while (address == nextSP && address <= finalTP) {
		nextSP = block[nextSP].i;
		address += adder;
	}

	int num = atr.getNum(attr);
	int ofs = atr.getOffset(num);

	if (!atr.ad.isUnique[num]) {
		cout << "ERROR: index on a not unique attribute.\n";
		return;
	}
	
	im.createIndex(name + attr, atr.ad.type[num]);
	//设置当前表
	im.setName(name, attr);
	while (address <= finalTP) {
		switch (atr.ad.type[num]) {
		case INT:
			im.Insert(block[address + ofs].i, address);
			break;
		case CHAR: {
			string tmp;
			for (int j = 0; j < atr.ad.length[num]; j++) {
				if (block[address + ofs + j].c) {
					tmp += block[address + ofs + j].c;
				}
			}
			im.Insert(tmp, address);
			break;
		}
		case FLOAT:
			im.Insert(block[address + ofs].f, address);
			break;
		}

		address += adder;
		while (address == nextSP && address <= finalTP) {
			nextSP = block[nextSP].i;
			address += adder;
		}
	}

	//让catalog设置有hasIndex, 需要接口
	giveIndex(name, num);
}


//int main()
//{
//	recordManager rm;
//	vector<dat> v;
//
//	/*unsigned char buf[100] = { 0 };
//	dat* xx = (dat *)buf;
//	xx->i = 2;
//	xx++;
//	xx->i = 9;
//	xx++;
//
//	xx->i = 16;
//	xx+=7;
//
//	xx->i = 123576;
//	xx++;
//	xx->c = 'H';
//	xx++;
//	xx->c = 'e';
//	xx++;
//	xx->c = 'l';
//	xx++;
//	xx->c = 'l';
//	xx++;
//	xx->c = 'o';
//	xx++;
//	xx->f = 2.2;
//	FILE *yy = fopen("./data/test", "w");
//	fwrite(buf, 1, 64, yy);
//	fclose(yy);*/
//	
//	dat u;
//	u.i = 123;
//	v.push_back(u);
//	u.i = 'H';
//	v.push_back(u);
//	u.i = 'e';
//	v.push_back(u);
//	u.i = 'l';
//	v.push_back(u);
//	u.i = 'l';
//	v.push_back(u);
//	u.i = 'x';
//	v.push_back(u);
//	u.f = 5.2;
//	v.push_back(u);
//
//	rm.createIndex("test", "int");
//	rm.createIndex("test", "char");
//	rm.createIndex("test", "float");
//	//rm.createTable("2333");
//	cout << rm.insertValue("test", v) << endl;
//	//bufferFresh("test");
//
//	vector<where> vv, kong;
//	//cout << rm.deleteFrom("test", vv) << endl;
//	where w;
//	w.attr = "int";
//	w.logic = 4;
//	w.data = "1236";
//	vv.push_back(w);
//	w.attr = "int";
//	w.logic = 5;
//	w.data = "2.2";
//	kong.push_back(w);
//	cout << "\n--------- delete ---------\n" << rm.deleteFrom("test", vv) << "\n--------- ------ ---------\n" << endl;
//	bufferFresh("x1");
//	cout << "\n--------- select ---------\n" << rm.selectFrom("test", kong) << "\n--------- ------ ---------\n" << endl;
//
//	vv.clear();
//	w.attr = "char";
//	w.logic = 0;
//	w.data = "Hellx";
//	vv.push_back(w);
//	cout << "\n--------- delete ---------\n" << rm.deleteFrom("x1", vv) << "\n--------- ------ ---------\n" << endl;
//	bufferFresh("x2");
//	cout << "\n--------- select ---------\n" << rm.selectFrom("x1", kong) << "\n--------- ------ ---------\n" << endl;
//
//	vv.clear();
//	w.attr = "float";
//	w.logic = 1;
//	w.data = "1.1";
//	vv.push_back(w);
//	cout << "\n--------- delete ---------\n" << rm.deleteFrom("x2", vv) << "\n--------- ------ ---------\n" << endl;
//	bufferFresh("x3");
//	cout << "\n--------- select ---------\n" << rm.selectFrom("x2", kong) << "\n--------- ------ ---------\n" << endl;
//	cout << "\n--------- select ---------\n" << rm.selectFrom("x3", kong) << "\n--------- ------ ---------\n" << endl;
//
//}