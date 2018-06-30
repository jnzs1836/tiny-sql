#include "API.h"
#include "interpreter.h"
#include "recordManager.h"
using namespace std;

int main(int argc, const char * argv[]) {
    string SQL;
    cout << endl;
    cout << "\t\t***********************************************" << endl;
    cout << "\t\t           Welcome to MiniSQL !" << endl;
    cout << "\t\t              Version (1.0)  " << endl;
    cout << "\t\t         " << endl;
    cout << "\t\t     copyright(2018) all right reserved !" << endl;
    cout << "\t\t***********************************************" << endl;
	cout << endl;
    while (1)
    {
        cout << endl << " >> ";
        SQL.clear();
        SQL = Interpreter(SQL);
        //cout << SQL << endl;
		if (SQL == "60") {
			bufferFresh();
			break;
		}
        API_Module(SQL);
    }
}

string read_input(){
    string SQL;
    string temp;
    int start;
    char str[100];
    bool finish=false;
    while(!finish)
    {
        cin>>str;
        temp=str;
        SQL=SQL+" "+temp;
        if(SQL[SQL.length()-1]==';')
        {
            SQL[SQL.length()-1] = ' ';
            SQL+=";";
            finish=true;
        }
    }
    return SQL;
}

string Interpreter(string statement){
    string SQL;
    string temp;
    string sql;
    int start=0,end;
    if(statement.empty()){
        SQL=read_input();
    }
    
    else
        SQL = statement;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    
    start=end+1;
    if(temp==";")
    {
        cout<<"syntax error: empty statement!"<<endl;
        SQL="99";
    }
    else if(temp=="create")
        SQL=create_clause(SQL,start);

    else if(temp=="drop")
        SQL=drop_clause(SQL,start);
 
    else if(temp=="select")
        SQL=select_clause(SQL,start);

    else if(temp=="insert")
        SQL=insert_clause(SQL,start);

    else if(temp=="delete")
        SQL=delete_clause(SQL,start);


    else if(temp=="execfile")
        SQL=execfile_clause(SQL,start);
  

    else if(temp=="quit")
        SQL=quit_clause(SQL,start);


    else
    {
        cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
        SQL="99";
    }
    return SQL;
}
///////////////////////////////////////////////////////
//
//
//
//
//         CREATE
//
//
//
///////////////////////////////////////////////////////

string create_clause(string SQL,int start)
{
    string temp;
    int end;

    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"syntax error: syntax error for create statement!"<<endl;
        SQL="99";
    }

    else if(temp=="table")
        SQL=create_table(SQL,start);

    else if(temp=="index")
        SQL=create_index(SQL,start);

    else
    {
        cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
        SQL="99";
    }
 
    return SQL;
}
///////////////////////////////////////////////////////
//
//
//
//
//         CREATE TABLE
//
//
//
///////////////////////////////////////////////////////

