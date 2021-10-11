//
// Created by toke019 on 8.10.2021.
//

#include <stdio.h>
#include "../include/utils.h"

void
usage()
{
    fprintf(stderr, "Use alias: \"./mcd [alias]\". To use it with cd, use command substitution\n");
    fprintf(stderr, "For full help menu, use the -h flag.\n");
}

void
verbose_usage()
{
    printf("Use alias:\t ./mcd [alias] \n");
    printf("New alias:\t mcd [alias] [full path] \n");
    printf("Remove alias:\t mcd -d [alias] \n");
    printf("Reset database:\t mcd -r\n\n");
    printf("Optional flags:\n");
    printf( "\t-d: Delete alias. Alias argument required.\n");
    printf("\t-r: Reset database. This flag must be used alone, and asks for confirmation before continuing.\n");
}

void
print_critical(char *message)
{
    fprintf(stderr, "(CRITICAL) %s\n", message);
}

void
print_err(char *message)
{
    fprintf(stderr, "(ERROR) %s\n", message);
}