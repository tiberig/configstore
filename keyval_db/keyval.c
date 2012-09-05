#include <stdio.h>
#include <sql_db.h>
#include <keyval.h>

int kvs_init(kvs_hnd *hnd, char *dbfile, char *table)
{
	if (0 != sql_db_init(&(hnd->sql), dbfile)) {
		printf("kvs_init failed\n");
		return -1;
	}
	hnd->table_name = table;
	return 0;
}

void kvs_close(kvs_hnd *hnd)
{
	sql_db_close(&(hnd->sql));
}

int kvs_get_text(kvs_hnd *hnd, char *key, char *buf, int len)
{
	sql_col col_text;

	col_text.table_name = hnd->table_name;
	if (0 != sql_map_column(&(hnd->sql), &col_text, "Value")) {
		printf("kvs: sql map failed\n");
		sql_db_close(&(hnd->sql));
		return -1;
	}

	if (0 != get_text_by_keyword(&(hnd->sql), &col_text, "Key", key, buf, len)) {
		sql_db_close(&(hnd->sql));
		printf("kvs loopup failed\n");
		return -1;
	}

	return 0;
}

