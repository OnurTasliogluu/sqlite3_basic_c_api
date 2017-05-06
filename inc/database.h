#ifndef DATABASE_H
#define DATABASE_H

int set_db_name(char* v_db_name);
char *get_db_name();
int variable_free();

int db_connection();
int sqlite3_query_execute(char* sql_query);
int sqlite3_query_execute_delete_table(char* table_name, int id);

#endif