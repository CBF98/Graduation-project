#include "data_processing.h"
#include "server.h"

void Extract_data(int num, char* buff)
{
    char buf[MAXLINE];
    memset(buf, 0, sizeof(buf));
    strcpy(buf, buff);
    char path[100];
    char name[20];
    FILE* fp;
    if( buf[0] == '0')
    {
        // sign up
        if ( buf[1] == '0')
        {
            //EPC
            memset(path, 0, sizeof(path));
            strcat(path, database);
            memset(name, 0, sizeof(name));

            EPC_data(buf, path, name);
            strcat(path, ".txt");
            if( (fp = fopen(path, "a+")) == NULL )
            {
                printf("%s sign up error in %s!\n",client[num].client_ip, path);
                exit(0);
            }
            printf("%s\n",name);
            //strcat(name, "  ");
            strcat(name, client[num].client_ip);
            fprintf(fp, "%s\n", name);
            fclose(fp);
        }
    }
    if( buf[0] == '1')
    {
        // sign down
    }
    if( buf[0] == '2')
    {
        //inquire
    }
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
}

void EPC_data(char* buff, char* path, char *NAME)
{
    unsigned char buf[MAXLINE];
    memset(buf, 0, sizeof(buf));
    strcpy(buf, buff);
    char name[5][5];
    char name_other[11];
    unsigned int data_int[10];
    int i,j;

    memset(name, 0, sizeof(name));
    memset(name_other, 0, sizeof(name_other));

    data_int[0] = (unsigned int)(buf[2] >> 4);
    printf("%d\n",data_int[0]);
    data_int[1] = (unsigned int)(buf[2] & 15);
    DatatoHex_int(data_int, 2, name[0]);

    data_int[0] = (unsigned int)(buf[3] >> 4);
    data_int[1] = (unsigned int)(buf[3] & 15);
    data_int[2] = (unsigned int)(buf[4] >> 4);
    DatatoHex_int(data_int, 3, name[1]);

    data_int[0] = (unsigned int)(buf[4] & 15);
    data_int[1] = (unsigned int)(buf[5] >> 4);
    data_int[2] = (unsigned int)(buf[5] & 15);
    data_int[3] = (unsigned int)(buf[6] >> 4);
    DatatoHex_int(data_int, 4, name[2]);

    data_int[0] = (unsigned int)(buf[6] & 15);
    data_int[1] = (unsigned int)(buf[7] >> 4);
    data_int[2] = (unsigned int)(buf[7] & 15);
    DatatoHex_int(data_int, 3, name[3]);

    data_int[0] = (unsigned int)(buf[8] >> 4);
    data_int[1] = (unsigned int)(buf[8] & 15);
    data_int[2] = (unsigned int)(buf[9] >> 4);
    DatatoHex_int(data_int, 3, name[4]);

    data_int[0] = (unsigned int)(buf[9] & 15);
    data_int[1] = (unsigned int)(buf[10] >> 4);
    data_int[2] = (unsigned int)(buf[10] & 15);
    data_int[3] = (unsigned int)(buf[11] >> 4);
    data_int[4] = (unsigned int)(buf[11] & 15);
    data_int[5] = (unsigned int)(buf[12] >> 4);
    data_int[6] = (unsigned int)(buf[12] & 15);
    data_int[7] = (unsigned int)(buf[13] >> 4);
    data_int[8] = (unsigned int)(buf[13] & 15);
    DatatoHex_int(data_int, 9, name_other);

    for( i = 0; i < 4; i++)
    {
        strcat(path, name[i]);
        strcat(NAME, name[i]);
        strcat(path, "/");
    }
    strcat(path, name[4]);
    strcat(NAME, name[4]);
    strcat(NAME, name_other);

}
