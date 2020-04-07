#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <libgen.h> 
#include "../disk/driver.h"
#define vdisk_path "disk/vdisk"

unsigned char bit_vector[512];
int inode_map[128];

void InitLLFS()
{
    FILE* disk = fopen(vdisk_path, "w");
    char* init = calloc(BLOCK_SIZE * NUM_BLOCKS, 1);
    fwrite(init,BLOCK_SIZE*NUM_BLOCKS, 1 , disk);
    free(init);
    fclose(disk);

    disk = fopen(vdisk_path, "rb+");

    char* buffer;

    buffer = (char*) malloc(BLOCK_SIZE);
    int magic = 36;
    int blocks = NUM_BLOCKS;
    int inodes = NUM_INODES;

    memcpy(buffer,&magic,sizeof(int));
    memcpy(buffer + sizeof(int) * 1, &blocks, sizeof(int));
    memcpy(buffer + sizeof(int) * 2, &inodes, sizeof(int));
    writeBlock(disk,0,buffer);
    free(buffer);
    for (int i = 0; i < 512; i++) {
            bit_vector[i] = 0x00;
    }
    for(int i = 0 ; i <13;i++){
        set_block(bit_vector,i);
    }
    writeBlock(disk,1,bit_vector);
    // create root inode
    buffer = (char*) malloc(BLOCK_SIZE);
    int flag  = 0; //folder
    int size = 0;
    short int location = 11;
    memcpy(buffer,&flag,sizeof(int));
    memcpy(buffer + sizeof(int)*1, &size,sizeof(int));
    memcpy(buffer + sizeof(int) * 2, &location,sizeof(short int));
    writeBlock(disk,10,buffer);
    //root is empty so we dont need to write anything to that block
    // create b
    for (int i = 0; i < 128; i++) {
            inode_map[i] = 0;
    }
    inode_map[0] = 10;
    free(buffer);
    buffer = calloc(BLOCK_SIZE,1);
    memcpy(buffer, &inode_map[0], sizeof(int));
    writeBlock(disk,12,buffer);
    free(buffer);
 
    fclose(disk);
}

void create_file(char * data){
    FILE* disk = fopen(vdisk_path, "r+");
    char* init = calloc(BLOCK_SIZE * NUM_BLOCKS, 1);
    memcpy(init, data, strlen(data) + 1);
    writeBlock(disk,34,init);
}

void set_block(unsigned char buffer[], int block_num)
{
    int index = block_num / 8;
    int bit_index = block_num % 8;
    
    //printf("SET\n");
    //printf("Index: %d\n", index);
    //printf("Bit Index: %d\n", bit_index);
    buffer[index] |= 1UL << bit_index;
}

void unset_block(unsigned char buffer[], int block_num)
{
    int index = block_num / 8;
    int bit_index = block_num % 8;
    
    //printf("UNSET\n");
    //printf("Index: %d\n", index);
    //printf("Bit Index: %d\n", bit_index);
    buffer[index] &= ~(1UL << bit_index);
}

void mkdir(char* path){
    char buff[100];
    memset(buff, '\0', sizeof(buff));
    strncpy(buff,path,strlen(path));
    if (buff[0] != '/'){
        printf("Path must be absolute. No action taken!\n");
    }

    char* tok = strtok(buff, "/"); 

     while (tok != NULL)
    {
        tok = strtok(NULL, "/");
    } 

}

char * read(char* filename, char* path){
    char * buffer;
    char* content;
    readLLFS();
    FILE* disk = fopen(vdisk_path, "r");
    buffer = malloc(BLOCK_SIZE);
    if (strncmp(path,"/root", strlen(path)) == 0){
        readBlock(disk,inode_map[0],buffer);
        short int root_index;
        memcpy(&root_index, buffer + sizeof(int) * 2 , sizeof(short int));
        memset(buffer, 0 , sizeof(buffer));
        readBlock(disk,root_index,buffer);
        content = malloc(BLOCK_SIZE);
        memset(content,0,sizeof(content));
        for (int i = 0 ; i<16; i++){
            memcpy(content, buffer+1+(32*i),31);
            if (strncmp(filename,content,sizeof(filename)) == 0){
                printf(" found %s\n", content);
                memset(content,0,BLOCK_SIZE);
                unsigned char index;
                memcpy(&index,buffer+(32*i),sizeof(unsigned char));
                printf(" the index is %d\n", index);
                printf("asdasdasdas%d\n", inode_map[index]);
                readBlock(disk,inode_map[index]-1,content);
                break;
            }
        }
    }
    return content;
}

void rmdir(char* path){

}

void unlink(char* path){

}

