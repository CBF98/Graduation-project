#include "StringtoHex.h"

void StringtoTen(char* buff,int* hex, int n)
{
	int i;
	char a[2] = {'0', '\0'};
	for(i = 0; i < n; i++)
	{
		a[0] = buff[i];
		hex[i] = strtol(a, NULL, 16);
	}
}
