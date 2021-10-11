//
// Created by toke019 on 8.10.2021.
//

#include <string.h>
#include <stdio.h>
#include "../include/utils.h"
#include "../include/argument_parser.h"

int populate_default(struct arguments *options);
int parse_flags(int argc, char **argv, struct arguments *args);
int parse_and_populate_args(int argc, char **argv, struct arguments *args);

int
handle_arguments(struct arguments *args, int argc, char **argv)
{
    populate_default(args);

    int pf_status, handle_flags_status;

    pf_status = parse_flags(argc, argv, args);
    if (pf_status != 0) {
        return pf_status;
    }

    handle_flags_status = parse_and_populate_args(argc, argv, args);
    if (handle_flags_status != 0) {
        return handle_flags_status;
    }

    return 0;
}

int
parse_and_populate_args(int argc, char **argv, struct arguments *args)
{
    if (args->arg_delete_alias == 1) {
        if (argc != 3) {
            print_err("Invalid amount of arguments");
            return 1;
        }

        int copy_to_memory;
        copy_to_memory = strlcpy(args->alias, argv[2], PATH_MAX_LENGTH);

        if (copy_to_memory > PATH_MAX_LENGTH) {
            print_critical("Predicted buffer overflow, maybe try shorter alias?");
            return 2;
        }

    } else if (args->reset_database == 1 || args->help == 1) {
        if (argc != 2) {
            print_err("Invalid amount of arguments: flag must be used alone");
            return 1;
        }

    } else {
        char pred_buf_overflow_msg[] =
                "Oops! I can only scan the first 255 letters, maybe try shorter alias?";

        switch(argc) {
            case 2:
                if (strlcpy(args->alias, argv[1], PATH_MAX_LENGTH) > PATH_MAX_LENGTH) {
                    print_critical(pred_buf_overflow_msg);
                    return 2;
                }

                break;
            case 3:
                if (strlcpy(args->alias, argv[1], PATH_MAX_LENGTH) > PATH_MAX_LENGTH) {
                    print_critical(pred_buf_overflow_msg);
                    return 2;
                }
                if (strlcpy(args->path, argv[2], PATH_MAX_LENGTH) > PATH_MAX_LENGTH) {
                    print_critical(pred_buf_overflow_msg);
                    return 2;
                }

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
        return 1;
    }

    int flag_count = 0;
    int option;

    for (option = 1; option < argc && argv[option][0] == '-'; option++) {
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
            default:
                fprintf(stderr, "(%s) Invalid flag: %s\n",
                        "WARNING", argv[option]);
                return 1;
        }

    }

    if (flag_count > 1) {
        print_err("Too many flags.");
        return 1;
    }

    return 0;
}

int
populate_default(struct arguments *options)
{
    // \0 == NULL
    strlcpy(options->alias, "\0", PATH_MAX_LENGTH);
    strlcpy(options->path, "\0", PATH_MAX_LENGTH);
    options->help = 0;
    options->arg_delete_alias = 0;
    options->reset_database = 0;

    return 0;
}