#include "API.h"
#include "interpreter.h"
#include "recordManager.h"
using namespace std;

void API_Module(string SQL)
{
	string schoice;
	string sql;
	int choice;
	schoice = SQL.substr(0, 2);
	sql = SQL.substr(2);
	choice = atoi(schoice.c_str());
	switch (choice) {
	case 0:
		createTable(sql);
		break;
	case 1:
		createIndex(sql);
		break;
	case 10:
		dropTable(sql);
		break;
	case 11:
		dropIndex(sql);
		break;
	case 20:
		select_deleteFrom(sql, 20);
		break;
	case 21:
		select_deleteFrom(sql, 21);
		break;
	case 30:
		insertValue(sql);
		break;
	case 40:
		select_deleteFrom(sql, 40);
		break;
	case 41:
		select_deleteFrom(sql, 41);
		break;
	case 50:
		read(sql);
		break;
	case 60:
		break;
	case 99:
		break;
	default:
		break;
	}
}

//读入文件执行
void read(string sql)
{
	char buffer[256];
	int f;
	string SQL;
	fstream out;
	out.open("./sql/" + sql, ios::in);
	if (!out.is_open()) {
		cout << "file error: no such file" << endl;
		return;
	}
	int statime = clock();
	while (!out.eof())
	{
		memset(buffer, 0, sizeof(buffer));
		f = -1;
		SQL.clear();
		do {
			out.getline(buffer, 256);//getline(char *,int,char) 表示该行字符达到256个或遇到换行就结束
			//cout << buffer << endl;
			if (buffer[0] != '\0')
				SQL = SQL + " " + buffer;
			f = SQL.find(';');
		} while (f == -1 && buffer[0] != '\0');

		while (f != -1) {
			f = SQL.find("\r");
			if (f != -1) {
				SQL.replace(f, 2, "  ");
			}
		}
		f = 0;
		while (f != -1) {
			f = SQL.find("\t");
			if (f != -1) {
				SQL.replace(f - 1, 2, "  ");
			}
		}

		f = SQL.find(';');
		if (f != -1)
			SQL.insert(f, " ");

		if (SQL != "") {
			SQL = Interpreter(SQL);
			//cout << SQL << endl;
			API_Module(SQL);
		}
	}
	int endtime = clock();
	cout << "used " << (double)(endtime - statime) / (double)CLOCKS_PER_SEC << " seconds" << endl;
	out.close();
	cin.get();
}

//插入数据
void insertValue(string sql)
{
	string snow = sql;

	int bp = snow.find(" ");
	string name = snow.substr(0, bp);
	snow = snow.substr(bp + 1);

	// !!!!!!改成和catalog manager的接口
	attrNode atr = callCatalog(name);

	vector<dat> val;
	for (int i = 0; i < atr.attrnum; i++) {
		if (atr.type[i] == CHAR) {
			bp = snow.find("\' ");
			if (bp != -1) bp++;
		}
		else bp = snow.find(" ");

		if (bp == -1) {
			if (i == atr.attrnum - 1) {
				bp = snow.length();
			}
			else {
				cout << "synax error: not enough input arguments!\n";
				return;
			}
		}

		string tmp = snow.substr(0, bp);
		if (i < atr.attrnum - 1) snow = snow.substr(bp + 1);

		//cout << tmp << " --- " << snow << endl;

		dat va;
		try {
			switch (atr.type[i]) {
			case INT: {
				va.i = stoi(tmp);
				val.push_back(va);
				//cout << va.i << " ";
				break;
			}
			case CHAR: {
				//开头和结尾不是单引号
				if (tmp[0] != '\'' || tmp[tmp.length() - 1] != '\'') {
					cout << "type error 0: invalid argument! " << tmp << endl;
					return;
				}
				string tt = tmp.substr(1, tmp.length() - 2);
				//输入的字符串太长
				if (tt.length() > atr.length[i]) {
					cout << "type error 1: the string is too long!" << endl;
					return;
				}

				for (int j = 0; j < atr.length[i]; j++) {
					if (j < tt.length()) {
						va.i = 0;
						va.c = tt[j];
						val.push_back(va);
					}
					else {
						va.i = 0;
						val.push_back(va);
					}
					//cout << va.i << " ";
				}
				break;
			}
			case FLOAT: {
				va.f = stof(tmp);
				val.push_back(va);
				//cout << va.f << " ";
				break;
			}
			}
		}
		catch (std::invalid_argument&) {
			cout << "type error 2: invalid argument!" << endl;
			return;
		}
		catch (std::out_of_range&) {
			cout << "type error 3: out of range!" << endl;
			return;
		}
	}

	//对接部分
	//cout << endl;
	recordManager rm;
	if (rm.insertValue(name, val)) {
		//cout << "query ok, 1 row affected" << endl;
	}
	else {
		cout << "insert error: duplicate entry for unique values" << endl;
	}
	//假装buffer写入
	//bufferFresh(name);
}

//创建表
void createTable(string sql)
{
	int start = 0, end;
	string tablename, attrstring;
	vector<singleAttr> sa;
	end = sql.find(',', start);
	tablename = sql.substr(start, end - start);
	start = end + 1;
	attrstring = sql.substr(start);
	sa = attrValue(attrstring);

	//调用record manager创建表
	recordManager rm;
	rm.createTable(tablename);
	// !!!!!!!!再加上给catalog的创建
	createCatalog(tablename, sa);
	// !!!!!!!!记得给primary key加索引?
	cout << "create table OK" << endl;
}