string create_table(string SQL,int start)
{
    string temp,sql,T;
    int index,end,length;

    while(SQL[start]==' ')
        start++;
    index=start;
    if((end=SQL.find('(',start))==-1)
    {
        cout<<"error: missing ( in the statement!"<<endl;
        SQL="99";
        return SQL;
    }
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(!(temp==";"))
    {
        while(SQL[start]==' ')
            start++;
        length=temp.length()-1;
        while(temp[length]==' ')
            length--;
        temp=temp.substr(0,length+1);
    }

    if(temp==";")
    {
        cout<<"error: error in create table statement!"<<endl;
        SQL="99";
        return SQL;
    }
    
    else if(temp.find(' ')!=-1)
    {
        cout<<"error: "<<temp<<"---is not a valid table name!"<<endl;
        SQL="99";
        return SQL;
    }
    else
    {
        sql=temp+",";
    
        while((end=SQL.find(',',start))!=-1)
        {
            temp=SQL.substr(start,end-start);
            start=end+1;
     
            if(temp==";")
            {
                cout<<"error: error in create table statement!"<<endl;
                SQL="99";
                return SQL;
            }
    
            else
            {
                sql=get_attribute(temp,sql);
                if(sql=="99")
                    return sql;
            }
            while(SQL[start]==' ')
                start++;
        }

        temp=SQL.substr(start,SQL.length()-start-1);
        length=temp.length()-1;
        while(temp[length]!=')'&&length>=0)
            length--;

        if(length<1)
        {
            cout<<"error: error in create table statement!"<<endl;
            SQL="99";
            return SQL;
        }

        else
        {
            temp=temp.substr(0,length);
            end=SQL.find(' ',start);
            T=SQL.substr(start,end-start);
            start=end+1;
 
            if(T=="primary")
            {
             
                temp+=")";
                while(SQL[start]==' ')
                    start++;
                end=SQL.find('(',start);
                T=SQL.substr(start,end-start);
                start=end+1;
                length=T.length()-1;
                while(T[length]==' ')
                    length--;
                T=T.substr(0,length+1);
  
                if(T==";")
                {
                    cout<<"syntax error: syntax error in create table statement!"<<endl;
                    cout<<"\t missing key word key!"<<endl;
                    SQL="99";
                    return SQL;
                }

                else if(T=="key")
                {
         
                    while(SQL[start]==' ')
                        start++;
                    end=SQL.find(')',start);
                    T=SQL.substr(start,end-start);
                    length=T.length()-1;
                    while(T[length]==' ')
                        length--;
                    T=T.substr(0,length+1);
      
                    if(T==";")
                    {
                        cout<<"error : missing primary key attribute name!"<<endl;
                        SQL="99";
                        return SQL;
                    }
               
                    else if(T.find(' ')!=-1)
                    {
                        cout<<"error : "<<T<<"---is not a valid primary key attribute name!"<<endl;
                        SQL="99";
                        return SQL;
                    }
        
                    else
                    {
                        sql+=T+" #,";
                        SQL="00"+sql;
                    }
                }
              
                else
                {
                    cout<<"error : "<<T<<"---is not a valid key word!"<<endl;
                    SQL="99";
                    return SQL;
                }
            }
        
            else
            {
                sql=get_attribute(temp,sql);
                if(sql=="99")
                {
                    SQL="99";
                    return SQL;
                }
                else
                    SQL="00"+sql;
            }
        }
    }
    return SQL;
}
/////////////////////////////////////////////////////////////
string get_attribute(string temp,string sql)
{
    int start=0,end,index;
    string T,C;
    temp+=" ";

    end=temp.find(' ',start);
    T=temp.substr(start,end-start);
    start=end+1;
    sql+=T+" ";

    while(temp[start]==' ')
        start++;
    end=temp.find(' ',start);
    T=temp.substr(start,end-start);
    start=end+1;
  
    if(T==";")
    {
        cout<<"error : error in create table statement!"<<endl;
        sql="99";
        return sql;
    }

    else if(T=="int")
        sql+="+";

    else if(T=="float")
        sql+="-";

    else
    {
        index=T.find('(');
        C=T.substr(0,index);
        index++;

        if(C==";")
        {
            cout<<"error: "<<T<<"---is not a valid data type definition!"<<endl;
            sql="99";
            return sql;
        }
 
        else if(C=="char")
        {
            C=T.substr(index,T.length()-index-1);
            if(C==";")
            {
                cout<<"error: the length of the data type char has not been specified!"<<endl;
                sql="99";
                return sql;
            }
            else
                sql+=C;
        }
    
        else
        {
            cout<<"error: "<<C<<"---is not a valid key word!"<<endl;
            sql="99";
            return sql;
        }
    }
    
    while(start<temp.length()&&temp[start]==' ')
        start++;
    if(start<temp.length())
    {
   
        end=temp.find(' ',start);
        T=temp.substr(start,end-start);
        if(T=="unique")
        {
            sql+=" 1,";
        }
   
        else
        {
            cout<<"error: "<<temp<<"---is not a valid key word!"<<endl;
            sql="99";
            return sql;
        }
    }

    else
        sql+=" 0,";
    return sql;
}
///////////////////////////////////////////////////////
//
//
//
//
//         CREATE INDEX
//
//
//
///////////////////////////////////////////////////////
string create_index(string SQL,int start)
{
    string temp,sql;
    int end;

    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;

    if(temp==";")
    {
        cout<<"syntax error: syntax error for create index statement!"<<endl;
        SQL="99";
    }
    else
    {
        sql=temp;

        while(SQL[start]==' ')
            start++;
        end=SQL.find(' ',start);
        temp=SQL.substr(start,end-start);
        start=end+1;

        if(temp==";")
        {
            cout<<"syntax error: syntax error for create index statement!"<<endl;
            SQL="99";
        }
 
        else if(temp=="on")
            SQL=create_index_on(SQL,start,sql);
   
        else
        {
            cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
            SQL="99";
        }
    }
    return SQL;
}
///////////////////////////////////////////////////////
string create_index_on(string SQL,int start,string sql)
{
    string temp;
    int end,length;

    while(SQL[start]==' ')
        start++;
    end=SQL.find('(',start);
    temp=SQL.substr(start,end-start);
    start=end+1;

    if(end==-1)
    {
        cout<<"syntax error: syntax error for create index statement!"<<endl;
        cout<<"\t missing ( !"<<endl;
        SQL="99";
        return SQL;
    }
    else
    {
        
        length=temp.length()-1;
        while(temp[length]==' ')
            length--;
        temp=temp.substr(0,length+1);
    
        if(temp.find(' ')==-1)
        {
            sql+=" "+temp;
     
            while(SQL[start]==' ')
                start++;
            end=SQL.find(')',start);
            temp=SQL.substr(start,end-start);
            start=end+1;

            if(temp==";")
            {
                cout<<"syntax error: syntax error for create index statement!"<<endl;
                cout<<"\t missing ) !"<<endl;
                SQL="99";
                return SQL;
            }
            else
            {
     
                length=temp.length()-1;
                while(temp[length]==' ')
                    length--;
                temp=temp.substr(0,length+1);
             
                if(temp.find(' ')==-1)
                {
                    sql+=" "+temp;
                    while(SQL[start]==' ')
                        start++;
                   // if(SQL[start]!=';'||start!=SQL.length()-1)
                    if(SQL[start]!=';')
                    {
                        cout<<"syntax error: syntax error for quit!"<<endl;
                        SQL="99";
                        return SQL;
                    }
                 
                    else
                        SQL="01"+sql;
                }
           
                else
                {
                    cout<<"error:"<<" "<<temp<<"---is not a valid attribute name!"<<endl;
                    SQL="99";
                    return SQL;
                }
            }
        }
 
        else
        {
            cout<<"error:"<<" "<<temp<<"---is not a valid table name!"<<endl;
            SQL="99";
            return SQL;
        }
    }
    return SQL;
}
///////////////////////////////////////////////////////
//
//
//
//
//          DROP
//
//
//
///////////////////////////////////////////////////////

