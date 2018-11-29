#include "dbms.h"

//Function to check if the table is present in the database that is being used
bool check_table(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//if no of databases>0
		if(temp_list->list[i]->database_name==curr_database){	//if curr_database is created
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//if no of tables in database > 0
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//curr_table present in database
					return true;
				}
			}
		}
	}
	return false;
}

//Function to retreive the table from the database that is being used
table* get_table(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//if no of databases > 0
		if(temp_list->list[i]->database_name==curr_database){	//if curr_database is created
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//if no of tables in database > 0
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//curr_table present in database
					return temp_list->list[i]->table_list[j];		//return pointer to the table
				}
			}
		}
	}
}

//Function to retreive the database from the list of databases
database* get_database(string curr_database,database_list* temp_list){
	for(int i=0;i<temp_list->list.size();i++){
		if(temp_list->list[i]->database_name==curr_database){
			return temp_list->list[i];
		}
	}
}

//Function to check if the database has been created or not
bool check_database(string temp, database_list* temp_list){
	for(int i=0;i<temp_list->list.size();i++){		//if no of databases > 0
		if(temp_list->list[i]->database_name==temp){	//if curr_database is created
			return true;
		}
	}
	return false;
}

//Function to check if the row to be insereted has the structure same as the table
bool check_insertion(vector<attr*>tup, table* curr_table){	
	if(tup.size()==curr_table->table_st.size()){	//if no of attributes in tuple and table structure are equal
		for(int j=0;j<tup.size();j++){		//for all attributes in the tuple to be inserted
			if(tup[j]->data_type!=curr_table->table_st[j]->data_type){	//checks for data_type
				return false;
			}
			else{
				if(tup[j]->attr_val=="" && curr_table->table_st[j]->constraint=="NOTNULL"){//checks for constraints
					return false;
				}
			}
		}
	}
	else{
		return false;
	}
	return true;
}

//Function to print the meta data of the table
void print_tuple_st(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//for all databases
		if(temp_list->list[i]->database_name==curr_database){	//In the current database
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//for all tables
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//In the current table
					print_sequence(80);
					print_format(80,curr_table+" meta data");
					cout<<endl;
					print_sequence(80);
					print_format(27,"Attribute");
					print_format(27,"Data Type");
					print_format(26,"Constraint");
					cout<<endl;
					print_sequence(80);
					for(int l=0;l<temp_list->list[i]->table_list[j]->table_st.size();l++){ //print structure
						print_format(27,temp_list->list[i]->table_list[j]->table_st[l]->attr_name);
						print_format(27,temp_list->list[i]->table_list[j]->table_st[l]->data_type);
						print_format(26,temp_list->list[i]->table_list[j]->table_st[l]->constraint);
						cout<<endl;
					}
					print_sequence(80);
				}
			}
		}
	}
}

//Function to print the rows present in the table of currently used database
void print_tuples(string curr_table,database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//for all databases
		if(temp_list->list[i]->database_name==curr_database){	//In current database
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){	//for all tables
				if(temp_list->list[i]->table_list[j]->table_name==curr_table){	//For the current table
					int k=temp_list->list[i]->table_list[j]->table_st.size();
					print_sequence(27*k);
					print_format(27*k,curr_table);
					cout<<endl;
					print_sequence(27*k);
					for(int l=0;l<temp_list->list[i]->table_list[j]->table_st.size();l++){	//print all attribute names
						print_format(27,temp_list->list[i]->table_list[j]->table_st[l]->attr_name);
					}
					cout<<endl;
					print_sequence(27*k);
					for(int l=0;l<temp_list->list[i]->table_list[j]->tup.size();l++){//Print the tuples
						for(int k=0;k<temp_list->list[i]->table_list[j]->tup[l].size();k++){
							print_format(27,temp_list->list[i]->table_list[j]->tup[l][k]->attr_val);
						}
						cout<<endl;
					}
					print_sequence(27*k);
				}
			}
		}
	}
}

//Function to print the databases present 
void print_databases(database_list* temp_list){
	cout<<"+--------------------------------+"<<endl;
	cout<<"|        List of databases       |"<<endl;
	cout<<"+--------------------------------+"<<endl;
	for(int i=0;i<temp_list->list.size();i++){	//Print all the databases
		print_format(34,temp_list->list[i]->database_name);
		cout<<endl;
	}
	cout<<"+--------------------------------+"<<endl;
}

//Function to print the tables present in the currently used database
void print_tables(database_list* temp_list,string curr_database){
	for(int i=0;i<temp_list->list.size();i++){	//For all the databases
		if(temp_list->list[i]->database_name==curr_database){	//In the current database
			cout<<"+--------------------------------+"<<endl;
			print_format(34,"Tables in "+curr_database);
			cout<<endl;
			cout<<"+--------------------------------+"<<endl;
			for(int j=0;j<temp_list->list[i]->table_list.size();j++){ //print all tables
				print_format(34,temp_list->list[i]->table_list[j]->table_name);
				cout<<endl;
			}
			cout<<"+--------------------------------+"<<endl;
		}
	}
}

//Function to print the detais neatly
void print_format(int count,string s){
	int l = count-s.length()-2;
	cout<<"|";
	if(l%2==0){
		for(int j=0;j<l/2;j++) cout<<" ";
	}
	else{
		for(int j=0;j<l/2+1;j++) cout<<" ";
	}
	cout<<s;
	for(int j=0;j<l/2;j++) cout<<" ";
	cout<<"|";
}

//Function to print the detais neatly
void print_sequence(int count){
	cout<<"+";
	for(int i=0;i<count-2;i++) cout<<"-";
	cout<<"+"<<endl;
}

//Function to print all the attributes of the selected tables by using join
void print_all(vector<table*> sel_tables,database_list* temp_list){
	int n = sel_tables.size();
	int l = 45;
	print_sequence(l);
	int total = 1;
	for(int i=0;i<sel_tables.size();i++){	//For all selected tables
		total = total * sel_tables[i]->tup.size();
		for(int j=0;j<sel_tables[i]->table_st.size();j++){	//print attributes of all selected tables
			print_format(15,sel_tables[i]->table_st[j]->attr_name);
		}
	}
	cout<<endl;
	print_sequence(l);
	int total1 = total;	//Total rows in the table created by join
	vector<vector<vector<vector<attr*> > > >temp(sel_tables.size(),vector<vector<vector<attr*> > >(total));
	int r = 1;
	for(int i=0;i<sel_tables.size();i++){	//For all selected tables
		total = total/sel_tables[i]->tup.size();
		int j=0;
		for(int k=0;k<total;k++){	//Push the attributes to the table as per the join rule
			for(int l=0;l<sel_tables[i]->tup.size();l++){
				for(int m=0;m<r;m++){
					temp[i][j].push_back(sel_tables[i]->tup[l]);
					j++;
				}
			}
		}
		r=r*sel_tables[i]->tup.size();
	}
	total = total1;
	for(int m=0;m<total;m++){	//print the tuples in newly created table
		for(int i=0;i<temp.size();i++){
			for(int k=0;k<temp[i][m].size();k++){
				for(int l=0;l<temp[i][m][k].size();l++){
					print_format(15,temp[i][m][k][l]->attr_val);
				}
			}
		}
		cout<<endl;
	}
	print_sequence(l);
}



