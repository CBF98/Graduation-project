#include "data_processing.h"
#include "server.h"

int Extract_data(int num, char* buff)
{   // return 0 represent success, -1 represent error
    char* buf = NULL;
    buf = (char*)malloc(MAXLINE);
    strcpy(buf, buff);
    MYSQL mysql;
    int rc;
    char *query_str = NULL;
    query_str = (char*)malloc(150);
    if( NULL == mysql_init(&mysql) ){
        printf("error! %s\n", mysql_error(&mysql));
        return -1;
    }

    if( buf[0] == '0')
    {
        // sign up
        if ( buf[1] == '0')
        {
            //EPC
            if ( NULL == mysql_real_connect(&mysql, "localhost", "root", "hengheng", "test1", 0, NULL, 0)){
                printf("connect error ! %s", mysql_error(&mysql));
                return -1;
            }
            printf("success connect!\n");
            char* head = NULL;
            char* company = NULL;
            char* type = NULL;
            char* object = NULL;
            head = (char*)malloc(3);
            company = (char*)malloc(8);
            type = (char*)malloc(7);
            object = (char*)malloc(10);
            EPC_data(buf, head, company, type, object);
            printf("%s\n%s\n%s\n%s\n",head, company, type, object);
            sprintf(query_str, "insert into EPC%s%s%s values('%s', '%s')", head, company, type, object, client[num].client_ip);
            rc = mysql_real_query(&mysql, query_str, strlen(query_str));
            if (0 != rc) {
                printf("mysql_real_query(): %s\n", mysql_error(&mysql));
                return -1;
            }
            free(head);
            free(company);
            free(type);
            free(object);
        }
    }

    if( buf[0] == '1')
    {
        // sign down
        if( buf[1] == '0')
        {
            //EPC
            if ( NULL == mysql_real_connect(&mysql, "localhost", "root", "hengheng", "test1", 0, NULL, 0)){
                printf("connect error ! %s", mysql_error(&mysql));
                return -1;
            }
            printf("success connect!\n");
            char* head = NULL;
            char* company = NULL;
            char* type = NULL;
            char* object = NULL;
            head = (char*)malloc(3);
            company = (char*)malloc(8);
            type = (char*)malloc(7);
            object = (char*)malloc(10);
            EPC_data(buf, head, company, type, object);
            printf("%s\n%s\n%s\n%s\n",head, company, type, object);
            sprintf(query_str, "delete from EPC%s%s%s where ID = '%s'", head, company, type, object);
            rc = mysql_real_query(&mysql, query_str, strlen(query_str));
            if (0 != rc) {
                printf("mysql_real_query(): %s\n", mysql_error(&mysql));
                return -1;
            }
            free(head);
            free(company);
            free(type);
            free(object);
        }
    }

    if( buf[0] == '2')
    {
        //inquire
    }

    free(query_str);
    return 0;
}

char TwotoHex_char(char* Two)
{
    long int i;
    char a;
    i = strtol(Two, NULL, 10);
    if ( i < 10 && i >= 0)
    {
        a = i + 48;
    }
    else
    {
        a = i + 55;
    }
    return a;
}

void DatatoHex_int(int* data, int n, char* Hex)
{
    int j;
    char a;
    for(j = 0; j < n; j++)
    {
        if (data[j] < 10 && data[j] >= 0)
        {
            Hex[j] = data[j] + 48;
        }
        else{
            Hex[j] = data[j] + 55;
            }
    }
    Hex[j] = '\0';
}

void EPC_data(char* buff, char* head, char* company, char* type, char* object)
{
    unsigned char* buf = NULL;
    buf = (char*)malloc(MAXLINE);
    memset(buf, 0, sizeof(buf));
    strcpy(buf, buff);
    unsigned int data_int[10];
    int i,j;

    data_int[0] = (unsigned int)(buf[2] >> 4);
    data_int[1] = (unsigned int)(buf[2] & 15);
    DatatoHex_int(data_int, 2, head);

    data_int[0] = (unsigned int)(buf[3] >> 4);
    data_int[1] = (unsigned int)(buf[3] & 15);
    data_int[2] = (unsigned int)(buf[4] >> 4);
    data_int[3] = (unsigned int)(buf[4] & 15);
    data_int[4] = (unsigned int)(buf[5] >> 4);
    data_int[5] = (unsigned int)(buf[5] & 15);
    data_int[6] = (unsigned int)(buf[6] >> 4);
    DatatoHex_int(data_int, 7, company);

    data_int[0] = (unsigned int)(buf[6] & 15);
    data_int[1] = (unsigned int)(buf[7] >> 4);
    data_int[2] = (unsigned int)(buf[7] & 15);
    data_int[3] = (unsigned int)(buf[8] >> 4);
    data_int[4] = (unsigned int)(buf[8] & 15);
    data_int[5] = (unsigned int)(buf[9] >> 4);
    DatatoHex_int(data_int, 6, type);

    data_int[0] = (unsigned int)(buf[9] & 15);
    data_int[1] = (unsigned int)(buf[10] >> 4);
    data_int[2] = (unsigned int)(buf[10] & 15);
    data_int[3] = (unsigned int)(buf[11] >> 4);
    data_int[4] = (unsigned int)(buf[11] & 15);
    data_int[5] = (unsigned int)(buf[12] >> 4);
    data_int[6] = (unsigned int)(buf[12] & 15);
    data_int[7] = (unsigned int)(buf[13] >> 4);
    data_int[8] = (unsigned int)(buf[13] & 15);
    DatatoHex_int(data_int, 9, object);
}
