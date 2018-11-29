#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

// Structure of the attributes that are to be inserted into the table
struct attr{
	string attr_val;		//Attribute value
	string data_type;		//Datatype of attribute to be insereted
};

// Structure of the attribute when the table is created
struct st{
	string attr_name;		//Name of the attribute
	string data_type;		//Datatype of the attribute
	string constraint;		//Constraint(NULL/NOTNULL)
};

// Structure of each table
struct  table{
	string table_name;		//Table name
	vector< st* > table_st;		//Table structure at the time of creation of table
	vector<vector< attr* > > tup;	//Structure of each row
	vector< attr* > temp_tup;	//Temporary row to validate the inserting row before insertion
};

// Structure of each database
struct database{
	string database_name;		//Database name
	vector< table* > table_list;	//List of table that the database consists
};

// Structure of the list of databases 
struct database_list{			
	vector < database* > list;	//List of databases that are created
};

//Structure to verify the constraints of each value to be insereted
struct node{
	string data_type;		//Datatypee of the value or attribute to be inserted into the table
	string constraint;		//Constraint to check if it allows NULL values or not
};

//Function to check if the row to be insereted has the structure same as the table
bool check_insertion(vector<attr*> tup, table* curr_table);

//Function to check if the database has been created or not
bool check_database(string temp,database_list* temp_list);

//Function to check if the table is present in the database that is being used
bool check_table(string curr_table,database_list* temp_list,string curr_database);

//Function to retreive the table from the database that is being used
table* get_table(string curr_table,database_list* temp_list,string curr_database);

//Function to retreive the database from the list of databases
database* get_database(string curr_database,database_list* temp_list);

//Function to print the databases present 
void print_databases(database_list* temp_list);

//Function to print the tables present in the currently used database
void print_tables(database_list* temp_list,string curr_database);

//Function to print the meta data of the table
void print_tuple_st(string curr_table,database_list* temp_list,string curr_database);

//Function to print the rows present in the table of currently used database
void print_tuples(string curr_table,database_list* temp_list,string curr_database);

//Function to print the detais neatly
void print_format(int count,string s);

//Function to print the detais neatly
void print_sequence(int count);

//Function to print all the attributes of the selected tables by using join
void print_all(vector<table*> sel_tables,database_list* temp_list);
