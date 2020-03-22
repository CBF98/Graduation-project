#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define EPC_head 2
#define EPC_company 7
#define EPC_type 6
#define EPC_object 9

#define Ecode_Version 1
#define Ecode_NSI 4
#define Ecode_AC 6
#define Ecode_IC 6

int Extract_data(int num, char* buff);
void DatatoHex_int(int* data, int n, char* Hex);
void DatatoTen_int(int* data, int n, char* Ten);
void EPC_data(char* buff, char* head, char* company, char* type, char* object);
void Ecode_data(char* buff, char* Version,  char* NSI, char* AC, char* IC);

