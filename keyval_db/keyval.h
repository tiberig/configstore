#ifndef KEYVAL_H
#define KEYVAL_H

#include "sql_db.h"

typedef struct {
	sql_db  sql;
	char   *table_name;
} kvs_hnd;

int  kvs_init(    kvs_hnd *hnd, char *dbfile, char *table);
void kvs_close(   kvs_hnd *hnd);
int  kvs_get_text(kvs_hnd *hnd, char *key, char *buf, int len);

#endif
