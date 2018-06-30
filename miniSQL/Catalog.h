#ifndef __CATALOG__H
#define __CATALOG__H
#include "attrNode.h"
#include <string>
#include <vector>

//表节点
class cgTable{
public:
    //构造
    cgTable();
    cgTable(std::string tablename, std::vector<singleAttr> attrname, int recordnum){
        this->tableName = tablename;
        this->attrList = attrname;
        this->recordNum = recordnum;
    }
    //数据
    std::string tableName;
    std::vector<singleAttr> attrList;
    int recordNum;
    //输入
    friend std::istream& operator>>(std::istream& in, cgTable& node){
        int s;
        in >> node.tableName >> node.recordNum >> s;
        if (s < 0) return in;
        node.attrList.resize(s);
        for (std::vector<singleAttr>::iterator a = node.attrList.begin(); a != node.attrList.end(); ++a){
            in >> a->isunique >> a->isprimary >> a-> hasIndex >> a->length >> a->type >> a->attrname;
        }
        return in;
	}
    //输出
    friend std::ostream& operator<<(std::ostream& out, const cgTable& node){
        out << node.tableName << '\t' << node.recordNum << '\t' << node.attrList.size() << endl;
        for (vector<attrNode>::iterator a = node.attrList.begin(); a != node.attrList.end(); ++a){
            out << a->isunique << '\t' << a->isprimary << '\t' << a->hasIndex << '\t' << a->length << '\t' << a->type << '\t' << a->attrName << endl;
        }
        return out;
	}
}

//索引节点
class cgIndex{
public:
    //构造
    cgIndex();
    cgIndex(std::string tablename, std::string indexname, std::string attrname, int column){
        this->tableName = tablename;
        this->indexName = indexname;
        this->attribute = attrname;
        this->column = column;
    }
    //数据
    std::string tableName;
    std::string indexName;
    std::string attribute;
    int column;
    //输入
	friend std::istream& operator>>(std::istream& in, cgIndex& node){
        in >> node.tableName >> node.indexName >> node.attribute >> node.column;
        return in;
	}
    //输出
    friend std::ostream& operator<<(std::ostream& out, const cgIndex& node){
        out << node.tableName << '\t' << node.indexName << '\t' << node.attribute << '\t' << node.column << endl;
        return out;
	}
}

class Catalog{
private:
    std::vector<cgTable> tableList;
    std::vector<cgIndex> indexList;
public:
    Catalog();
    virtual ~Catalog();
    void cgRead();
    void cgWrite();
    //create命令
	bool createTable(string tablename, vector<singleAttr> attrlist);
	bool createIndex(string indexname, string tablename, int columns);//columns:第几列的属性需要index
	bool tableExist(string tablename);
	bool indexExist(string indexname);
	cgIndex findindex(string indexname, string tablename);
	//insert命令
    bool attrExist(string tablename, string attrname);
	bool attrUnique(string tablename, string attrname);
	int getAttrType(string tablename, string attrname);
	int getAttrLength(string tablename, string attrname);
	int getAttrNum(string tablename, string attrname);
    //获得表的attrlist
	vector<singleAttr> getAttrList(string tablename);
    //attrlist的内容
	vector<singleAttr> getAttrInfo(string tablename, string attrname);
    vector<string> getIndexOfTable(string tablename);
	string getIndexByAttrID(string tablename, int column);//若不存在返回值为""
	string getIndexFileByName(string indexname);
    int getRecordNum(string tablename);//返回条目数量
	//delete命令
    bool dropTable(string tablename);
	bool dropIndex(string indexname);
    bool deleteRecord(string tablename, int num);//删除条目数量，删除成功返回1
	bool addRec(string tablename, int num);
    //show命令
	void showTables();
	void showIndexes();
};
//canuse:
//giveIndex(name, num);

#endif