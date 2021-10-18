//
// Created by toke019 on 8.10.2021.
//

#ifndef JUMP_ARGUMENT_PARSER_H
#define JUMP_ARGUMENT_PARSER_H

#include "extern.h"

struct arguments {
    char alias[ALIAS_MAX_LENGTH];
    char path[PATH_MAX_LENGTH];
    unsigned int help: 1;
    unsigned int arg_delete_alias: 1;
    unsigned int reset_database: 1;
    unsigned int show_all: 1;
};

int parse_arguments(struct arguments *args, int argc, char **argv);

#endif //JUMP_ARGUMENT_PARSER_H
