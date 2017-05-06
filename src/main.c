#include "database.h"

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>


int main(int argc, char *argv[])
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

    variable_free();

    return 0;
}