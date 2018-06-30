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

//������ͷ�ļ��������Ǽ�װ�Ľӿ�
#include "test.h"

//�Լ��ӹ��ܵ�������
class attrMine{
public:
	attrNode ad;

	//����attr���Եı��
	int getNum(string attr);

	//���ص�num�����Ե�ƫ����
	int getOffset(int num);
	
	//����һ��Ԫ��ĳ���
	int totLength();
};

//Record Manager����
class recordManager {
public:
	/*
	* --- ���� ---
	* ����һ���µı�
	* --- ���� ---
	* name: ����
	* --- ���� ---
	* ��catalog����, ��֤�����ڸñ����ñ�����
	*/
	void createTable(std::string name);

	/*
	* --- ���� ---
	* ɾ����
	* --- ���� ---
	* name: ����
	* --- ���� ---
	* ��catalog����, ��֤����ں���ñ�����
	*/
	void dropTable(std::string name);

	/*
	* --- ���� ---
	* ��������
	* --- ���� ---
	* name:	����
	* v:	����Ԫ��(���union������)
	* --- ���� ---
	* 0: unique/primary���ظ���ֵ
	* 1: ����ɹ�
	*/
	int insertValue(string name, vector<dat> v);

	/*
	* --- ���� ---
	* ɾ������
	* --- ���� ---
	* name:	����
	* w:	��������(���where�������)
	* --- ���� ---
	* ɾ���˶���������
	*/
	int deleteFrom(string name, vector<where> w);

	/*
	* --- ���� ---
	* ѡ������
	* --- ���� ---
	* name: ����
	* w:	��������(���where�������)
	* --- ���� ---
	* ��ѯ���˶���������
	*/
	int selectFrom(string name, vector<where> w);

	/*
	* --- ���� ---
	* ��������
	* --- ���� ---
	* name:	����
	* attr:	������
	*/
	void createIndex(string name, string attr);

private:
	/*
	* --- ���� ---
	* ��Buffer Manager�л�ȡ��Ҫ��block
	* --- ���� ---
	* type: 0->�տ� 1->name���Ӧ��
	* name: ����
	*/
	dat* getBlock(int type, string name);

	/*
	* --- ���� ---
	* ��Catalog Manager�л�ȡname���Ӧ�����Ա�
	* --- ���� ---
	* name: ����
	*/
	attrMine getAttribute(string name);

	/*
	* --- ���� ---
	* ���unique/primary
	* --- ���� ---
	* block: ���Ӧ�Ŀ�
	* name:	 ����
	* atr:	 ���Ա�
	* num:	 ���Եı��
	* val:	 ֵ
	*/
	bool checkUnique(dat* block, string name, attrMine atr, int num, vector<dat> val);

	/*
	* --- ���� ---
	* ���һ��where
	* --- ���� ---
	* block: ������ʼ��ַ
	* tp��	 ��������
	* len:	 ���ݳ���(�����char)
	* where: ��������
	*/
	bool checkWhere(dat *block, rua tp, int len,  where w);
	
	//�Ƚ�ģ��
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
	* --- ���� ---
	* ���select�������
	* --- ���� ---
	* block: ������ʼ��ַ
	* atr:	 ���Ա�
	* cnt:	 ������
	*/
	void outputSelect(dat *block, attrMine atr, int cnt);

	IndexManager::IndexManager im;
};

#endif