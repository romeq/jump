//
// Created by toke019 on 5.10.2021.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "../include/argument_parser.h"
#include "../include/database.h"
#include "../include/utils.h"

int handle_arguments(struct arguments *args, int argc, char ** argv);

int
main(int argc, char *argv[])
{
    struct arguments args;
    struct DBHandler db_handler;

    if (handle_arguments(&args, argc, argv) < 0) {
        return EPERM;
    }

    db_handler = init_db();
    if (db_handler.succeeded < 0) {
        return EPERM;
    }

    sqlite3_close(db_handler.db_connection);
    return EXIT_SUCCESS;
}

int
handle_arguments(struct arguments *args, int argc, char ** argv)
{
    int parse_status;
    if ((parse_status = parse_arguments(args, argc, argv)) < 0) {
        return parse_status;
    }

    if (args->help == 1) {
        verbose_usage();
    }

    if (strncmp(args->alias, "_test", sizeof(args->alias)) == 0) {
        printf("/Users/toke019\n");
    }

    return EXIT_SUCCESS;
}