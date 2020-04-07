#include <stdio.h>
#include <stdlib.h>
#include "../io/File.h"

int main()
{
   //create_file(buffer);
   InitLLFS();
   mkfile("a3.txt" , "/root" , "this is some data to test");
   mkfile("d3.sd", "/root", "this should be in the folder as well!");
    mkfile("ds.sd", "/root", "this should beasdasdsa in the folder as well!");
    mkfile("homework.txt", "/root", "this should beasdasdsa in the folder as well!");
    char * content = read("homework.txt", "/root");
    if (content != NULL){
        printf("%s\n", content);
    }
}
