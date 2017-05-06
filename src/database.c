#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>

sqlite3 *db = NULL;
char* db_name = NULL;

int set_db_name(char* v_db_name)
{
    db_name = malloc(sizeof(v_db_name));
    if (db_name == NULL)
    {
        fprintf(stdout, "set_db_name - malloc failed.");
        return -1;
    }
    memcpy(db_name,v_db_name,sizeof(v_db_name));
    return 0;
}

char *get_db_name()
{
    return db_name;
}

int db_connection()
{
    int connection_result = sqlite3_open(db_name, &db);

    if( connection_result != SQLITE_OK)
    {
        fprintf(stdout,"Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        fprintf(stdout,"Opened database successfully\n");
        return 0;
    }
}

int sqlite3_query_execute(char* sql_query)
{
    int result;
    char *error_message = NULL;

    fprintf(stdout, "%s\n", sql_query);

    db_connection(db_name);
    result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
    sqlite3_close(db);

    for(;;)
    {
            switch(result)
            {
                case SQLITE_BUSY:
                    fprintf(stdout,"busy, wait 0.1 seconds\n");
                    usleep(100000);
                    // Again Query
                    db_connection(db_name);
                    result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
                    sqlite3_close(db);
                    break;

                case SQLITE_ERROR:
                {
                    fprintf(stdout,"SQL ERROR: %s \n", error_message);
                    sqlite3_free(sql_query);
                    sqlite3_free(error_message);
                    return -1;
                }

                case SQLITE_OK:
                {
                    sqlite3_free(sql_query);
                    return 0;
                }

                case SQLITE_FULL: // 13   /* Insertion failed because database is full */
                    break;
            }
    }
    return 0;
}


int sqlite3_query_execute_delete_table(char* table_name, int id)
{
    int result;
    char *error_message = NULL;

    char *sql_query = sqlite3_mprintf("DELETE FROM %q WHERE id=%d", table_name, id);

    fprintf(stdout, "%s\n", sql_query);

    db_connection(db_name);
    result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
    sqlite3_close(db);

    for(;;)
    {
            switch(result)
            {
                case SQLITE_BUSY:
                    fprintf(stdout,"busy, wait 0.1 seconds\n");
                    usleep(100000);
                    // Again Query
                    db_connection(db_name);
                    result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
                    sqlite3_close(db);
                    break;

                case SQLITE_ERROR:
                {
                    fprintf(stdout,"SQL ERROR : %s\n", error_message);
                    sqlite3_free(sql_query);
                    sqlite3_free(error_message);
                    return -1;
                }

                case SQLITE_OK:
                {
                    sqlite3_free(sql_query);
                    return 0;
                }

                case SQLITE_FULL: // 13   /* Insertion failed because database is full */
                    break;        
            }
    }
    return 0;
}

int main(int argc,char *argv[])
{
    set_db_name("test.db");

    char *sql_query_test_create_table = sqlite3_mprintf("CREATE TABLE test(" \
        "ID INT," \
        "NAME TEXT)");

    char *sql_query_test_update = sqlite3_mprintf("UPDATE test SET id=1");

    if (sqlite3_query_execute(sql_query_test_create_table))
    {
        fprintf(stdout, "Can't execute query\n");
        return -1;
    }

     // UPDATE test SET id=1
    if (sqlite3_query_execute(sql_query_test_update))
    {
        fprintf(stdout, "Can't execute query\n");
        return -1;
    }

    // Test Delete table from id.
    if (sqlite3_query_execute_delete_table("test", 1))
    {
        fprintf(stdout, "Can't execute delete table query\n");
        return -1;
    }

    return 0;
}