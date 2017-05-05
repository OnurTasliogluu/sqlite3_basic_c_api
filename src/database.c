#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>

sqlite3 *db;
int db_connection(char *db_name)
{
	// Veri tabani baglantisi yap.
	int connection_result = sqlite3_open(db_name, &db);

	// Baglantida hata var mi yok mu kontrol et.
	if( connection_result != SQLITE_OK)
	{
		// loglama yapilacak.
		fprintf(stdout,"Veri tabanına erişilemedi: %s", sqlite3_errmsg(db));
		return -1;
	}
	// Baglantida hata varsa hata numarasini `db_connection_error` yap.
	else
	{
		fprintf(stdout,"Veri tabanı başarıyla açıldı.");
		return 0;
	}
}

int sqlite3_insert_query(char* db_name, char* table_name, char *column_name, char *column_value)
{

	int result;
	char *error_message = NULL;
	char *sql_query = sqlite3_mprintf("INSERT INTO %q(%q) VALUES(%Q)",
						table_name,
						column_name,
						column_value);

	db_connection(db_name);
	result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
	sqlite3_close(db);

	for(;;)
	{
		switch(result)
		{
			case SQLITE_BUSY:
				fprintf(stdout,"busy, wait 0.1 seconds");
				usleep(100000);
				// Again Query
				db_connection(db_name);
				result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
				sqlite3_close(db);
				break;
			case SQLITE_ERROR:
			{
				fprintf(stdout,"SQL ERROR #%d: %s", result, error_message);
				sqlite3_free(error_message);
				return -1;
			}
			case SQLITE_OK:
			{
				// free sql_query
				sqlite3_free(sql_query);
				return 0;
			}
		}
	}
}

sqlite3_update_query(char *db_name, char* table_name, char* column_name, char* column_value)
{
        int result;
        char *error_message = NULL;
        char *sql_query = sqlite3_mprintf("UPDATE %q SET %q=%q",
                                                table_name,
                                                column_name,
                                                column_value);

        db_connection(db_name);
        result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
        sqlite3_close(db);

        for(;;)
        {
                switch(result)
                {
                        case SQLITE_BUSY:
                                fprintf(stdout,"busy, wait 0.1 seconds");
                                usleep(100000);
                                // Again Query
                                db_connection(db_name);
                                result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
                                sqlite3_close(db);
                                break;
                        case SQLITE_ERROR:
                        {
                                fprintf(stdout,"SQL ERROR #%d: %s", result, error_message);
                                sqlite3_free(error_message);
                                return -1;
                        }
                        case SQLITE_OK:
                        {
                                sqlite3_free(sql_query);
                                return 0;
                        }
                }
        }

}

int sqlite3_query_execute(char *db_name, char* sql_query)
{
	int result;
	char *error_message = NULL;

        db_connection(db_name);
        result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
        sqlite3_close(db);

        for(;;)
        {
                switch(result)
                {
                        case SQLITE_BUSY:
                                fprintf(stdout,"busy, wait 0.1 seconds");
                                usleep(100000);
                                // Again Query
                                db_connection(db_name);
                                result = sqlite3_exec(db, sql_query, 0, 0, &error_message);
                                sqlite3_close(db);
                                break;
                        case SQLITE_ERROR:
                        {
                                fprintf(stdout,"SQL ERROR #%d: %s", result, error_message);
                                sqlite3_free(error_message);
                                return -1;
                        }
                        case SQLITE_OK:
                        {
                                sqlite3_free(sql_query);
                                return 0;
                        }
                }
        }
	return 0;
}

int main(int argc,char *argv[])
{
	char *test = "test.db";
	char *sql_query = sqlite3_mprintf("UPDATE test SET id=1");
	sqlite3_query_execute(test,sql_query);

	return 0;
}
