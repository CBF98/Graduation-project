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