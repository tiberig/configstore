#ifndef SQL_DB_H
#define SQL_DB_H

#include <sqlite3.h>

typedef struct {
	sqlite3 *db;
} sql_db;

typedef struct {
	char *table_name;
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

int get_int_by_keyword  (sql_db  *sqldb,
		                 sql_col *sqlcol,
						 char    *colname,
						 char    *keyword,
						 float   *value);

int  sql_db_init    (sql_db *sqldb, char *filename);
void sql_db_close   (sql_db *sqldb);
int  sql_map_column (sql_db *sqldb, sql_col *sqlcol, char *name);
#endif