//创建索引
void createIndex(string sql)
{
	int start = 0, end;
	string indexname, tablename, attr;
	//indexname
	end = sql.find(' ', start);
	indexname = sql.substr(start, end - start);
	start = end + 1;
	//tablename
	while (sql[start] == ' ')
		start++;
	end = sql.find(' ', start);
	tablename = sql.substr(start, end - start);
	start = end + 1;
	//attr
	while (sql[start] == ' ')
		start++;
	end = sql.find(' ', start);
	attr = sql.substr(start, end - start);
	start = end + 1;

	// !!!!!!!!!indexname直接抛弃?
	recordManager rm;
	rm.createIndex(tablename, attr);
	cout << "create index OK" << endl;
}

//选择 & 删除
void select_deleteFrom(string sql, int type)
{
	int start = 0, end;
	string tablename, wherestring;
	vector<where> w;
	end = sql.find(',', start);
	tablename = sql.substr(start, end - start);

	recordManager rm;
	//没有where语句
	if (type == 40) {
		w.clear();
		
		//对接部分
		int cnt = rm.deleteFrom(tablename,w);
		cout << "query OK, " << cnt;
		if (cnt == 0 || cnt == 1) cout << " row affected" << endl;
		else cout << " rows affected" << endl;
		//假装刷新buffer
		//bufferFresh(tablename);
	}
	if (type == 20) {
		w.clear();
		
		//对接部分
		rm.selectFrom(tablename,w);
	}
	//有where语句
	else if (type == 41) {
		start = end + 1;
		wherestring = sql.substr(start);
		w = whereValue(wherestring);

		//对接部分
		int cnt = rm.deleteFrom(tablename, w);
		cout << "query OK, " << cnt;
		if (cnt == 0 || cnt == 1) cout << " row affected" << end;
		else cout << " rows affected" << endl;
		//假装刷新buffer
		//bufferFresh(tablename);
	}
	else if (type == 21) {
		start = end + 1;
		wherestring = sql.substr(start);
		w = whereValue(wherestring);

		//对接部分
		rm.selectFrom(tablename, w);
	}
}

//删除表
void dropTable(string sql)
{
	recordManager rm;
	rm.dropTable(sql);
	cout << "drop table OK" << endl;
}

//删除索引
void dropIndex(string sql)
{
	cout << "drop index OK" << endl;
}

//////////////////////////////////////////////////////
vector<where> whereValue(string wherestring)
{
	string single_where_clause;
	int start = 0, end;
	vector<where> w;
	stringstream ss;
	end = wherestring.find(',', start);
	while (end != -1) {
		where vv;
		int singlestart = 0, singleend;
		single_where_clause = wherestring.substr(start, end - start);
		start = end + 1;
		end = wherestring.find(',', start);

		singleend = single_where_clause.find(' ', singlestart);
		vv.attr = single_where_clause.substr(singlestart, singleend - singlestart);
		singlestart = singleend + 1;
		singleend = single_where_clause.find(' ', singlestart);
		ss << single_where_clause.substr(singlestart, singleend - singlestart);
		ss >> vv.logic;
		singlestart = singleend + 1;
		singleend = single_where_clause.find(' ', singlestart);
		vv.data = single_where_clause.substr(singlestart, singleend - singlestart);
		vv.data.erase(remove(vv.data.begin(), vv.data.end(), '\''), vv.data.end());
		w.push_back(vv);
		ss.clear();
	}
	return w;
}

/////////////////////////////////////////////////////////
vector<singleAttr> attrValue(string attrstring)
{
	int start = 0, end;
	string single_attr_clause;
	string temp_attrname, temp_type, temp_unique;
	vector<singleAttr> sattr;
	stringstream ss;
	end = attrstring.find(',', start);
	while (end != -1) {
		singleAttr sa;
		int singlestart = 0, singleend;
		single_attr_clause = attrstring.substr(start, end - start);
		start = end + 1;
		end = attrstring.find(',', start);

		//暂存attrname
		singleend = single_attr_clause.find(' ', singlestart);
		temp_attrname = single_attr_clause.substr(singlestart, singleend - singlestart);
		singlestart = singleend + 1;

		//处理INT,FLOAT,CHAR,PRIMARY
		singleend = single_attr_clause.find(' ', singlestart);
		temp_type = single_attr_clause.substr(singlestart, singleend - singlestart);
		singlestart = singleend + 1;
		if (temp_type == "+") {
			sa.attrname = temp_attrname;
			sa.type = INT;
			sa.length = 1;
		}
		else if (temp_type == "-") {
			sa.attrname = temp_attrname;
			sa.type = FLOAT;
			sa.length = 1;
		}
		else if (temp_type == "#") {
			for (int i = 0; i<sattr.size(); i++) {
				if (sattr[i].attrname == temp_attrname)
					sattr[i].isprimary = true;
			}
			continue;
		}
		else {
			sa.attrname = temp_attrname;
			sa.type = CHAR;
			ss << temp_type;
			ss >> sa.length;
		}

		//判断unique
		singleend = single_attr_clause.find(' ', singlestart);
		temp_unique = single_attr_clause.substr(singlestart, singleend - singlestart);
		singlestart = singleend + 1;
		if (temp_unique == "0")
			sa.isunique = false;
		else if (temp_unique == "1")
			sa.isunique = true;

		sattr.push_back(sa);
		ss.clear();
	}
	return sattr;
}