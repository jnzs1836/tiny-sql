#include "catalog.h"
#include <iostream>
using  namespace;

Catalog::Catalog(){
    cgRead();
}

Catalog::~Catalog(){
    cgWrite();
}

void Catalog::cgRead(){
	fstream logfile;									//文件流打开
	logfile.open("table.inf", ios::in);					//打开table.inf的文件
	if (logfile.good()){
		string s;
		logfile >> s;
		if (s.length() != 0){
			logfile.seekg(ios::beg);
			try{
				copy(istream_iterator<cgTable>(logfile), istream_iterator<cgTable>(), back_inserter(this->tableList));
			}
			catch(const length_error& e){
				cerr << e.what() << endl;
			}
			logfile.close();
		}
	}
	logfile.open("index.inf", ios::in);
	if (logfile.good()){
		copy(istream_iterator<cgIndex>(logfile), istream_iterator<cgIndex>(), back_inserter(this->indexList));
		logfile.close();
	}
}

void Catalog::cgWrite(){
	fstream logfile;
	logfile.open("table.inf", ios::out);
	if (logfile.good()){
		copy(this->tableList.begin(), this->tableList.end(), ostream_iterator<cgTable>(logfile));
		logfile.close();
	}
	logfile.open("index.inf", ios::out);
	if (logfile.good()){
		copy(this->indexList.begin(), this->indexList.end(), ostream_iterator<cgIndex>(logfile));
		logfile.close();
	}
}

bool Catalog::createTable(string tablename, attrNode attrlist){
	fstream tablefile;
	tablefile.open(tablename, ios::in);												//只读打开文件
	if (tablefile.good()){															//如果存在
		return false;																//则返回false
	}else{
		tablefile.close();
		tablefile.open(tablename, ios::out);										//读写打开
		tablefile.close();
		cgTable node(tablename, attrlist, 0);
		this->tableList.push_back(node);
		return true;																//写入到文件中
	}
}

bool Catalog::createIndex(string indexname, string tablename, int columns){
	fstream indexfile, configfile;
	string filename = (tablename + "-" + indexname + ".index");
	indexfile.open(filename, ios::in);
	filename = (tablename + "-" + indexname + ".config");
	configfile.open(filename, ios::in);
	if (indexfile.good() && configfile.good()) return false;
	else{
		string attrname;
		vector<cgTable>::iterator t;
		vector<singleAttr>::iterator a;
		for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
			if (t->tableName == tablename){
				a = t->attrList.begin();
				for (int i = 0; i < columns; ++i)
					++a;
				attrname = a->attrname;
				a->hasIndex = true;//set hasIndex
				if (a->isPrimary)
					indexname = string("$") + indexname;
			}
		}
        string iName = (tablename + "-" + indexname + ".index");
        string cName = (tablename + "-" + indexname + ".config");
        fopen( iName.c_str(), "wb+" );
        fopen( cName.c_str(), "wb+" );
		cgIndex node(tablename, indexname, attrname, columns);
		this->indexList.push_back(node);
		return true;
	}
}

bool Catalog::tableExist(string tablename){
	vector<cgTable>::iterator i;
	for (i = this->tableList.begin(); i != this->tableList.end(); ++i){
		if (i->tableName == tablename) return true;
	}
	return false;
}

bool Catalog::indexExist(string indexname){
	vector<cgIndex>::iterator i;
	for (i = this->indexList.begin(); i != this->indexList.end(); ++i){
		if (i->indexName == indexname) return true;
	}
	return false;
}

cgIndex Catalog::findindex(string indexname, string tablename){
	vector<cgIndex>::iterator i;
	for (i = this->indexList.begin(); i != this->indexList.end(); ++i){
		if (i->indexName == indexname&&i->tableName == tablename)
			return *i;
	}
    return *i;
}

bool Catalog::attrExist(string tablename, string attrname){
	vector<cgTable>::iterator t;
	vector<AttrNode>::iterator a;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			for (a = t->attrList.begin(); a != t->attrList.end(); ++a){
				if (a->attrname == attrname)	return true;
			}
		}
	}
	return false;
}

bool Catalog::attrUnique(string tablename, string attrname){
	vector<cgTable>::iterator t;
	vector<singleAttr>::iterator a;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			for (a = t->attrList.begin(); a != t->attrList.end(); ++a){
				if (a->isUnique) return true;
				else return false;
			}
		}
	}
	return -1;
}

int Catalog::getAttrType(string tablename, string attrname){
	vector<cgTable>::iterator t;
	vector<singleAttr>::iterator a;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			for (a = t->attrList.begin(); a != t->attrList.end(); ++a){
				if (a->attrname == attrname)  return a->type;
			}
		}
	}
	return -1;
}

int Catalog::getAttrLength(string tablename, string attrname){
	vector<cgTable>::iterator t;
	vector<singleAttr>::iterator a;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
	 		for (a = t->attrList.begin(); a != t->attrList.end(); ++a){
				if (a->attrname == attrname)
					return a->length;
			}
		}
	}
	return -1;//not found
}

int Catalog::getAttrNum(string tablename, string attrname){
	vector<cgTable>::iterator t;
	vector<singleAttr>::iterator a;
	int num = 0;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			for (a = t->attrList.begin(); a != t->attrList.end(); ++a){
				if (a->attrname == attrname)
					return num;
				++num;
			}
		}
	}
	return -1;//not found;
}

vector<singleAttr> Catalog::getAttrList(string tablename){
	vector<cgTable>::iterator t;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			return t->attrList;
		}
	}
    return t->attrList;
}

