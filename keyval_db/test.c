#include <stdio.h>

#include "keyval.h"

int main(void)
{
	kvs_hnd hnd;
	char buf[100];
	char table[] = "Test";

	if (0 != kvs_init(&hnd, "test.db",  table)) {
		return -1;

	}

	if (0 != kvs_get_text(&hnd, "DeviceName", buf, 100)) {
		return -1;
	}

	printf("DeviceName = %s\n", buf);

	return 0;
}
