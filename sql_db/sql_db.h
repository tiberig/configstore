#ifndef SQL_DB_H
#define SQL_DB_H

#include <sqlite3.h>

#define LABEL_SIZE 32

typedef struct {
	sqlite3 *db;
} sql_db;

typedef struct {
	char table_name[LABEL_SIZE];
	char column_name[LABEL_SIZE];
	int   id;
} sql_col;

int get_float_by_keyword(sql_db  *sqldb, 
						 sql_col *sqlcol,
						 char    *colname,
						 char    *keyword, 
						 float   *value);

int get_text_by_keyword (sql_db  *sqldb,
		                 sql_col *sqlcol,
						 char    *colname,
						 char    *keyword,
						 char    *buf,
						 int      len);

int set_text_by_keyword(sql_db   *sqldb, 
		                sql_col  *sqlcol, 
						char     *key_col, 
						char     *key, 
						char     *value);

int get_int_by_keyword  (sql_db  *sqldb,
		                 sql_col *sqlcol,
						 char    *colname,
						 char    *keyword,
						 int     *value);

int  sql_db_init    (sql_db *sqldb, char *filename);
void sql_db_close   (sql_db *sqldb);
int  sql_map_column (sql_db *sqldb, sql_col *sqlcol, char *table, char *column);
#endif
