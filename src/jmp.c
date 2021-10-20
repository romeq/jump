//
// Created by toke019 on 5.10.2021.
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <sys/stat.h>

#include "../include/argument_parser.h"
#include "../include/database.h"
#include "../include/utils.h"

#ifdef __OpenBSD__
    #include <bsd/string.h>
#endif

#ifdef __linux__
    #include <bsd/string.h>
#endif

int handle_arguments(struct arguments *, int, sqlite3 *);
int show_all_callback(void *, int, char **, char **);

int
main(int argc, char *argv[])
{
    struct arguments args;
    sqlite3* controller;

    if ((controller = init_db()) == NULL)
        return 1;

    if (parse_arguments(&args, argc, argv) < 0)
        return 1;

    if (handle_arguments(&args, argc, controller) < 0)
        return 1;

    sqlite3_close(controller);
    return EXIT_SUCCESS;
}

int
show_all_callback(void *_unused, int count, char **column_value, char **column_name)
{
    // suppress warnings of unused parameters e.g. tell compiler we don't use them
    (void)(_unused);
    (void)(count);
    (void)(column_name);

    printf("\x1B[90m[\x1B[0m\x1B[37m%s\x1B[0m\x1B[90m]\x1B[0m %s\x1B[90m -> \x1B[0m%s\n",
           column_value[0], column_value[1], column_value[2]);
    return 0;
}

int
handle_arguments(struct arguments *args, int argc, sqlite3 *controller)
{
    if (argc == 2) {
        if (args->help == 1) {
            verbose_usage();
        }

        if (args->reset_database == 1) {
            char *confirm_question = "Are you sure you want to reset all aliases?";
            if (raw_confirm(confirm_question)) {
                if (reset_table(controller) < 0) {
                    return -1;
                } else {
                    printf("(SUCCESS) Process succeeded :)\n");
                }
            } else {
                printf("Bye!\n");
            }
        }

        if (args->show_all == 1) {
            show_all(controller, show_all_callback);
        }

        if (strlen(args->alias) > 0) {
            char result[PATH_MAX_LENGTH] = "\0";

            int err = find_record_by_alias(args->alias, controller, result);
            if (err < 0)
                return -1;
            else if (err == SQLITE_DONE) {
                print_err("Not found!");
                return -1;
            }
            else if (err == SQLITE_ROW)
                printf("%s\n", result); // result found!
        }

    }
    else if (argc == 3) {
        if (args->arg_delete_alias && strlen(args->alias) > 0) {
            if (delete_record_by_alias(args->alias, controller) < 0)
                return -1;
            else
                printf("(%s) Record deleted\n", "SUCCESS");
            return 0;
        }

        if (strlen(args->alias) > 0 && strlen(args->path) > 0) {


            time_t show_egg;
            srand(time(&show_egg) / 2);
            if (rand() % 5 == 2){
                if (strncmp(args->alias, "give", ALIAS_MAX_LENGTH) == 0) {
                    if (strncmp(args->path, "cookie", PATH_MAX_LENGTH) == 0) {
                        char uwuified_responses[4][64] = {
                                "c-cookie? thank you *blushes*",
                                "thank y-you U-UwU (≧◡≦)",
                                "c-cookie? t-thanks uwu",
                                "c-cookie? *blushes* (≧◡≦)",
                        };
                        time_t response_num;
                        srand(time(&response_num));
                        printf("%s\n", uwuified_responses[rand() % 4]);
                        return 0;
                    }
                }
            }
            struct stat dir_stat = {0};
            if (stat(args->path, &dir_stat) == -1) {
                fprintf(stderr, "Invalid path \"%s\".\n", args->path);
                return -1;
            }

            char absolute_path[PATH_MAX_LENGTH];
            if (*realpath(args->path, absolute_path) == -1) {
                print_err("Failed to get absolute path");
                return -1;
            }

            if (new_path(args->alias, absolute_path, controller) < 0) {
                return -1;
            } else {
                printf("Saved!\n"); // created
                return 0;
            }
        }
    }

    return 0;
}
