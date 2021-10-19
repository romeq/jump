//
// Created by toke019 on 8.10.2021.
//

#include <string.h>
#include <stdio.h>

#include "../include/utils.h"
#include "../include/extern.h"
#include "../include/argument_parser.h"

int populate_struct_defaults(struct arguments *options);
int parse_flags(int argc, char **argv, struct arguments *args);
int parse_and_populate_args(int argc, char **argv, struct arguments *args);

int
parse_arguments(struct arguments *args, int argc, char **argv)
{
    populate_struct_defaults(args);

    if (parse_flags(argc, argv, args) < 0) {
        return -1;
    }

    if (parse_and_populate_args(argc, argv, args) < 0) {
        return -1;
    }

    return 0;
}

int
parse_and_populate_args(int argc, char **argv, struct arguments *args)
{
    if (args->arg_delete_alias == 1) {

        if (argc != 3) {
            print_err("Invalid amount of arguments");
            return -1;
        }

        int alias_to_memory;
        alias_to_memory = strlcpy(args->alias, argv[2], sizeof(args->alias));

        if (alias_to_memory > PATH_MAX_LENGTH) {
            print_critical("Predicted buffer overflow, maybe try shorter alias?");
            return -1;
        }

    } else {
        char *pred_buf_overflow_msg =
                "Oops! I can only have the first 255 letters, maybe try shorter alias?";

        switch(argc) {
            case 2:
                if (argv[1][0] == '-')
                    return 0;

                if (strlcpy(args->alias, argv[1], sizeof(args->alias)) > PATH_MAX_LENGTH) {
                    print_critical(pred_buf_overflow_msg);
                    return -1;
                }
                break;
            case 3:
                if (argv[1][0] == '-' || argv[2][0] == '-') {
                    print_err("Arguments can not be flags!");
                    return -1;
                }

                if (strlcpy(args->alias, argv[1], sizeof(args->alias)) > PATH_MAX_LENGTH) {
                    print_critical(pred_buf_overflow_msg);
                    return -1;
                }
                if (strlcpy(args->path, argv[2], sizeof(args->path)) > PATH_MAX_LENGTH) {
                    print_critical(pred_buf_overflow_msg);
                    return -1;
                }

                break;

            default:
                break;

        }
    }

    return 0;
}

int
parse_flags(int argc, char **argv, struct arguments *args)
{
    if (argc < 2 || argc > 3) {
        if (argc != 1) { print_err("Invalid amount of arguments."); }
        else { usage(); }
        return -1;
    }

    int flag_count = 0;
    int option;

    for (option = 1; option < argc && argv[option][0] == '-'; option++) {
<<<<<<< HEAD
        if (strlen(argv[option]) != 2) {
            fprintf(stderr, "Invalid flag: '%s'\n",
                    argv[option]);
            return -1;
        }
        if (strlen(argv[option]) <= 2) {
=======
        if (strlen(argv[option]) == 2) {
>>>>>>> dev
            switch (argv[option][1]) {
                case 'd':
                    args->arg_delete_alias = 1;
                    ++flag_count;
                    break;
                case 'r':
                    args->reset_database = 1;
                    ++flag_count;
                    break;
                case 'h':
                    args->help = 1;
                    ++flag_count;
                    break;
                case 'l':
                    args->show_all = 1;
                    ++flag_count;
                    break;
                default:
                    fprintf(stderr, "(ERROR) Invalid flag: '%s'\n",
                            argv[option]);
                    return -1;
            }
        } else {
            fprintf(stderr, "(ERROR) Invalid flag: '%s'\n",
                    argv[option]);
            return -1;
        }

    }

    if (flag_count > 1) {
        print_err("Too many flags.");
        return -1;
    }

    return 0;
}

int
populate_struct_defaults(struct arguments *options)
{
    // \0 == NULL
    strlcpy(options->alias, "\0", sizeof(options->alias));
    strlcpy(options->path,  "\0", sizeof(options->path));
    options->help = 0;
    options->arg_delete_alias = 0;
    options->reset_database = 0;
    options->show_all = 0;

    return 0;
}