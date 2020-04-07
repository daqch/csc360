#ifndef FILE_H
#define FILE_H

void runTest();

void InitLLFS();

void create_file(char* disk);

void readLLFS();

void set_block(unsigned char buffer[], int block_num);

void unset_block(unsigned char buffer[], int block_num);

int find_end();

void mkfile(char* filename,char*path, char*data);

char * read(char* filename,char* path);


#endif