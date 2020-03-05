#include "data_processing.h"



void obtain_lot_id(char* buff, char* destination, char* source)
{	//the first char of destination and source is the lot's type
	int i;
	if(buff[0] == '0')
	{	//the length of the id's is 96 bits
		strncpy(destination, &buff[0], 13);
		destination[13] = '\0';
		if(buff[13] == '0')
		{
			strncpy(source, &buff[13], 13);
			source[13] = '\0';
		}
	}
	else
	{
		//the others types of lot id
	}
}


