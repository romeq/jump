//
// Created by toke019 on 5.10.2021.
//

#include <stdio.h>
#include "../include/argument_parser.h"
#include "../include/utils.h"

int
main(int argc, char *argv[])
{
    struct arguments args;

    int parse_status;
    if ((parse_status = handle_arguments(&args, argc, argv)) != 0) {
        return parse_status;
    }

    if (args.help == 1) {
        verbose_usage();
        return 0;
    }

    return 0;
}