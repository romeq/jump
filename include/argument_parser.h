//
// Created by toke019 on 8.10.2021.
//

#ifndef MCD_ARGUMENT_PARSER_H
#define MCD_ARGUMENT_PARSER_H
#define PATH_MAX_LENGTH 256

struct arguments {
    char alias[PATH_MAX_LENGTH];
    char path[PATH_MAX_LENGTH];
    unsigned int  help: 1;
    unsigned int  arg_delete_alias: 1;
    unsigned int  reset_database: 1;
};

int handle_arguments(struct arguments *args, int argc, char **argv);

#endif //MCD_ARGUMENT_PARSER_H