string drop_table(string SQL,int start)
{
    string temp;
    int index,end;

    while(SQL[start]==' ')
        start++;
    index=start;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"error: table name can not be empty!"<<endl;
        SQL="99";
    }
    else
    {
        while(SQL[start]==' ')
            start++;
       
       // if(SQL[start]!=';'||start!=SQL.length()-1)
        if(SQL[start]!=';')
        {
            cout<<"error12:"<<SQL.substr(index,SQL.length()-index-2)<<"---is not a valid table name!"<<endl;
            SQL="99";
        }
        else
            SQL="10"+temp;
    }
    
    return SQL;
}
///////////////////////////////////////////////////////
string drop_index(string SQL,int start)
{
    string temp;
    int index,end;
    
    while(SQL[start]==' ')
        start++;
    index=start;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"error: index name can not be empty!"<<endl;
        SQL="99";
    }
    else
    {
        while(SQL[start]==' ')
            start++;
        
        //if(SQL[start]!=';'||start!=SQL.length()-1)
        if(SQL[start]!=';')
        {
            cout<<"error12:"<<SQL.substr(index,SQL.length()-index-2)<<"---is not a valid index name!"<<endl;
            SQL="99";
        }
        else
            SQL="11"+temp;
    }
    
    return SQL;
}
///////////////////////////////////////////////////////
string drop_clause(string SQL,int start)
{
    string temp;
    int end;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"syntax error: syntax error for drop statement!"<<endl;
        SQL="99";
    }
    
    else if(temp=="table")
        SQL=drop_table(SQL,start);
    
     else if(temp=="index")
        SQL=drop_index(SQL,start);
    
    else
    {
        cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
        SQL="99";
    }
    
    return SQL;
}

