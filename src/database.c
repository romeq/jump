//
// Created by toke019 on 8.10.2021.
//

#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include <wordexp.h>
#include <sqlite3.h>
#include <errno.h>

#include "../include/extern.h"
#include "../include/utils.h"
#include "../include/database.h"

int     validate_DB_existence(char *, char *);
int     get_db_path(char *, char *);


struct DBHandler
init_db() {
    sqlite3            *db;
    struct DBHandler    db_handler = {0, 0};
    char                db_path[PATH_MAX_LENGTH];
    char                cfg_full_path[PATH_MAX_LENGTH];

    get_db_path(db_path, cfg_full_path);

    int err;
    if ((err = validate_DB_existence(db_path, cfg_full_path)) < 0) {
        db_handler.succeeded = 0;
        return db_handler;
    }

    sqlite3_open(cfg_full_path, &db);
    db_handler.db_connection = db;

    return db_handler;
}

int
validate_DB_existence(char *config_dir, char *config_full_path)
{
    struct stat cfg_dir_stat = {0};
    if (stat(config_dir, &cfg_dir_stat) == -1) {
        errno = 0;
        if (mkdir(config_dir, 0755) > 0) {
            switch (errno) {
                case EACCES:
                    print_err("Unable to create the config folder: Permission denied");
                    break;
                case EEXIST:
                    print_err("Unable to create the config folder: Already exists");
                    break;
                default:
                    print_err("Unable to create a config folder, sorry!");
                    break;
            }
            return -1;
        }
    }

    struct stat cfg_full_stat = {0};
    if (stat(config_full_path, &cfg_full_stat) == -1) {
        if (fopen(config_full_path, "w")==NULL) {
            print_err("Unable to create a new database, sorry! ");
            return -1;
        }
    }

    return 0;
}


int
get_db_path(char *dest_dir, char *dest_full)
{
    int     standard_exists;
    char    config_home[PATH_MAX_LENGTH];
    char   *xdg_config_home;
    char    config_full[PATH_MAX_LENGTH];

    xdg_config_home = getenv("XDG_CONFIG_HOME");
    standard_exists = xdg_config_home != NULL &&
            strlen(xdg_config_home) > 3;

    if (standard_exists == 1) {
        strlcpy(config_home, xdg_config_home, PATH_MAX_LENGTH-1);
    } else {
        wordexp_t p;
        wordexp( DB_DIR , &p, 0 );
        strlcpy(config_home, *p.we_wordv, PATH_MAX_LENGTH-1);
        wordfree(&p);
    }

    if (strncmp(&config_home[strlen(config_home)], "/", 1)!=0) {
        strlcat(config_home, "/", PATH_MAX_LENGTH);
    }

    strlcpy(config_full, config_home, sizeof(config_full) - 1);
    strlcat(config_full, DB_FILENAME, sizeof(config_full) - 1);

    strlcpy(dest_dir,  config_home, PATH_MAX_LENGTH - 1);
    strlcpy(dest_full, config_full, PATH_MAX_LENGTH - 1);

    return 0;
}