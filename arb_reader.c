#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<time.h>

#define FIRST_LINE 0
#define MAX_BUFFER 1024
#define SELECTED 1
#define CUR_DIR "."
#define PAR_DIR ".."

int rand_range(int, int);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "invalid number of arguments: %d\n", argc - 1);
        fprintf(stderr, "usage: %s [file list]\n", argv[0]);
        exit(1);
    }

    srand(time(NULL));

    int file_selected = !SELECTED;
    char *f_list_name = argv[1];

    FILE *f_list = fopen(f_list_name, "r");


    if(f_list == NULL)
    {
        fprintf(stderr, "error opening file: %s\n", f_list_name);
        return 1;
    }

    char full_file_path[MAX_BUFFER];
    char *file_name;

    FILE *file = fopen(full_file_path, "r");
    int ch, number_of_lines = 0;

    if(file == NULL)
    {
        fprintf(stderr, "error opening file: %s\n", file_name);
        exit(1);
    }

    do
    {
        ch = fgetc(file);
        if(ch == '\n')
        {
            number_of_lines++;
        }
    } while(ch != EOF);

    fclose(file);

    int end_padding = number_of_lines / 10;
    if(end_padding < 1)
    {
        end_padding = 1;
    }
    int start_padding = number_of_lines / 10;
    if(start_padding < 1)
    {
        start_padding = 1;
    }

    int start_line_number = rand_range(FIRST_LINE, number_of_lines - end_padding);
    int read_line_number = rand_range(start_padding, number_of_lines - start_line_number);
    char r_line[MAX_BUFFER];
    char out_buff[MAX_BUFFER * read_line_number];

    file = fopen(full_file_path, "r");

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
        strcat(out_buff, r_line);
    }

    fclose(file);

    printf("excerpt from: %s\n\n", file_name);
    printf("%s", out_buff);

    return 0;
}

int rand_range(int min, int max)
{

    int diff = max - min;

    return (int) (((double) (diff + 1) / RAND_MAX) * rand() + min);

}

void select_file()
