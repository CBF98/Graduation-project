#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "HextoTwo.h"
#include "StringtoHex.h"

#define ID "110000001000001EF023FFEA"
#define FIFO "/home/cbf/lot_server/fifo/"
#define FIFO_sign "/home/cbf/lot_server/fifo/sign.fifo"
