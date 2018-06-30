#ifndef record_manager
#define record_manager

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "type.h"
#include "index_manager.h"
using namespace std;

//测试用头文件，里面是假装的接口
#include "test.h"

//自己加功能的属性类
class attrMine{
public:
	attrNode ad;

	//返回attr属性的编号
	int getNum(string attr);

	//返回第num个属性的偏移量
	int getOffset(int num);
	
	//返回一个元组的长度
	int totLength();
};

//Record Manager主类
class recordManager {
public:
	/*
	* --- 功能 ---
	* 创建一个新的表
	* --- 输入 ---
	* name: 表名
	* --- 报错 ---
	* 由catalog处理, 保证不存在该表后调用本函数
	*/
	void createTable(std::string name);

	/*
	* --- 功能 ---
	* 删除表
	* --- 输入 ---
	* name: 表名
	* --- 报错 ---
	* 由catalog处理, 保证表存在后调用本函数
	*/
	void dropTable(std::string name);

	/*
	* --- 功能 ---
	* 插入数据
	* --- 输入 ---
	* name:	表名
	* v:	数据元组(存放union的向量)
	* --- 返回 ---
	* 0: unique/primary有重复的值
	* 1: 插入成功
	*/
	int insertValue(string name, vector<dat> v);

	/*
	* --- 功能 ---
	* 删除数据
	* --- 输入 ---
	* name:	表名
	* w:	所有条件(存放where类的向量)
	* --- 返回 ---
	* 删除了多少条数据
	*/
	int deleteFrom(string name, vector<where> w);

	/*
	* --- 功能 ---
	* 选择数据
	* --- 输入 ---
	* name: 表名
	* w:	所有条件(存放where类的向量)
	* --- 返回 ---
	* 查询到了多少条数据
	*/
	int selectFrom(string name, vector<where> w);

	/*
	* --- 功能 ---
	* 创建索引
	* --- 输入 ---
	* name:	表名
	* attr:	属性名
	*/
	void createIndex(string name, string attr);

private:
	/*
	* --- 功能 ---
	* 从Buffer Manager中获取需要的block
	* --- 输入 ---
	* type: 0->空块 1->name表对应块
	* name: 表名
	*/
	dat* getBlock(int type, string name);

	/*
	* --- 功能 ---
	* 从Catalog Manager中获取name表对应的属性表
	* --- 输入 ---
	* name: 表名
	*/
	attrMine getAttribute(string name);

	/*
	* --- 功能 ---
	* 检查unique/primary
	* --- 输入 ---
	* block: 表对应的块
	* name:	 表名
	* atr:	 属性表
	* num:	 属性的编号
	* val:	 值
	*/
	bool checkUnique(dat* block, string name, attrMine atr, int num, vector<dat> val);

	/*
	* --- 功能 ---
	* 检查一条where
	* --- 输入 ---
	* block: 数据起始地址
	* tp：	 数据类型
	* len:	 数据长度(如果是char)
	* where: 单个条件
	*/
	bool checkWhere(dat *block, rua tp, int len,  where w);
	
	//比较模板
	template <typename T>
	inline bool compare(T a, T b, int loc)
	{
		switch (loc) {
		case 0:
			return a == b;
		case 1:
			return a != b;
		case 2:
			return a < b;
		case 3:
			return a > b;
		case 4:
			return a <= b;
		case 5:
			return a >= b;
		}
	}

	/*
	* --- 功能 ---
	* 输出select后的内容
	* --- 输入 ---
	* block: 数据起始地址
	* atr:	 属性表
	* cnt:	 数据量
	*/
	void outputSelect(dat *block, attrMine atr, int cnt);

	IndexManager::IndexManager im;
};

#endif