///////////////////////////////////////////////////////
//
//
//
//
//         DELETE
//
//
//
///////////////////////////////////////////////////////
string delete_clause(string SQL,int start)
{
    string temp;
    int end;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"syntax error: syntax error for delete statement!"<<endl;
        SQL="99";
    }
    
    else if(temp=="from")
        SQL=delete_from(SQL,start);
    
    else
    {
        cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
        SQL="99";
    }
    
    return SQL;
}
////////////////////////////////////////////////////////////////////////////////
string delete_from(string SQL,int start)
{
    string temp,sql;
    int index,end;
    
    while(SQL[start]==' ')
        start++;
    index=start;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    sql = temp + ",";
    start=end+1;
    if(temp==";")
    {
        cout<<"error: table name can not be empty!"<<endl;
        SQL="99";
    }
    else
    {
        while(SQL[start]==' ')
            start++;
        end=SQL.find(' ',start);
        if(SQL[start]!=';'||start!=SQL.length()-1)
        {
            temp=SQL.substr(start,end-start);
            start=end+1;
            if(temp=="where"){
                sql=where_clause(SQL,start,sql);
                if(sql=="99")
                    SQL=sql;
                else
                    SQL="41"+sql;
                
            }
            else{
            cout<<"error12:"<<SQL.substr(index,SQL.length()-index-2)<<"---is not a valid table name!"<<endl;
            SQL="99";
            }
        }
        else{
            SQL="40"+temp;
        }
        
    }
    
    return SQL;
}