vector<singleAttr> Catalog::getAttrInfo(string tablename, string attrname){
	vector<cgTable>::iterator t;
	vector<singleAttr>::iterator a;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			for (a = t->attrList.begin(); a != t->attrList.end(); ++a){
				if (a->attrname == attrname)
					return *a;
			}
		}
	}
    return *a;
}

vector<string> Catalog::getIndexOfTable(string tablename){
	vector<cgIndex>::iterator i;
	vector<string> sv;
	for (i = this->indexList.begin(); i != this->indexList.end(); ++i){
		if (i->tableName == tablename)sv.push_back(i->indexName);
	}
	return sv;
}

string Catalog::getIndexByAttrID(string tablename, int column){
	vector<cgIndex>::iterator i;
	for (i = this->indexList.begin(); i != this->indexList.end(); ++i){
		if (i->tableName == tablename && i->column == column)return tablename + "-" + indexname;
	}
	return "";
}

string Catalog::getIndexFileByIndexName(string indexname){
	vector<cgIndex>::iterator i;
	for (i = this->indexList.begin(); i != this->indexList.end(); ++i){
		if (i->indexName == indexname)
			return tablename + "-" + indexname;
	}
	return "";
}

int Catalog::getRecordNum(string tablename){
	vector<cgTable>::iterator t;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			return t->recordNum;
		}
	}
	return -1;
}


bool Catalog::dropTable(string tablename){
	vector<cgTable>::iterator t;
	for (t = this->tableList.begin(); t != this->tableList.end();){
		if (t->tableName == tablename){
			t = this->tableList.erase(t);
			string filename = tablename;
			remove(filename.c_str());
		}else{
			++t;
		}
	}
	vector<cgIndex>::iterator i;
	for (i = this->indexList.begin(); i != this->indexList.end();){
		if (i->tableName == tablename){
			string filename = (tablename + "-" + indexname + ".index");
			remove(filename.c_str());
			filename = (tablename + "-" + indexname + ".config");
			remove(filename.c_str());
			i = this->indexList.erase(i);
		}else{
			++i;
		}
	}
	return true;
}

bool Catalog::dropIndex(string indexname){
	vector<cgIndex>::iterator i;
	vector<cgTable>::iterator t;
	vector<singleAttr>::iterator a;
	for (i = this->indexList.begin(); i != this->indexList.end();){
		if (i->indexName == indexname){
			for (t = this->tableList.begin(); t != this->tableList.end(); ++t){//reset hasIndex
				if (t->tableName == i->tableName)
				{
					for (a = t->attrList.begin(); a != t->attrList.end(); ++a)
					{
						if (a->attrname == i->attribute)
							a->hasIndex = false;
					}
				}
			}
			string filename = (tablename + "-" + indexname + ".index");
			remove(filename.c_str());
			filename = (tablename + "-" + indexname + ".config");
			remove(filename.c_str());
			i = this->indexList.erase(i);
		}else{
			++i;
		}
	}
	return true;
}


bool Catalog::recordDelete(string tablename, int num){
	vector<cgTable>::iterator t;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			t->recordNum -= num;
			return true;
		}
	}
	return -1;//not found;
}
bool Catalog::addRec(string tablename, int num){
	vector<cgTable>::iterator t;
	for (t = this->tableList.begin(); t != this->tableList.end(); ++t){
		if (t->tableName == tablename){
			t->recordNum += num;
			return true;
		}
	}
	return -1;//not found;
}

string isTrue(bool x){
	if (x) return "true";
	else return "false";
}

void Catalog::showTables(){	
	for (vector<cgTable>::iterator t = this->tableList.begin(); t != this->tableList.end(); ++t){
		cout << "==============================================================================" << endl;
		int column = 0;
		cout << "table: " << t->tableName << endl;
		cout << endl;
		
		cout << "attribute" << '\t' << "Primary" << '\t' << "Unique" << '\t' << "Type" << "\t\t" << "index" << endl;
		cout << "------------------------------------------------------------------------------" << endl;
/**/		for (vector<singleAttr>::iterator a = t->attrList.begin(); a != t->attrList.end(); ++a){
			cout << a->attrname << (a->attrname.size() < 8 ? "\t" : "") <<  '\t' << isTrue(a->isPrimary) << '\t' << isTrue(a->isUnique) << '\t';
			switch (a->type){
				case 3:	cout << "float" << "\t\t"; break;
				case 1: cout << "int" <<"\t\t"; break;
				case 2:	cout << "char(" << a->length << ")" << '\t' << (a->length > 9 ? "" : "\t"); break;
			}
			if (a->hasIndex)	cout << getIndexByAttrID(t->tableName, column);
			else cout << "none";
			cout << endl;
			column++;
		}
		cout << "==============================================================================" << endl;
	}
	cout << endl;
}

void Catalog::showIndexes(){
	cout << "==============================================================================" << endl;
	cout << "index" << "\t\t" << "table" << "\t\t" << "attribute" << endl;
	cout << "------------------------------------------------------------------------------" << endl;
	for (vector<cgIndex>::iterator iter = this->indexList.begin(); iter != this->indexList.end(); ++iter){
		cout << iter->indexName << '\t' << (iter->indexName.size() >= 8 ? "" : "\t") << iter->tableName << '\t' << (iter->tableName.size() >= 8 ? "" : "\t") << iter->attribute << endl;
	}
	cout << "==============================================================================" << endl;
}