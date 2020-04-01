#include "HextoTwo.h"

void HextoTwo(int* id, int n, unsigned char* id_char)
{
	int m = 4*n,i,j,t = n/2;
	int id_int[m];
	int reference[16][4] = { {0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1},\
	{0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1},\
	{1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1},\
	{1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1}};
	memset(id_int, 0, sizeof(id_int));
	memset(id_char, 0, sizeof(id_char));
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < 4; j++)
		{
			id_int[4*i+j] = reference[(int)id[i]][j];
		}
	}
	for(i = 0; i < t; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(id_int[8*i+j] == 1)
			{
				id_char[i] = id_char[i] | (0x1 << (7-j));
			}
		}
	}
}

void StringtoTwo(char* id_char, int n, unsigned char* id_two)
{
    int i;
    unsigned int id_ten[n];
    memset(id_ten, 0, sizeof(id_ten));
    for(i = 0; i < n; i++){
        if(id_char[i] >= 48 && id_char[i] <= 57) {
            if (id_char[i] == 48) {
                id_ten[i] = 10;
                continue;
            }
            id_ten[i] = id_char[i] - 48;
        }
    }
    id_two[0] = id_ten[0] + 48;
    HextoTwo(&id_ten[1], n, &id_two[1]);
}

void Divide_OID(char* Ecode_id, char* lot_real_id)
{
	int i = 0;
	char* lot_id = NULL;
	lot_id = (char *)calloc(1, sizeof(char) * 18);
	strcpy(lot_id, &Ecode_id[16]);

	unsigned int id_ten[18];
	memset(id_ten, 0, sizeof(id_ten));

	for(i = 0; i < 8; i++){
		if(lot_id[i] > 48 && lot_id[i] <= 57){
			id_ten[i] = lot_id[i] - 48;
		}
	}

	HextoTwo(id_ten, 8, lot_real_id);
	lot_real_id[4] = '.';

	for(i = 9; i < 17; i++){
		if(lot_id[i] > 48 && lot_id[i] <= 57){
			id_ten[i-9] = lot_id[i] - 48;
		}
	}

	HextoTwo(id_ten, 8, &lot_real_id[5]);
	lot_real_id[9] = '\0';
	free(lot_id);
}