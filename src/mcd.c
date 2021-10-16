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

int handle_arguments(struct arguments *, int, sqlite3 *);

int
main(int argc, char *argv[])
{
    struct arguments args;
    struct DBHandler db_handler;

    if (init_db(&db_handler) < 0)
        return 1;

    if (parse_arguments(&args, argc, argv) < 0)
        return 1;

    if (handle_arguments(&args, argc, db_handler.db_connection) < 0)
        return 1;

    sqlite3_close(db_handler.db_connection);
    return EXIT_SUCCESS;
}

int
handle_arguments(struct arguments *args, int argc, sqlite3 *db)
{
    if (argc == 2) {
        if (args->help == 1) {
            verbose_usage();
        }

        if (args->reset_database == 1) {

            print_err("Reset db");
        }

        if (strlen(args->alias)) {
            printf("/Users/toke019/Documents/Development/mcd\n");
        }
    } else if (argc == 3) {
        if (args->arg_delete_alias && strlen(args->alias) > 0) {
            print_err("delete alias here");
        }

        if (strlen(args->alias) > 0 && strlen(args->path) > 0) {
            print_err("create new alias here");
        }
    }

    return EXIT_SUCCESS;
}