#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manage.h"

void obtain_lot_id(char* buff, char* destination, char* source);
int match_lot(char* buff, struct lot_Attributes lot[], int quantity);
