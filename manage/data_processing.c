#include "data_processing.h"



void obtain_lot_id(char* buff, char* destination, char* source)
{	//the first char of destination and source is the lot's type
	int i;
	if(buff[0] == '0')
	{	//the length of the id's is 96 bits
		strncpy(destination, &buff[0], EPC_length + 1);
		destination[EPC_length + 1] = '\0';
		if(buff[EPC_length + 1] == '0')
		{
			strncpy(source, &buff[EPC_length + 1], EPC_length + 1);
			source[EPC_length + 1] = '\0';
		}
        else if(buff[EPC_length + 1] == '1')
        {
            strncpy(source, &buff[EPC_length + 1], Ecode_length + 1);
            source[Ecode_length + 1] = '\0';
        }
	}
	else if(buff[0] == '1')
	{
		//the others types of lot id
		strncpy(destination, &buff[0], Ecode_length + 1);
		destination[Ecode_length + 1] = '\0';
		if(buff[Ecode_length + 1] == '0')
        {
            strncpy(source, &buff[Ecode_length + 1], EPC_length + 1);
            source[EPC_length + 1] = '\0';
        }
		else if(buff[Ecode_length + 1] == '1')
        {
            strncpy(source, &buff[Ecode_length + 1], Ecode_length + 1);
            source[Ecode_length + 1] = '\0';
        }
	}
	else
    {

    }
}


