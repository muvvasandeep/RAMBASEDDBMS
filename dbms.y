%{
	#include "dbms.h"
	using namespace std;
	database *temp_database = new database;
	table *curr_table ;
	table *temp_table = new table;
	database_list *temp_list = new database_list;
	vector< table* > sel_tables;
	string curr_database="";
	int yylex(void);
	int yyerror(string);
%}

%union {
	char* p;
	node* con;
	attr* a;
}

%token <p> 	CREATE TABLE DATABASE SHOW INSERT DESCRIBE DROP DATA_TYPE INTO CONSTRAINT
%token <p> 	IDENTIFIER VALUES NUM USE DATABASES TABLES SELECT FROM WHERE REL_OP
%type  <con> 	OP_CON CR_ARG CREATE_ST
%type  <a>	INS_ARG INSERT_ST

%%
PROGRAM:		ST
			|ST PROGRAM
			;
ST:			CREATE_ST ';' 
			| INSERT_ST ';'
			| USE_ST ';'
			| SHOW_ST ';'
			| SELECT_ST ';'
			| DESCRIBE_ST ';'
			| error ';' {yyerror("Invalid Query");}
			;
CREATE_ST:		CREATE DATABASE IDENTIFIER {
				if(!check_database($3,temp_list)){
					temp_database->database_name = $3;
					temp_list->list.push_back(temp_database);
					temp_database = new database;
				}
				else {
					cout<<"Database already present"<<endl;
				}
			}
			| CREATE TABLE IDENTIFIER {curr_table = new table;}'(' CR_ARG ')' {
				if(curr_database==""){
					cout<<"No database is being used"<<endl;
				}
				else{
					if(check_database(curr_database,temp_list)){
						database* temp = new database;
						curr_table->table_name = $3;
						temp = get_database(curr_database,temp_list);
						temp->table_list.push_back(curr_table);
					}
					else{
						cout<<"No database is being used"<<endl;
					}
				}
			}
			;
CR_ARG:			IDENTIFIER OP_CON {
				st *temp_st = new st;
				temp_st->attr_name = $1;
				temp_st->data_type = $<con>2->data_type;
				temp_st->constraint = $<con>2->constraint;
				curr_table->table_st.insert(curr_table->table_st.begin(),temp_st);
			}
			| IDENTIFIER OP_CON ',' CR_ARG {
				st *temp_st = new st;
				temp_st->attr_name = $1;
				temp_st->data_type = $<con>2->data_type;
				temp_st->constraint = $<con>2->constraint;
				curr_table->table_st.insert(curr_table->table_st.begin(),temp_st);
			}
			;
OP_CON:			DATA_TYPE {
				node *temp = new node;
				temp->data_type = $1;
				temp->constraint = "NULL";
				$<con>$ = temp;
			}
			| CONSTRAINT {
				node *temp = new node;
				temp->data_type = "string";
				temp->constraint = $1;
				$<con>$ = temp;
			}
			| DATA_TYPE CONSTRAINT {
				node *temp = new node;
				temp->data_type = $1;
				temp->constraint = $2;
				$<con>$ = temp;
			}
			|%empty{
				node* temp = new node;
				temp -> data_type = "string";
				temp -> constraint = "NULL";
				$<con>$ = temp;
			}
			;
INSERT_ST:		INSERT INTO IDENTIFIER VALUES '(' {
								if(check_table($3,temp_list,curr_database)){
									curr_table = get_table($3,temp_list,curr_database);
								}
							  } INS_ARG ')' {
				if(check_insertion(curr_table->temp_tup,curr_table)){
					curr_table->tup.push_back(curr_table->temp_tup);
					vector<attr* >().swap(curr_table->temp_tup);
					print_tuples($3,temp_list,curr_database);
					cout<<"VALID"<<endl;
				}
				else{
					cout<<curr_table->table_name<<endl;
					cout<<"INVALID"<<endl;
				}
			}
			;
INS_ARG:		IDENTIFIER {
				attr* temp_attr = new attr;
				temp_attr->attr_val = $1;
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|IDENTIFIER ',' INS_ARG{
				attr* temp_attr = new attr;
				temp_attr->attr_val = $1;
				temp_attr->data_type = "string";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|NUM {
				attr* temp_attr = new attr;
				temp_attr->attr_val = $1;
				temp_attr->data_type = "int";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			|NUM ',' INS_ARG{
				attr* temp_attr = new attr;
				temp_attr->attr_val = $1;
				temp_attr->data_type = "int";
				curr_table->temp_tup.insert(curr_table->temp_tup.begin(),temp_attr);
			}
			;
USE_ST:			USE IDENTIFIER {
				if(check_database($2,temp_list)){
					curr_database = $2;
				}
				else {
					cout<<"No database is present with the name"<<$2<<endl;
				}
			}
			;
SHOW_ST:		SHOW DATABASES {
				print_databases(temp_list);
			}
			|SHOW TABLES {
				print_tables(temp_list,curr_database);
			}
			;
DESCRIBE_ST:		DESCRIBE IDENTIFIER {
				if(check_table($2,temp_list,curr_database)){
					print_tuple_st($2,temp_list,curr_database);
				}
				else{
					cout<<"No such table is there in the current database "<<curr_database<<endl;
				}
			}
			;
SELECT_ST:		SELECT '*' FROM SEL_TABLES {
				print_all(sel_tables,temp_list);
				vector<table*>().swap(sel_tables);
			}
			| SELECT '*' FROM SEL_TABLES WHERE CONDITION_ST {
				
			}
			;
SEL_TABLES:		IDENTIFIER ',' SEL_TABLES {
				table* temp = new table;
				string t = $1;
				if(check_table($1,temp_list,curr_database)){
					temp = get_table($1,temp_list,curr_database);
					sel_tables.insert(sel_tables.begin(),temp);
				}
				else{
					yyerror(t+" table not found");
					vector<table*>().swap(sel_tables);
				}	
			}
			| IDENTIFIER {
				table* temp = new table;
				string t = $1;
				if(check_table($1,temp_list,curr_database)){
					temp = get_table($1,temp_list,curr_database);
					sel_tables.insert(sel_tables.begin(),temp);
				}
				else{
					yyerror(t+" table not found");
					vector<table*>().swap(sel_tables);
				}
			}
			;
CONDITION_ST:		IDENTIFIER REL_OP EXP {
				
			}
			;
EXP:			EXP '+' TERM
			| EXP '-' TERM
			| TERM
			;
TERM:			TERM '*' FACTOR
			| TERM '/' FACTOR
			| FACTOR
			;
FACTOR:			'(' EXP ')'
			| IDENTIFIER
			| NUM
			;
			

%%

int main(int argc, char *argv[]){
	yyparse();
}

int yyerror(string s){
  	cout<<s<<endl;
}

