
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "storage.h"

int debug_list(void)
{
	int i = 0;
	STORAGE_INFO_t info;

	storage_init();

	for(int i=0;i<65535;i++)
	{
		info.serialid = i;
		storage_insert_information(&info);
	}
	storage_print_info();
	storage_deinit();

	return 0;

}

int main(void)
{
	printf("hello world \n");

	debug_list();


	return 0;

}