///////////////////////////////////////////////////////
//
//
//
//
//         WHERE
//
//
//
///////////////////////////////////////////////////////
string where_clause(string SQL,int start,string sql){
    string temp,temp2;
    int end,index,finalend;
    int tempstart,f,success;
    tempstart=start;
    
    f = SQL.find("<>",tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.replace(f,2,"! ");
        SQL.insert(f, " ");
  
        f = SQL.find("<>",tempstart);
        tempstart = f+3;
    }
    f = SQL.find("<=",tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.replace(f,2,"@ ");
        SQL.insert(f, " ");

        f = SQL.find("<=",tempstart);
        tempstart = f+3;
    }
    f = SQL.find(">=",tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.replace(f,2,"# ");
        SQL.insert(f, " ");

        f = SQL.find(">=",tempstart);
        tempstart = f+3;
    }
    f = SQL.find('=',tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.insert(f, " ");
        SQL.insert(f+2," ");
        f = SQL.find('=',tempstart);
        tempstart = f+3;
    }
    f = SQL.find('>',tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.insert(f, " ");
        SQL.insert(f+2," ");
        f = SQL.find('>',tempstart);
        tempstart = f+3;
    }
    f = SQL.find('<',tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.insert(f, " ");
        SQL.insert(f+2," ");
        f = SQL.find('<',tempstart);
        tempstart = f+3;
    }

   
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    index=start;
    if(temp==";")
    {
        cout<<"syntax error: syntax error for where statement!"<<endl;
        sql="99";
    }
    else{
        finalend=SQL.find(';',start);
        while(index < finalend){
            temp2 = temp+" ";
            while(SQL[start]==' ')
                start++;
            end=SQL.find(' ',start);
            temp=SQL.substr(start,end-start);
            start=end+1;
            index=start;
            if(temp==";")
            {
                cout<<"syntax error: syntax error for where statement!"<<endl;
                sql="99";
                break;
            }
            else{
                if(temp=="=")temp2+="0 ";
                else if(temp=="!")temp2+="1 ";
                else if(temp=="<")temp2+="2 ";
                else if(temp==">")temp2+="3 ";
                else if(temp=="@")temp2+="4 ";
                else if(temp=="#")temp2+="5 ";
                else{
                    cout<<"syntax error: syntax error for where statement!"<<endl;
                    sql="99";
                    break;
                }
            }
            
            
            while(SQL[start]==' ')
                start++;
            
            end=SQL.find(' ',start);
            temp=SQL.substr(start,end-start);
            start=end+1;
            index=start;
            if(temp==";")
            {
                cout<<"syntax error: syntax error for where statement!"<<endl;
                sql="99";
                break;
            }
            else{
                temp2 += temp;
            }
            sql=sql+temp2+",";
            
            while(SQL[start]==' ')
                start++;
            
            end=SQL.find(' ',start);
            temp=SQL.substr(start,end-start);
            start=end+1;
            index=start;
            if(temp==";")break;
            else if(temp=="and"){
                while(SQL[start]==' ')
                    start++;
                
                end=SQL.find(' ',start);
                temp=SQL.substr(start,end-start);
                start=end+1;
                index=start;
            }
            else{
                cout<<"syntax error: syntax error for where statement!"<<endl;
                sql="99";
                break;
            }
        }
    }
    return sql;
}

///////////////////////////////////////////////////////
//
//
//
//
//        SELECT
//
//
//
///////////////////////////////////////////////////////
string select_clause(string SQL,int start)
{
    string temp;
    int end;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"syntax error: syntax error for select statement!"<<endl;
        SQL="99";
    }
    
    else if(temp=="*")
    {
        while(SQL[start]==' ')
            start++;
        end=SQL.find(' ',start);
        temp=SQL.substr(start,end-start);
        start=end+1;
        if(temp==";")
        {
            cout<<"syntax error: syntax error for select statement!"<<endl;
            SQL="99";
        }
        
        else if(temp=="from")
        {
            SQL=select_from(SQL,start);
        }
    }
    
    else
    {
        cout<<"syntax error: syntax error for select statement!"<<endl;
        SQL="99";
    }
    
    return SQL;
}

