#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sql_db.h"

int main ()
{
	sql_db db;
	sql_col col_amount;
	float amount;

	if (0 != sql_db_init(&db, "test.db")) {
		printf("sql_db_init failed\n");
		return -1;
	}

	if (0 != sql_map_column(&db, &col_amount, "ausgaben", "Amount")) {
		printf("sql map failed\n");
		sql_db_close(&db);
		return -1;
	}

	if (0 != get_float_by_keyword(&db, &col_amount, "Description", "Tanken", &amount)) {
		sql_db_close(&db);
		printf("loopup failed\n");
		return -1;
	}
	printf("Tanken = %f\n", amount);

	if (0 != set_text_by_keyword(&db, &col_amount, "Description", "Tanken", "155")) {
		sql_db_close(&db);
		printf("update failed\n");
		return -1;
	}
	printf("Tanken = %f\n", amount);

	if (0 != get_float_by_keyword(&db, &col_amount, "Description", "Fleischer", &amount)) {
		sql_db_close(&db);
		printf("loopup failed\n");
		return -1;
	}

	printf("Fleischer = %f\n", amount);

	sql_db_close(&db);

    return 0;
}
