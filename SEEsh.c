#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


extern char **environ;

void launch(char *args)
{
    printf("%s\n", args);
    char * buffer = (char*)malloc(sizeof(char) * strlen(args));
    strncpy(buffer, args , strlen(args) );
    char *executable =   strtok(buffer, " ");
    char *tmp = strchr(args, ' ');
    if(tmp != NULL){
        args = tmp + 1;
        printf("%s\n",args);
    }
    char *path_buffer = (char *)malloc(strlen(getenv("PATH")) * sizeof(char));
    strncpy(path_buffer, getenv("PATH"), strlen(getenv("PATH")));
    char* directory = strtok(path_buffer, ":");
    pid_t pid, wpid
    while (directory){
        printf("%s\n", directory);
    }


    free(path_buffer);
    free(buffer);
}

char *help()
{
    return (getenv("HELP"));
}

void unset(char *name)
{
    unsetenv(name);
}

void set(char *name, char *value)
{
    if (name && value)
        setenv(name, value, 1);
    else
    {
        char **variables = environ;
        while (*variables != NULL)
        {
            printf("%s\n", *variables++);
        }
    }
}

char *pwd()
{
    return (getenv("PWD"));
}

void cd(char *dir)
{
    strcat(getenv("PWD"), dir);
    printf("%s", getenv("PWD"));
}
char *evaluate(char *command)
{
    if (strncmp(command, "cd", 2) == 0)
    {
        command = strtok(NULL, " ");
        if (command)
            cd(command);
        else if (getenv("HOME"))
            setenv("PWD", getenv("HOME"), 1);
        else
            return "HOME is not defined or has been removed from enviroment";
    }
    else if (strncmp(command, "pwd", 3) == 0)
    {
        return pwd();
    }
    else if (strncmp(command, "help", 4) == 0)
    {
        return help();
    }
    else if (strncmp(command, "set", 3) == 0)
    {
        char *name = strtok(NULL, " ");
        char *value = strtok(NULL, " ");
        if (value && name)
            set(name, value);
        else if ((name && !value) || (!name && value))
            return "usage: set <variable> <value>";
        else
            set(NULL, NULL);
    }
    else if (strncmp(command, "unset", 5) == 0)
    {

        unset(strtok(NULL, " "));
    }
    else
    {
        return "executable";
    }
    return NULL;
}

void seesh_loop()
{
    size_t size = 512;
    char *buff = (char *)malloc(size * sizeof(char));
    char * line = (char*)malloc(size * sizeof(char));
    while (1)
    {
        printf("? ");
        getline(&buff, &size, stdin);
        if (strncmp("\n", buff, 3) == 0)
            continue;
        buff[strcspn(buff, "\n")] = 0;
        strncpy(line , buff, strlen(buff));
        char *command = strtok(buff, " ");
        char *seesh_out;
        if (strncmp(command, "exit", 4) == 0)
            break;
        else
            seesh_out = evaluate(command);
        if (seesh_out)
            if (strncmp(seesh_out, "executable", 10) == 0)
                launch(line);
            else
                printf("%s\n", seesh_out);
        buff[0] = '\0';
        memset(line,0,strlen(line));
        
    }
    free(buff);
        
}

void seesh_launch()
{
    char *buffer = (char *)malloc(sizeof(char) * 512);
    setenv("PWD", getenv("HOME"), 1);
    strcpy(buffer, getenv("HOME"));
    strncat(buffer, "/.SEEshrc", 9);
    FILE *f = fopen(buffer, "r");
    while (fgets(buffer, 512, f))
    {
        buffer[strcspn(buffer, "\n")] = 0;
        buffer = strtok(buffer, " ");
        evaluate(buffer);
    }
    free(buffer);
}

int main(int argc, char **argv)
{
    // Load config files, if any.
    seesh_launch();

    // Run command loop.
    seesh_loop();

    // Perform any shutdown/cleanup.

    return 0;
}
