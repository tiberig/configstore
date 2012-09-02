#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	sqlite3 *db;
} sql_db;

typedef struct {
	char *table_name;
	int   id;
} sql_col;

/*********************************************************************************/
static int sql_lookup(sqlite3 *db, char *table_name, char *col_name, char *keyword, sqlite3_stmt **stmt)
{
	char query_string[256];

	//TODO add string length check
	sprintf(query_string, "select * from %s  where %s = \"%s\"", table_name, col_name, keyword);

	if (0 != sqlite3_prepare_v2 (db, query_string, strlen (query_string) + 1, stmt, NULL)) {
		printf("SQLite prepare failed\n");
		return -1;
	}

	/* fetch data (only 1st row) */
	return sqlite3_step(*stmt);
}

/*********************************************************************************/
int get_float_by_keyword(sql_db *sqldb, sql_col *sqlcol, char *colname, char *keyword, float *value)
{
	sqlite3_stmt *stmt;

	if (SQLITE_ROW != sql_lookup(sqldb->db, sqlcol->table_name, colname, keyword, &stmt)) {
		printf("now row found\n");
		return -1;
	}

	*value = atof((char *)sqlite3_column_text(stmt, sqlcol->id));
	return 0;
}

/*********************************************************************************/
int get_text_by_keyword(sql_db *sqldb, sql_col *sqlcol, char *colname, char *keyword, char *value, int len)
{
	sqlite3_stmt *stmt;
	int           entry_len;
	char         *entry;

	if (SQLITE_ROW != sql_lookup(sqldb->db, sqlcol->table_name, colname, keyword, &stmt)) {
		printf("now row found\n");
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
int get_int_by_keyword(sql_db *sqldb, sql_col *sqlcol, char *colname, char *keyword, int *value)
{
	sqlite3_stmt *stmt;

	if (SQLITE_ROW != sql_lookup(sqldb->db, sqlcol->table_name, colname, keyword, &stmt)) {
		printf("now row found\n");
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
int sql_map_column(sql_db *sqldb, sql_col *sqlcol, char *name)
{
	char query_string[256], *s;
	sqlite3_stmt *stmt;

	//TODO add string length check
	sprintf(query_string, "pragma table_info(%s)", sqlcol->table_name);
	if (0 != sqlite3_prepare_v2 (sqldb->db, query_string, strlen (query_string) + 1, &stmt, NULL)) {
		printf("SQLite prepare failed\n");
		return -1;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		//printf("col=%s name=%s\n", sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1));
		s = (char *)sqlite3_column_text(stmt, 1);
		if (!strcmp(s, name)) {
			sqlcol->id = atoi((char *)sqlite3_column_text(stmt, 0));
			return 0;
		}
	}

	printf("sql_db: table=%s column=%s not found in database\n", sqlcol->table_name, name);
	return -1;
}

