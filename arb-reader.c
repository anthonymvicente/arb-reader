#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<time.h>
#include<unistd.h>
#include<sys/stat.h>
#include"helpers.h"

#define FIRST_LINE 0
#define MAX_BUFFER 1024
#define MAX_LOOP 5
#define PADDING_RATIO 10

#define CUR_DIR "."
#define PAR_DIR ".."

int file_selected = 0;

int rand_range(int, int);
void add_glitch_to_line(char *);
void select_file(FILE *, char *);
void select_file_from_dir(char *, char *);
int count_lines(FILE *);
void remove_ending_newline(char *);
char *get_filename_from_path(char **, int);

int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 4)
    {
        fprintf(stderr, "invalid number of arguments: %d\n", argc - 1);
        fprintf(stderr, "usage: %s -[d,g] [file list]\n", argv[0]);
        exit(1);
    }

    int select_from_dir = 0;
    int add_glitch = 0;
    char file_path[MAX_BUFFER];
    char dir_path[MAX_BUFFER];

    // if we have more than one argument, the first arguments need to be flags
    if(argc >= 3)
    {
        // loop through flags, stopping before the last argument, which should be the filepath
        int i = 1;
        for(; i < argc - 1; i++)
        {
            if(strstr(argv[i], "-") == NULL)
            {
                fprintf(stderr, "invalid argument: %s\n", argv[i]);
                fprintf(stderr, "this argument is expected as a flag with '-' preceding flag options [d,g]\n");
                exit(1);
            }

            if(strstr(argv[i], "d") != NULL)
            {
                select_from_dir = 1;
                strcpy(dir_path, argv[argc - 1]);
                if(dir_path[strlen(dir_path) - 1] != '/')
                {
                    strcat(dir_path, "/");
                }
            }

            if(strstr(argv[i], "g") != NULL)
            {
                add_glitch = 1;
            }

        }
    }

    srand(time(NULL));

    if(!select_from_dir)
    {
        char *f_list_name = argv[argc - 1];
        FILE *f_list = fopen(f_list_name, "r");

        if(f_list == NULL)
        {
            fprintf(stderr, "error opening file: %s\n", f_list_name);
            return 1;
        }

        select_file(f_list, file_path);

        fclose(f_list);

        remove_ending_newline(file_path);
    } else
    {
        strcpy(file_path, dir_path);
        select_file_from_dir(dir_path, file_path);
    }

    FILE *file = fopen(file_path, "r");

    if(file == NULL)
    {
        fprintf(stderr, "error opening file: %s\n", file_path);
        exit(1);
    }

    int number_of_lines = count_lines(file);

    // reset pointer in file stream
    fseek(file, 0, SEEK_SET);

    // end padding is used to ensure our start_line_number isn't the very last line of the text
    // after all, we want to read some lines
    int end_padding;
    if(PADDING_RATIO != 0)
    {
        end_padding = number_of_lines / PADDING_RATIO;
        if(end_padding < 1)
        {
            end_padding = 1;
        }
    } else
    {
        end_padding = 1;
    }
    // start padding is used to ensure that the number of lines we read is somewhat relative to the size of the text
    // this means the longer the text, the more lines we'll read
    int read_line_padding;
    if(PADDING_RATIO != 0)
    {
        read_line_padding = number_of_lines / PADDING_RATIO;
        if(read_line_padding < 1)
        {
            read_line_padding = 1;
        }
    } else
    {
        read_line_padding = 1;
    }

    // determine the starting line
    // this will range between the first line, and a padding offset back from the end of the text
    int start_line_number = rand_range(FIRST_LINE, number_of_lines - end_padding);
    // determine the number of lines to read
    // this will range between the padding we determined above, and the number of lines remaining after the start line is taken into account
    int read_line_number = rand_range(read_line_padding, number_of_lines - start_line_number);

    char r_line[MAX_BUFFER];
    char out_buff[MAX_BUFFER * read_line_number];

    int i = 0;
    for(; i < start_line_number; i++)
    {
        fgets(r_line, MAX_BUFFER, file);
    }

    // initial empty buffer
    strcpy(out_buff, "");

    for(i = 0; i < read_line_number; i++)
    {
        fgets(r_line, MAX_BUFFER, file);
        if(add_glitch && rand() % 2)
        {
            add_glitch_to_line(r_line);
        }
        strcat(out_buff, r_line);
    }

    fclose(file);

    int depth;
    char **split_path = split_filepath(file_path, &depth);
    char *file_name = get_filename_from_path(split_path, depth);

    printf("excerpt from: %s\n\n", file_name);
    printf("%s", out_buff);

    return 0;
}

int rand_range(int min, int max)
{

    int diff = max - min;

    return (int) (((double) (diff + 1) / RAND_MAX) * rand() + min);

}

void select_file(FILE *f_list, char *file_name)
{
    char *fgets_results;
    int restart_count = 0;

    while(!file_selected)
    {
        fgets_results = fgets(file_name, MAX_BUFFER, f_list);

        // if fgets returns null, we're at EOF
        if(fgets_results == NULL)
        {
            // start back from beginning
            fseek(f_list, 0, SEEK_SET);

            // try again
            fgets_results = fgets(file_name, MAX_BUFFER, f_list);

            restart_count++;

            if(fgets_results == NULL)
            {
                fprintf(stderr, "empty file...\n");
                exit(1);
            }
        }

        if(rand() % 2)
        {
            file_selected = 1;
        }

        if(restart_count == MAX_LOOP)
        {
            file_selected = 1;
        }
    }
}

void add_glitch_to_line(char *str)
{
    int str_len = strlen(str);
    int i = 0;
    for(; i < str_len; i++)
    {
        if(rand() % 10 == 2)
        {
            str[i] = '$';
        }
    }
}

void select_file_from_dir(char *dir_path, char *file_name)
{
    DIR *dp;
    struct dirent *ep;
    struct stat fstat;
    dp = opendir(dir_path);
    int restart_count = 0;
    char file_path[MAX_BUFFER];

    if(dp == NULL)
    {
        fprintf(stderr, "error opening directory: %s\n", dir_path);
        exit(1);
    }

    while(!file_selected)
    {
        ep = readdir(dp);

        if(ep == NULL)
        {
            closedir(dp);
            dp = opendir(dir_path);
            ep = readdir(dp);
            if(ep == NULL)
            {
                fprintf(stderr, "empty directory: %s\n", dir_path);
                exit(1);
            }
            restart_count++;
        }

        strcpy(file_path, dir_path);

        strcat(file_path, ep->d_name);

        if((stat(file_path, &fstat)) == -1)
        {
            fprintf(stderr, "error stating file: %s\n", ep->d_name);
            exit(1);
        }

        if(S_ISREG(fstat.st_mode))
        {
            if(rand() % 2 || restart_count == MAX_LOOP)
            {
                file_selected = 1;
                strcat(file_name, ep->d_name);
            }
        }
    }

    closedir(dp);
}

int count_lines(FILE *file)
{
    int ch, number_of_lines = 0;

    do
    {
        ch = fgetc(file);
        if(ch == '\n')
        {
            number_of_lines++;
        }
    } while(ch != EOF);

    return number_of_lines;
}

void remove_ending_newline(char *str)
{
    str[strlen(str) - 1] = '\0';
}

char *get_filename_from_path(char **split_path, int depth)
{
    return split_path[depth];
}
