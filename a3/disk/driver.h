#ifndef DRIVER_H
#define DRIVER_H

void readBlock(FILE *disk, int blockNum, char *buffer);

void writeBlock(FILE *disk, int blockNum, char *data);

void test();

#endif