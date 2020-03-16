#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int Extract_data(int num, char* buff);
char TwotoHex_char(char* Two);
void DatatoHex_int(int* data, int n, char* Hex);
void EPC_data(char* buff, char* head, char* company, char* type, char* object);