///////////////////////////////////////////////////////
string select_from(string SQL,int start)
{
    string temp,sql;
    int index,end;
    
    while(SQL[start]==' ')
        start++;
    index=start;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    sql = temp + ",";
    start=end+1;
    if(temp==";")
    {
        cout<<"error: table name can not be empty!"<<endl;
        SQL="99";
    }
    else
    {
        while(SQL[start]==' ')
            start++;
        end=SQL.find(' ',start);
        //if(SQL[start]!=';'||start!=SQL.length()-1)
        if(SQL[start]!=';')
        {
            temp=SQL.substr(start,end-start);
            start=end+1;
            if(temp=="where"){
                sql=where_clause(SQL,start,sql);
                if(sql=="99")
                    SQL=sql;
                else
                    SQL="21"+sql;
                
            }
            else{
                cout<<"error12:"<<SQL.substr(index,SQL.length()-index-2)<<"---is not a valid table name!"<<endl;
                SQL="99";
            }
        }
        else{
            SQL="20"+temp;
        }
        
    }
    
    return SQL;
}
///////////////////////////////////////////////////////
//
//
//
//
//        INSERT
//
//
//
///////////////////////////////////////////////////////
string insert_clause(string SQL,int start)
{
    string temp;
    int end;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"syntax error: syntax error for insert statement!"<<endl;
        SQL="99";
    }
    
    else if(temp=="into")
        SQL=insert_into(SQL,start);
    
    else
    {
        cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
        SQL="99";
    }
    
    return SQL;
}
///////////////////////////////////////////////////////
string insert_into(string SQL,int start)
{
    string temp,sql;
    int end,f,tempstart;
    tempstart=start;
    
    f = SQL.find(',',tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.insert(f, " ");
        SQL.insert(f+2," ");
        f = SQL.find(',',tempstart);
        tempstart = f+3;
    }
    
    f = SQL.find('(',tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.insert(f, " ");
        SQL.insert(f+2," ");
        f = SQL.find('(',tempstart);
        tempstart = f+3;
    }
    
    f = SQL.find(')',tempstart);
    tempstart = f+3;
    while(f!=-1){
        SQL.insert(f, " ");
        SQL.insert(f+2," ");
        f = SQL.find(')',tempstart);
        tempstart = f+3;
    }
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    
    if(temp==";")
    {
        cout<<"syntax error: syntax error for insert into statement!"<<endl;
        SQL="99";
    }
    else
    {
        sql=temp;
        
        while(SQL[start]==' ')
            start++;
        end=SQL.find(' ',start);
        temp=SQL.substr(start,end-start);
        start=end+1;
        
        if(temp==";")
        {
            cout<<"syntax error: syntax error for insert into statement!"<<endl;
            SQL="99";
        }
        
        else if(temp=="values"){
            sql=insert_into_values(SQL,start,sql);
            SQL="30"+sql;
        }
        
        else
        {
            cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
            SQL="99";
        }
    }
    return SQL;
}
///////////////////////////////////////////////////////
string insert_into_values(string SQL,int start,string sql)
{
    string temp;
    int end,length;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find('(',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    
    if(end==-1)
    {
        cout<<"syntax error: syntax error for insert into statement!"<<endl;
        cout<<"\t missing ( !"<<endl;
        SQL="99";
        return SQL;
    }
    else if(temp!=""){
        cout<<"syntax error: syntax error for insert into statement!"<<endl;
        SQL="99";
        return SQL;
    }
    else
    {
        while(true){
            while(SQL[start]==' ')
                start++;
            end=SQL.find(' ',start);
            temp=SQL.substr(start,end-start);
            start=end+1;
            if(temp==""){
                cout<<"syntax error: syntax error for insert into statement!"<<endl;
                SQL="99";
                return SQL;
            }
            else if(temp==";"){
                cout<<"syntax error: syntax error for insert into statement!"<<endl;
                  cout<<"\t missing ) !"<<endl;
                SQL="99";
                return SQL;
            }
            else if(temp==")"){
                break;
            }
            else if(temp==","){
                continue;
            }
            else{
                sql = sql+" "+temp;
            }
        }
    }
    return sql;
}

///////////////////////////////////////////////////////
//
//
//
//
//         EXECFILE
//
//
//
///////////////////////////////////////////////////////
string execfile_clause(string SQL,int start){
    string temp,temp2;
    int end;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp2=SQL.substr(start,end-start);
    start=end+1;
    if(temp==";")
    {
        cout<<"syntax error: syntax error for execfile statement!"<<endl;
        SQL="99";
    }
    else if(temp2!=";")
    {
        cout<<"syntax error: syntax error for execfile statement!"<<endl;
        SQL="99";
    }
    else
        SQL = "50"+temp;
    return SQL;
}

///////////////////////////////////////////////////////
//
//
//
//
//         QUIT
//
//
//
///////////////////////////////////////////////////////
string quit_clause(string SQL,int start){
    string temp;
    int end;
    
    while(SQL[start]==' ')
        start++;
    end=SQL.find(' ',start);
    temp=SQL.substr(start,end-start);
    start=end+1;
    if(temp!=";")
    {
        cout<<"syntax error: syntax error for quit statement!"<<endl;
        SQL="99";
    }
    else
        SQL = "60";
    return SQL;
}

