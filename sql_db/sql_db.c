#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include <sql_db.h>

/*********************************************************************************/
static int sql_lookup(sqlite3 *db, char *table_name, char *col_name, char *keyword, sqlite3_stmt **stmt)
{
	char query_string[256];

	//TODO add string length check
	sprintf(query_string, "select * from %s  where %s = \"%s\"", table_name, col_name, keyword);

	if (0 != sqlite3_prepare_v2 (db, query_string, strlen (query_string) + 1, stmt, NULL)) {
		printf("SQLite lookup failed\n");
		return -1;
	}

	/* fetch data (only 1st row) */
	return sqlite3_step(*stmt);
}

/*********************************************************************************/
static int sql_update_string(sqlite3 *db, 
					  char *table_name, 
					  char *key_col_name, 
					  char *keyword, 
					  char *update_col_name, 
					  char *update_value)
{
	char query_string[256];
	char *err = NULL;

	//TODO add string length check
	sprintf(query_string, "update %s set %s = \"%s\" where %s = \"%s\"", 
			table_name, 
			update_col_name, update_value,
			key_col_name, keyword);

	sqlite3_exec(db, query_string, NULL, NULL, &err);

	if (err != NULL) {
		sqlite3_free(err);
		return -1;
	}

	return 0;
}

/*********************************************************************************/
int get_float_by_keyword(sql_db *sqldb, sql_col *sqlcol, char *colname, char *keyword, float *value)
{
	sqlite3_stmt *stmt;

	if (SQLITE_ROW != sql_lookup(sqldb->db, sqlcol->table_name, colname, keyword, &stmt)) {
		printf("no row found\n");
		return -1;
	}

	*value = atof((char *)sqlite3_column_text(stmt, sqlcol->id));
	return 0;
}

/*********************************************************************************/
int get_text_by_keyword(sql_db *sqldb, 
		                sql_col *sqlcol, 
						char *colname, 
						char *keyword, 
						char *value, 
						int len)
{
	sqlite3_stmt *stmt;
	int           entry_len;
	char         *entry;

	if (SQLITE_ROW != sql_lookup(sqldb->db, 
				                 sqlcol->table_name, 
								 colname, 
								 keyword, 
								 &stmt)) {
		printf("no row found\n");
		return -1;
	}

	entry     = (char *) sqlite3_column_text(stmt, sqlcol->id);
	entry_len = strlen(entry);
	if (entry_len >= len) {
		printf("get_text_by_keyword: buffer too small\n");
		return -1;
	} 

	strncpy(value, entry, entry_len + 1);
	return 0;
}

/*********************************************************************************/
int set_text_by_keyword(sql_db *sqldb, 
		                sql_col *sqlcol, 
						char *key_col, 
						char *key, 
						char *value)
{
	sqlite3_stmt *stmt;

	//first check if row exists
	if (SQLITE_ROW != sql_lookup(sqldb->db, 
				                 sqlcol->table_name, 
								 key_col, 
								 key, 
								 &stmt)) {
		printf("no row found\n");
		return -1;
	}

	if (0 != sql_update_string(sqldb->db, 
				               sqlcol->table_name, 
							   key_col, 
							   key, 
							   sqlcol->column_name, 
							   value)) {
		printf("no row found\n");
		return -1;
	}

	return 0;
}

/*********************************************************************************/
int get_int_by_keyword(sql_db *sqldb, sql_col *sqlcol, char *colname, char *keyword, int *value)
{
	sqlite3_stmt *stmt;

	if (SQLITE_ROW != sql_lookup(sqldb->db, sqlcol->table_name, colname, keyword, &stmt)) {
		printf("no row found\n");
		return -1;
	}

	*value = atoi((char *)sqlite3_column_text(stmt, sqlcol->id));
	return 0;
}

/*********************************************************************************/
int sql_db_init(sql_db *sqldb, char *filename)
{
    if (0 != sqlite3_open(filename, &sqldb->db)) {
		printf("SQLite Open failed\n");
		return -1;
	}
	return 0;
}

/*********************************************************************************/
void sql_db_close(sql_db *sqldb)
{
	sqlite3_close(sqldb->db);
}

/*********************************************************************************/
/* maps "name" to a column id and stores id in sqlcol */
int sql_map_column(sql_db *sqldb, sql_col *sqlcol, char *table, char *column)
{
	char query_string[256], *s;
	sqlite3_stmt *stmt;

	if (strlen(table) >= LABEL_SIZE) {
		printf("sql_map_column: table name exceeds LABLE_SIZE\n");
		return -1;
	}

	if (strlen(column) >= LABEL_SIZE) {
		printf("sql_map_column: column name exceeds LABLE_SIZE\n");
		return -1;
	}

	strncpy(sqlcol->table_name,  table,  strlen(table)  + 1);
	strncpy(sqlcol->column_name, column, strlen(column) + 1);

	//TODO add string length check
	sprintf(query_string, "pragma table_info(%s)", table);
	if (0 != sqlite3_prepare_v2 (sqldb->db, query_string, strlen (query_string) + 1, &stmt, NULL)) {
		printf("SQLite prepare failed\n");
		return -1;
	}

	sqlcol->id = -1;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		//printf("col=%s name=%s\n", sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1));
		s = (char *)sqlite3_column_text(stmt, 1);
		if (!strcmp(s, column)) {
			sqlcol->id = atoi((char *)sqlite3_column_text(stmt, 0));
			return 0;
		}
	}

	printf("sql_db: table=%s column=%s not found in database\n", 
			sqlcol->table_name, column);
	return -1;
}

