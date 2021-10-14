//
// Created by toke019 on 8.10.2021.
//

#ifndef MCD_ARGUMENT_PARSER_H
#define MCD_ARGUMENT_PARSER_H

#include "utils.h" // max lengths

struct arguments {
    char alias[ALIAS_MAX_LENGTH];
    char path[PATH_MAX_LENGTH];
    u_int8_t help: 1;
    u_int8_t arg_delete_alias: 1;
    u_int8_t reset_database: 1;
};

int parse_arguments(struct arguments *args, int argc, char **argv);

#endif //MCD_ARGUMENT_PARSER_H
