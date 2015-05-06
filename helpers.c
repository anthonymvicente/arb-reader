#include<stdlib.h>
#include<stdio.h>
#include<string.h>

char **split_filepath(char *filepath, int *depth)
{
    *depth = 0;
    int root = 0;
    int path_length = strlen(filepath);
    int i;

    for(i = 0; i < path_length; i++)
    {
        if(filepath[i] == '/')
        {
            (*depth)++;
            if(i == 0)
            {
                root = 1;
            }
        }
    }

    char **split_path;

    split_path = (char **) malloc(sizeof(char) * (*depth));

    if(root)
    {
        split_path[0] = NULL;
        i = 1;
    } else
    {
        i = 0;
    }
    split_path[i] = strtok(filepath, "/");
    i++;
    for(; i <= *depth; i++)
    {
        split_path[i] = strtok(NULL, "/");
    }

    return split_path;
}
