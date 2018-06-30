#ifndef interpreter_h
#define interpreter_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "type.h"
using namespace std;

string Interpreter(string statement);
string read_input();
string create_clause(string SQL,int start);
string get_attribute(string temp,string sql);
string create_table(string SQL,int start);
string create_index(string SQL,int start);
string create_index_on(string SQL,int start,string sql);
string drop_clause(string SQL,int start);
string quit_clause(string SQL,int start);
string delete_clause(string SQL,int start);
string delete_from(string SQL,int start);
string where_clause(string SQL,int start,string sql);
string select_clause(string SQL,int start);
string select_from(string SQL,int start);
string insert_clause(string SQL,int start);
string insert_into(string SQL,int start);
string insert_into_values(string SQL,int start,string sql);
string execfile_clause(string SQL,int start);

#endif /* interpreter_h */