void mkfile(char* filename, char* path, char* data){
    readLLFS();
    FILE * fp = fopen(vdisk_path,"rb+");
    char buff [100];
    char * block;
    int index;
    memset(buff,'\0',sizeof(buff));
    strncpy(buff,path,strlen(path));
    int curr_end= find_end();
    //printf("%s\n", buff);
    if (strncmp(buff,"/root\0" ,strlen(path)) == 0){
        block = calloc(BLOCK_SIZE,1);
        readBlock(fp,inode_map[0],block);
        short int root_index;
        memcpy(&root_index,block+ sizeof(int)*2, sizeof(short int));
        printf("root directory is located at %d\n" , root_index );

        //printf("%d\n", root_index);
        memset(block,0 , sizeof(block));
        readBlock(fp,root_index,block);
        for (int i = 0 ; i < 16; i++){
            memcpy(buff, block+sizeof(char)*i*32,sizeof(int));;
            if (strncmp(buff, "0",strlen(buff)) == 0){
                //printf("index available\n");
                index = i;
                printf("index is available at position %d\n" , i);
                break;
            }
            else if(i == 15){
                printf("system is already full");
                return;
            }
        }
            unsigned char inode_number  = (unsigned char) free_index();
            memset(buff,0,sizeof(buff));
            strncpy(buff,filename,strlen(filename));
            memcpy(block+sizeof(char)*index*32,&inode_number,sizeof(unsigned char));
            memcpy(block+sizeof(char)*index*32 + sizeof(unsigned char),buff,sizeof(char)*32);
            printf("%d\n", curr_end);
            writeBlock(fp,curr_end,block);
            block = (char*) malloc(BLOCK_SIZE);
            int flag  = 0; //folder
            int size = 0;
            short int location = (short int) curr_end;
            memcpy(block,&flag,sizeof(int));
            memcpy(block + sizeof(int)*1, &size,sizeof(int));
            memcpy(block + sizeof(int) * 2, &location,sizeof(short int));
            writeBlock(fp,curr_end+1,block);
            inode_map[0] = curr_end+1;

            memset(block,0,BLOCK_SIZE);
            strncpy(block,data,strlen(data));
            writeBlock(fp,curr_end+2,block);

            memset(block,0,BLOCK_SIZE);
            flag  = 1; //file
            size = 0;
            location = (short int) (curr_end +2);
            memcpy(block,&flag,sizeof(int));
            memcpy(block + sizeof(int)*1, &size,sizeof(int));
            memcpy(block + sizeof(int) * 2, &location,sizeof(short int));
            writeBlock(fp,curr_end+3,block);
            inode_map[inode_number] = curr_end+3;

            for(int i = 0; i < 5;i++){
                set_block(bit_vector,curr_end+i);
            }
            writeBlock(fp,curr_end+4, inode_map);
            writeBlock(fp,1,bit_vector);

        free(block);
    }
    
    fclose(fp);
}



void readLLFS(){
    char * buffer;
    FILE* disk;
    buffer = (char*) malloc(BLOCK_SIZE);
    disk = fopen("disk/vdisk", "rb+");
    readBlock(disk,1,buffer);
    for (int i = 0 ;  i < 512 ; i++){
        memcpy(&bit_vector[i], buffer+sizeof(unsigned char)*i,sizeof(unsigned char));
    }
    int end = find_end();
    free(buffer);
    buffer = (char*)malloc(BLOCK_SIZE);
    readBlock(disk,end-1,buffer);
    for (int i = 0 ; i < 128 ; i++){
        memcpy(&inode_map[i], buffer + sizeof(int)*i, sizeof(int));
    }
    free(buffer);
    fclose(disk);


}

int find_end()
{
    int offset = 0;
    for (int i = 0 ; i<512;i++){
        unsigned char byte;
        char * buff;
        int k;
        buff = malloc(sizeof(char)*8);
        for (int x = 7; x >= 0; x--)
        {
            byte = bit_vector[i];
            k = byte >> x;
        
            if (k & 1)
                strcat(buff,"1");
            else
                strcat(buff,"0");
        }
        if (strncmp(buff,"11111111",strlen(buff)) < 0){
            offset = (i+1)*8;
            for (int j = 0;j<8;j++){
                if(buff[j] == '0'){
                    offset--;
                }
            }
            free(buff);
            break;
        }
        free (buff);
    }
    return offset;
}

void print_binary(unsigned char byte)
{
    int k;
    for (int i = 7; i >= 0; i--)
    {
        k = byte >> i;
        
        if (k & 1)
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

int free_index(){
    for (int  i = 0 ; i < 128 ; i++){
        int  x = inode_map[i];
        if (x == 0){
            return i;
            break;
        }
    }
    return -1;
}
